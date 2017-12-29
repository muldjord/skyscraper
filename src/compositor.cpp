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
      printf("Added image resource of type '%s'\n", image.resource.toStdString().c_str());
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
    QImage canvas;
    if(output.type == "cover") {
      canvas = game.coverData;
    } else if(output.type == "screenshot") {
      canvas = game.screenshotData;
    } else if(output.type == "wheel") {
      canvas = game.wheelData;
    } else if(output.type == "marquee") {
      canvas = game.marqueeData;
    }

    canvas = canvas.convertToFormat(QImage::Format_ARGB32);
    
    if(output.width == -1 && output.height != -1) {
      canvas = canvas.scaledToHeight(output.height, Qt::SmoothTransformation);
    } else if(output.width != -1 && output.height == -1) {
      canvas = canvas.scaledToWidth(output.width, Qt::SmoothTransformation);
    } else if(output.width != -1 && output.height != -1) {
      canvas = canvas.scaled(output.width, output.height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }

    if(!output.images.isEmpty()) {
      // Reset canvas since composite images exist
      canvas.fill(Qt::transparent);
      foreach(Image image, output.images) {
	QImage element;
	if(image.resource == "cover") {
	  element = game.coverData;
	} else if(image.resource == "screenshot") {
	  element = game.screenshotData;
	} else if(image.resource == "wheel") {
	  element = game.wheelData;
	} else if(image.resource == "marquee") {
	  element = game.marqueeData;
	}
	if(image.width == -1 && image.height != -1) {
	  element = element.scaledToHeight(image.height, Qt::SmoothTransformation);
	} else if(image.width != -1 && image.height == -1) {
	  element = element.scaledToWidth(image.width, Qt::SmoothTransformation);
	} else if(image.width != -1 && image.height != -1) {
	  element = element.scaled(image.width, image.height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	}
	// Update width + height as we will need them for easier placement and alignment
	image.width = element.width();
	image.height = element.height();
	if(image.shadowDistance != -1 || image.shadowSoftness != -1 || image.shadowOpacity != -1) {
	  element = applyShadow(element, image.shadowDistance, image.shadowSoftness, image.shadowOpacity);
	}
	QPainter painter;
	painter.begin(&canvas);
	int x = 0;
	if(image.align == "center") {
	  x = (canvas.width() / 2) - (image.width / 2);
	} else if(image.align == "right") {
	  x = canvas.width() - image.width;
	}
	if(image.x != -1) {
	  x += image.x;
	}
	int y = 0;
	if(image.valign == "middle") {
	  y = (canvas.height() / 2) - (image.height / 2);
	} else if(image.align == "bottom") {
	  y = canvas.height() - image.height;
	}
	if(image.y != -1) {
	  y += image.y;
	}
	painter.drawImage(x, y, element);
	painter.end();
      }
    }
    
    if(output.type == "cover") {
      if(canvas.save(config.coversFolder + "/" + completeBaseName + ".png"))
	game.coverFile = StrTools::xmlUnescape(config.coversFolder + "/" +
					       completeBaseName + ".png");
    } else if(output.type == "screenshot") {
      if(canvas.save(config.screenshotsFolder + "/" + completeBaseName + ".png"))
	game.screenshotFile = StrTools::xmlUnescape(config.screenshotsFolder + "/" +
						    completeBaseName + ".png");
    } else if(output.type == "wheel") {
      if(canvas.save(config.wheelsFolder + "/" + completeBaseName + ".png"))
	game.wheelFile = StrTools::xmlUnescape(config.wheelsFolder + "/" +
					       completeBaseName + ".png");
    } else if(output.type == "marquee") {
      if(canvas.save(config.marqueesFolder + "/" + completeBaseName + ".png"))
	game.marqueeFile = StrTools::xmlUnescape(config.marqueesFolder + "/" +
						 completeBaseName + ".png");
    }
  }
}

QImage Compositor::applyShadow(QImage &image, int distance, int softness, int opacity)
{
  if(distance == -1)
    distance = 0;
  if(softness == -1)
    softness = 5;
  if(opacity == -1)
    opacity = 50;

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
  
  return shadowImage;
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
