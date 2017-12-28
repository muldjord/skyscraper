/***************************************************************************
 *            compositor.cpp
 *
 *  Wed Jun 18 12:00:00 CEST 2017
 *  Copyright 2017 Lars Muldjord
 *  muldjordlars@gmail.com
 ****************************************************************************/
/*
 *  This file is part of skyscraper.
 *
 *  skyscraper is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  skyscraper is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with skyscraper; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
 */

#include <QSettings>
#include <QPainter>
#include <QFile>
#include <QXmlStreamReader>

#include "compositor.h"
#include "strtools.h"

Compositor::Compositor(Settings &config)
{
  this->config = config;
}

bool Compositor::processXml(QString filename)
{
  outputs.clear();
  
  QFile xmlFile(filename);
  if(!xmlFile.open(QIODevice::ReadOnly)) {
    return false;
  }
  QXmlStreamReader xml(&xmlFile);
  
  while(xml.readNext() && !xml.atEnd()) {
    if(xml.name() == "output" && xml.isStartElement()) {
      Output output;
      QXmlStreamAttributes attribs = xml.attributes();
      if(attribs.hasAttribute("type"))
	output.type = attribs.value("", "type").toString();
      if(attribs.hasAttribute("width"))
	output.width = attribs.value("", "width").toInt();
      if(attribs.hasAttribute("height"))
	output.height = attribs.value("", "height").toInt();
      printf("Added artwork of type '%s'\n", output.type.toStdString().c_str());
      outputs.append(output);
    }
    if(xml.name() == "image" && xml.isStartElement()) {
      Image image;
      QXmlStreamAttributes attribs = xml.attributes();
      if(attribs.hasAttribute("resource"))
	image.resource = attribs.value("", "resource").toString();
      if(attribs.hasAttribute("width"))
	image.width = attribs.value("", "width").toInt();
      if(attribs.hasAttribute("height"))
	image.height = attribs.value("", "height").toInt();
      if(attribs.hasAttribute("align"))
	image.align = attribs.value("", "align").toString();
      if(attribs.hasAttribute("valign"))
	image.valign = attribs.value("", "valign").toString();
      if(attribs.hasAttribute("x"))
	image.x = attribs.value("", "x").toInt();
      if(attribs.hasAttribute("y"))
	image.y = attribs.value("", "y").toInt();
      outputs.last().images.append(image);
    }
    if(xml.name() == "shadow" && xml.isStartElement()) {
      QXmlStreamAttributes attribs = xml.attributes();
      if(attribs.hasAttribute("distance"))
	outputs.last().images.last().shadowDistance = attribs.value("", "distance").toInt();
      if(attribs.hasAttribute("softness"))
	outputs.last().images.last().shadowSoftness = attribs.value("", "softness").toInt();
      if(attribs.hasAttribute("opacity"))
	outputs.last().images.last().shadowOpacity = attribs.value("", "opacity").toInt();
    }
  }
  xmlFile.close();

  return true;
}

void Compositor::saveAll(GameEntry &game, QString completeBaseName)
{
  foreach(Output output, outputs) {
    if(output.width == -1 && output.height == -1 && output.images.isEmpty()) {
      if(output.type == "cover") {
	if(game.coverData.save(config.coversFolder + "/" + completeBaseName + ".png"))
	  game.coverFile = StrTools::xmlUnescape(config.coversFolder + "/" +
						 completeBaseName + ".png");
      } else if(output.type == "screenshot") {
	if(game.screenshotData.save(config.screenshotsFolder + "/" + completeBaseName + ".png"))
	  game.screenshotFile = StrTools::xmlUnescape(config.screenshotsFolder + "/" +
						      completeBaseName + ".png");
      } else if(output.type == "wheel") {
	if(game.wheelData.save(config.wheelsFolder + "/" + completeBaseName + ".png"))
	  game.wheelFile = StrTools::xmlUnescape(config.wheelsFolder + "/" +
						 completeBaseName + ".png");
      } else if(output.type == "marquee") {
	if(game.marqueeData.save(config.marqueesFolder + "/" + completeBaseName + ".png"))
	  game.marqueeFile = StrTools::xmlUnescape(config.marqueesFolder + "/" +
						   completeBaseName + ".png");
      }
    }
    
  }
}

/*
QImage Compositor::composite()
{
  if(config.coverYSet == false) {
    config.coverY = config.finalImageHeight - config.coverHeight -
      (config.coverShadowEnabled?config.coverShadowDistance:0);
  }
  if(config.screenshotXSet == false) {
    config.screenshotX = config.finalImageWidth - config.screenshotWidth -
      (config.screenshotShadowEnabled?config.screenshotShadowDistance:0);
  }

  if(cover.isNull()) {
    config.coverEnabled = false;
    cover = QImage(config.coverWidth, config.coverHeight, QImage::Format_ARGB32);
    cover.fill(Qt::transparent);
  }
  if(cover.width() > cover.height()) {
    cover = cover.scaled(config.coverWidth, config.coverHeight,
			 Qt::KeepAspectRatioByExpanding,
			 Qt::SmoothTransformation);
  } else {
    cover = cover.scaled(config.coverWidth, config.coverHeight,
			 Qt::IgnoreAspectRatio,
			 Qt::SmoothTransformation);
  }
  if(config.coverShadowEnabled) {
    applyShadow(cover, config.coverShadowDistance, config.coverShadowSoftness,
		config.coverShadowOpacity);
  }

  if(screenshot.isNull()) {
    config.screenshotEnabled = false;
    screenshot = QImage(config.screenshotWidth, config.screenshotHeight, QImage::Format_ARGB32);
    screenshot.fill(Qt::transparent);
  }
  screenshot = screenshot.scaled(config.screenshotWidth, config.screenshotHeight,
				 Qt::IgnoreAspectRatio,
				 Qt::SmoothTransformation);

  if(config.screenshotShadowEnabled) {
    applyShadow(screenshot, config.screenshotShadowDistance,
		config.screenshotShadowSoftness, config.screenshotShadowOpacity);
  }

  QImage finalImage(config.finalImageWidth, config.finalImageHeight, QImage::Format_ARGB32);
  finalImage.fill(Qt::transparent);

  QPainter painter(&finalImage);

  if(config.screenshotEnabled) {
    painter.drawImage(config.screenshotX, config.screenshotY, screenshot);
  }
  if(config.coverEnabled) {
    painter.drawImage(config.coverX, config.coverY, cover);
  }
  
  return finalImage;
}

bool Compositor::parseXml(QString xmlFile)
{
}
*/
void Compositor::applyShadow(QImage &image, int distance, int softness, int opacity)
{
  QPainter painter;
  
  QImage shadow(image.width(), image.height(), QImage::Format_ARGB32);
  
  shadow.fill(Qt::transparent);
  painter.begin(&shadow);
  painter.setBrush(QColor(0, 0, 0, 255 / softness));
  painter.setPen(QColor(0, 0, 0, 0));
  for(qreal a = softness; a >= 0.0; --a) {
    painter.drawRoundedRect(softness - a, softness - a,
			    shadow.width() + ((a - softness) * 2),
			    image.height() + ((a - softness) * 2),
			    a, a);
  }
  painter.end();
  
  QImage shadowImage(image.width() + distance,
		     image.height() + distance,
		     QImage::Format_ARGB32);
  shadowImage.fill(Qt::transparent);
  painter.begin(&shadowImage);
  painter.setOpacity(opacity * 0.01);
  painter.drawImage(distance, distance, shadow);
  painter.setOpacity(1.0);
  painter.drawImage(0, 0, image);
  painter.end();
  
  image = shadowImage;
}

/*
      if(config.noComposite) {
	if(!game.screenshotData.isNull()) {
	  if(game.screenshotData.save(config.screenshotsFolder + "/" + info.completeBaseName() + ".png"))
	    game.screenshotFile = StrTools::xmlUnescape(config.screenshotsFolder + "/" + info.completeBaseName() + ".png");
	}
	if(!game.coverData.isNull()) {
	  if(game.coverData.save(config.coversFolder + "/" + info.completeBaseName() + ".png"))
	    game.coverFile = StrTools::xmlUnescape(config.coversFolder + "/" + info.completeBaseName() + ".png");
	}
	if(!game.wheelData.isNull()) {
	  if(game.wheelData.save(config.wheelsFolder + "/" + info.completeBaseName() + ".png"))
	    game.wheelFile = StrTools::xmlUnescape(config.wheelsFolder + "/" + info.completeBaseName() + ".png");
	}
	if(!game.marqueeData.isNull()) {
	  if(game.marqueeData.save(config.marqueesFolder + "/" + info.completeBaseName() + ".png"))
	    game.marqueeFile = StrTools::xmlUnescape(config.marqueesFolder + "/" + info.completeBaseName() + ".png");
	}
      } else {
	Compositor artCreator;
	if(artCreator.composite(game.coverData, game.screenshotData, config).save(config.screenshotsFolder + "/" + info.completeBaseName() + ".png")) {
	  game.screenshotFile = StrTools::xmlUnescape(config.screenshotsFolder + "/" + info.completeBaseName() + ".png");
	}
      }
*/
