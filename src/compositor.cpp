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
#include <QMatrix3x3>

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
  
  bool inLayer = false;
  bool inOutput = false;
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
      outputs.append(output);
      inLayer = false;
      inOutput = true;
    }
    if(xml.name() == "layer" && xml.isStartElement() && inOutput) {
      Layer layer;
      QXmlStreamAttributes attribs = xml.attributes();
      if(attribs.hasAttribute("resource"))
	layer.resource = attribs.value("", "resource").toString();
      if(attribs.hasAttribute("width"))
	layer.width = attribs.value("", "width").toInt();
      if(attribs.hasAttribute("height"))
	layer.height = attribs.value("", "height").toInt();
      if(attribs.hasAttribute("align"))
	layer.align = attribs.value("", "align").toString();
      if(attribs.hasAttribute("valign"))
	layer.valign = attribs.value("", "valign").toString();
      if(attribs.hasAttribute("x"))
	layer.x = attribs.value("", "x").toInt();
      if(attribs.hasAttribute("y"))
	layer.y = attribs.value("", "y").toInt();
      outputs.last().layers.append(layer);
      inLayer = true;
    }
    if(xml.name() == "shadow" && xml.isStartElement() && inOutput && inLayer) {
      QXmlStreamAttributes attribs = xml.attributes();
      if(attribs.hasAttribute("distance"))
	outputs.last().layers.last().shadowDistance = attribs.value("distance").toInt();
      if(attribs.hasAttribute("softness"))
	outputs.last().layers.last().shadowSoftness = attribs.value("softness").toInt();
      if(attribs.hasAttribute("opacity"))
	outputs.last().layers.last().shadowOpacity = attribs.value("opacity").toInt();
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

    canvas = canvas.convertToFormat(QImage::Format_ARGB32_Premultiplied);
    
    if(output.width == -1 && output.height != -1) {
      canvas = canvas.scaledToHeight(output.height, Qt::SmoothTransformation);
    } else if(output.width != -1 && output.height == -1) {
      canvas = canvas.scaledToWidth(output.width, Qt::SmoothTransformation);
    } else if(output.width != -1 && output.height != -1) {
      canvas = canvas.scaled(output.width, output.height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }

    if(!output.layers.isEmpty()) {
      // Reset canvas since composite layers exist
      canvas.fill(Qt::transparent);
      for(int a = output.layers.size() - 1; a >= 0; a--) {
	Layer layer = output.layers.at(a);
	QImage element;
	if(layer.resource == "cover") {
	  element = game.coverData;
	} else if(layer.resource == "screenshot") {
	  element = game.screenshotData;
	} else if(layer.resource == "wheel") {
	  element = game.wheelData;
	} else if(layer.resource == "marquee") {
	  element = game.marqueeData;
	}
	if(layer.width == -1 && layer.height != -1) {
	  element = element.scaledToHeight(layer.height, Qt::SmoothTransformation);
	} else if(layer.width != -1 && layer.height == -1) {
	  element = element.scaledToWidth(layer.width, Qt::SmoothTransformation);
	} else if(layer.width != -1 && layer.height != -1) {
	  element = element.scaled(layer.width, layer.height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	}
	// Update width + height as we will need them for easier placement and alignment
	layer.width = element.width();
	layer.height = element.height();
	if(layer.shadowDistance != -1 || layer.shadowSoftness != -1 || layer.shadowOpacity != -1) {
	  element = applyShadow(element, layer.shadowDistance, layer.shadowSoftness, layer.shadowOpacity);
	}
	QPainter painter;
	painter.begin(&canvas);

	int x = 0;
	if(layer.align == "center") {
	  x = (canvas.width() / 2) - (layer.width / 2);
	} else if(layer.align == "right") {
	  x = canvas.width() - layer.width;
	}
	x += layer.x;

	int y = 0;
	if(layer.valign == "middle") {
	  y = (canvas.height() / 2) - (layer.height / 2);
	} else if(layer.valign == "bottom") {
	  y = canvas.height() - layer.height;
	}
	y += layer.y;

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
  
  
  QImage shadow(image.width() + softness, image.height() + softness,
		QImage::Format_ARGB32_Premultiplied);
  
  shadow.fill(Qt::transparent);

  QImage matrix(softness, softness, QImage::Format_Grayscale8);
  QPainter painter;
  painter.begin(&matrix);
  
  for(int y = 1; y <= image.height() - 1; ++y) {
    for(int x = 1; x <= image.width() - 1; ++x) {
  
  
  
  QMatrix3x3 kernel;
  kernel(0, 0) = 1; kernel(0, 1) = 2; kernel(0, 2) = 1;
  kernel(1, 0) = 2; kernel(1, 1) = 4; kernel(1, 2) = 2;
  kernel(2, 0) = 1; kernel(2, 1) = 2; kernel(2, 2) = 1;
  float kernel_sum = 16.0;
  
  for(int y = 1; y <= image.height() - 1; ++y) {
    for(int x = 1; x <= image.width() - 1; ++x) {
      float alpha = 0;
      
      alpha =
	kernel(0, 0) * qAlpha(image.pixel(x+1, y+1)) +
	kernel(0, 1) * qAlpha(image.pixel(x, y+1)) +
	kernel(0, 2) * qAlpha(image.pixel(x-1, y+1)) +
	
	kernel(1, 0) * qAlpha(image.pixel(x+1, y)) +
	kernel(1, 1) * qAlpha(image.pixel(x, y)) +
	kernel(1, 2) * qAlpha(image.pixel(x-1, y)) +
	
	kernel(2, 0) * qAlpha(image.pixel(x+1, y-1)) +
	kernel(2, 1) * qAlpha(image.pixel(x, y-1)) +
	kernel(2, 2) * qAlpha(image.pixel(x-1, y-1));
      shadow.setPixelColor(x, y, QColor(0, 0, 0, alpha / kernel_sum));
      
    }
  }
    
  QImage shadowImage(image.width() + distance,
		     image.height() + distance,
		     QImage::Format_ARGB32_Premultiplied);
  shadowImage.fill(Qt::transparent);
  QPainter painter;
  painter.begin(&shadowImage);
  painter.setOpacity(opacity * 0.01);
  painter.drawImage(distance, distance, shadow);
  painter.setOpacity(1.0);
  painter.drawImage(0, 0, image);
  painter.end();
  
  return shadowImage;
}
/*
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
*/
