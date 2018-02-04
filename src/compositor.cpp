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

#include <cmath>
#include <QSettings>
#include <QPainter>

#include "compositor.h"
#include "strtools.h"

#include "fxshadow.h"
#include "fxmask.h"
#include "fxframe.h"
#include "fxrounded.h"
#include "fxstroke.h"
#include "fxbrightness.h"
#include "fxcontrast.h"
#include "fxbalance.h"
#include "fxopacity.h"
#include "fxgamebox.h"

Compositor::Compositor(Settings &config)
{
  this->config = &config;
}

bool Compositor::processXml()
{
  Layer newOutputs;
  
  QXmlStreamReader xml(config->artworkXml);
  
  // Init recursive parsing
  addLayer(newOutputs, xml);

  // Assign global outputs to these new outputs
  outputs = newOutputs;
  return true;
}

void Compositor::addLayer(Layer &layer, QXmlStreamReader &xml)
{
  while(xml.readNext() && !xml.atEnd()) {
    Layer newLayer;
    if(xml.isStartElement() && xml.name() == "output") {
      QXmlStreamAttributes attribs = xml.attributes();
      if(attribs.hasAttribute("type")) {
	newLayer.resource = attribs.value("", "type").toString();
	newLayer.type = T_OUTPUT;
      }
      if(attribs.hasAttribute("width"))
	newLayer.width = attribs.value("", "width").toInt();
      if(attribs.hasAttribute("height"))
	newLayer.height = attribs.value("", "height").toInt();

      if(newLayer.type != T_NONE) {
	addLayer(newLayer, xml);
	layer.layers.append(newLayer);
      }
    } else if(xml.isStartElement() && xml.name() == "layer") {
      QXmlStreamAttributes attribs = xml.attributes();
      newLayer.resource = attribs.value("", "resource").toString();
      newLayer.type = T_LAYER;
      if(attribs.hasAttribute("width"))
	newLayer.width = attribs.value("", "width").toInt();
      if(attribs.hasAttribute("height"))
	newLayer.height = attribs.value("", "height").toInt();
      if(attribs.hasAttribute("align"))
	newLayer.align = attribs.value("", "align").toString();
      if(attribs.hasAttribute("valign"))
	newLayer.valign = attribs.value("", "valign").toString();
      if(attribs.hasAttribute("x"))
	newLayer.x = attribs.value("", "x").toInt();
      if(attribs.hasAttribute("y"))
	newLayer.y = attribs.value("", "y").toInt();
      addLayer(newLayer, xml);
      layer.layers.append(newLayer);
    } else if(xml.isStartElement() && xml.name() == "shadow") {
      QXmlStreamAttributes attribs = xml.attributes();
      if(attribs.hasAttribute("distance") &&
	 attribs.hasAttribute("softness") &&
	 attribs.hasAttribute("opacity")) {
	newLayer.type = T_SHADOW;
	newLayer.distance = attribs.value("distance").toInt();
	newLayer.softness = attribs.value("softness").toInt();
	newLayer.opacity = attribs.value("opacity").toInt();
	layer.layers.append(newLayer);
      }
    } else if(xml.isStartElement() && xml.name() == "mask") {
      QXmlStreamAttributes attribs = xml.attributes();
      if(attribs.hasAttribute("file")) {
	newLayer.type = T_MASK;
	newLayer.resource = attribs.value("file").toString();
	if(attribs.hasAttribute("width"))
	  newLayer.width = attribs.value("", "width").toInt();
	if(attribs.hasAttribute("height"))
	  newLayer.height = attribs.value("", "height").toInt();
	if(attribs.hasAttribute("x"))
	  newLayer.x = attribs.value("", "x").toInt();
	if(attribs.hasAttribute("y"))
	  newLayer.y = attribs.value("", "y").toInt();
	layer.layers.append(newLayer);
      }
    } else if(xml.isStartElement() && xml.name() == "frame") {
      QXmlStreamAttributes attribs = xml.attributes();
      if(attribs.hasAttribute("file")) {
	newLayer.type = T_FRAME;
	newLayer.resource = attribs.value("file").toString();
	if(attribs.hasAttribute("width"))
	  newLayer.width = attribs.value("", "width").toInt();
	if(attribs.hasAttribute("height"))
	  newLayer.height = attribs.value("", "height").toInt();
	layer.layers.append(newLayer);
      }
    } else if(xml.isStartElement() && xml.name() == "stroke") {
      QXmlStreamAttributes attribs = xml.attributes();
      if(attribs.hasAttribute("width")) {
	newLayer.type = T_STROKE;
	newLayer.width = attribs.value("width").toInt();
	if(attribs.hasAttribute("red"))
	  newLayer.red = attribs.value("", "red").toInt();
	if(attribs.hasAttribute("green"))
	  newLayer.green = attribs.value("", "green").toInt();
	if(attribs.hasAttribute("blue"))
	  newLayer.blue = attribs.value("", "blue").toInt();
	layer.layers.append(newLayer);
      }
    } else if(xml.isStartElement() && xml.name() == "rounded") {
      QXmlStreamAttributes attribs = xml.attributes();
      if(attribs.hasAttribute("radius")) {
	newLayer.type = T_ROUNDED;
	newLayer.width = attribs.value("radius").toInt();
	layer.layers.append(newLayer);
      }
    } else if(xml.isStartElement() && xml.name() == "brightness") {
      QXmlStreamAttributes attribs = xml.attributes();
      if(attribs.hasAttribute("value")) {
	newLayer.type = T_BRIGHTNESS;
	newLayer.delta = attribs.value("value").toInt();
	layer.layers.append(newLayer);
      }
    } else if(xml.isStartElement() && xml.name() == "opacity") {
      QXmlStreamAttributes attribs = xml.attributes();
      if(attribs.hasAttribute("value")) {
	newLayer.type = T_OPACITY;
	newLayer.opacity = attribs.value("value").toInt();
	layer.layers.append(newLayer);
      }
    } else if(xml.isStartElement() && xml.name() == "contrast") {
      QXmlStreamAttributes attribs = xml.attributes();
      if(attribs.hasAttribute("value")) {
	newLayer.type = T_CONTRAST;
	newLayer.delta = attribs.value("value").toInt();
	layer.layers.append(newLayer);
      }
    } else if(xml.isStartElement() && xml.name() == "balance") {
      QXmlStreamAttributes attribs = xml.attributes();
      newLayer.type = T_BALANCE;
      if(attribs.hasAttribute("red"))
	newLayer.red = attribs.value("", "red").toInt();
      if(attribs.hasAttribute("green"))
	newLayer.green = attribs.value("", "green").toInt();
      if(attribs.hasAttribute("blue"))
	newLayer.blue = attribs.value("", "blue").toInt();
      layer.layers.append(newLayer);
    } else if(xml.isStartElement() && xml.name() == "gamebox") {
      QXmlStreamAttributes attribs = xml.attributes();
      newLayer.type = T_GAMEBOX;
      if(attribs.hasAttribute("front"))
	newLayer.resource = attribs.value("front").toString();
      if(attribs.hasAttribute("side"))
	newLayer.resource2 = attribs.value("side").toString();
      layer.layers.append(newLayer);
    } else if(xml.isEndElement() && xml.name() == "layer") {
      return;
    } else if(xml.isEndElement() && xml.name() == "output") {
      return;
    }
  }
}

void Compositor::saveAll(GameEntry &game, QString completeBaseName)
{
  foreach(Layer output, outputs.layers) {
    QImage canvas;
    if(output.resource == "cover") {
      canvas = game.coverData;
    } else if(output.resource == "screenshot") {
      canvas = game.screenshotData;
    } else if(output.resource == "wheel") {
      canvas = game.wheelData;
    } else if(output.resource == "marquee") {
      canvas = game.marqueeData;
    }

    canvas = canvas.convertToFormat(QImage::Format_ARGB32_Premultiplied);
    canvas = cropToFit(canvas);
    
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
      // Initiate recursive compositing
      compositeLayer(game, canvas, output);
    }

    if(output.resource == "cover") {
      if(canvas.convertToFormat(QImage::Format_ARGB6666_Premultiplied).save(config->coversFolder + "/" + completeBaseName + ".png"))
	game.coverFile = StrTools::xmlUnescape(config->coversFolder + "/" +
					       completeBaseName + ".png");
    } else if(output.resource == "screenshot") {
      if(canvas.convertToFormat(QImage::Format_ARGB6666_Premultiplied).save(config->screenshotsFolder + "/" + completeBaseName + ".png"))
	game.screenshotFile = StrTools::xmlUnescape(config->screenshotsFolder + "/" +
						    completeBaseName + ".png");
    } else if(output.resource == "wheel") {
      if(canvas.convertToFormat(QImage::Format_ARGB6666_Premultiplied).save(config->wheelsFolder + "/" + completeBaseName + ".png"))
	game.wheelFile = StrTools::xmlUnescape(config->wheelsFolder + "/" +
					       completeBaseName + ".png");
    } else if(output.resource == "marquee") {
      if(canvas.convertToFormat(QImage::Format_ARGB6666_Premultiplied).save(config->marqueesFolder + "/" + completeBaseName + ".png"))
	game.marqueeFile = StrTools::xmlUnescape(config->marqueesFolder + "/" +
						 completeBaseName + ".png");
    }
  }
}

void Compositor::compositeLayer(GameEntry &game, QImage &canvas, Layer &layer)
{
  for(int a = 0; a < layer.layers.length(); ++a) {
    QImage thisCanvas;
    Layer thisLayer = layer.layers.at(a);

    if(thisLayer.type == T_LAYER) {
      if(thisLayer.resource == "") {
	QImage emptyCanvas(1, 1, QImage::Format_ARGB32_Premultiplied);
	emptyCanvas.fill(Qt::transparent);
	thisCanvas = emptyCanvas;
      } else if(thisLayer.resource == "cover") {
	thisCanvas = game.coverData;
      } else if(thisLayer.resource == "screenshot") {
	thisCanvas = game.screenshotData;
      } else if(thisLayer.resource == "wheel") {
	thisCanvas = game.wheelData;
      } else if(thisLayer.resource == "marquee") {
	thisCanvas = game.marqueeData;
      } else {
	thisCanvas = QImage(config->resources[thisLayer.resource]);
      }

      thisCanvas = thisCanvas.convertToFormat(QImage::Format_ARGB32_Premultiplied);
      thisCanvas = cropToFit(thisCanvas);

      if(thisLayer.width == -1 && thisLayer.height != -1) {
	thisCanvas = thisCanvas.scaledToHeight(thisLayer.height, Qt::SmoothTransformation);
      } else if(thisLayer.width != -1 && thisLayer.height == -1) {
	thisCanvas = thisCanvas.scaledToWidth(thisLayer.width, Qt::SmoothTransformation);
      } else if(thisLayer.width != -1 && thisLayer.height != -1) {
	thisCanvas = thisCanvas.scaled(thisLayer.width, thisLayer.height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
      }
      // Update width + height as we will need them for easier placement and alignment
      thisLayer.width = thisCanvas.width();
      thisLayer.height = thisCanvas.height();
      
      if(!thisLayer.layers.isEmpty()) {
	compositeLayer(game, thisCanvas, thisLayer);
      }
    } else if(thisLayer.type == T_SHADOW) {
      FxShadow effect;
      canvas = effect.applyEffect(canvas, thisLayer);
    } else if(thisLayer.type == T_MASK) {
      FxMask effect;
      canvas = effect.applyEffect(canvas, thisLayer, config);
    } else if(thisLayer.type == T_FRAME) {
      FxFrame effect;
      canvas = effect.applyEffect(canvas, thisLayer, config);
    } else if(thisLayer.type == T_STROKE) {
      FxStroke effect;
      canvas = effect.applyEffect(canvas, thisLayer);
      layer.width = canvas.width();
      layer.height = canvas.height();
    } else if(thisLayer.type == T_ROUNDED) {
      FxRounded effect;
      canvas = effect.applyEffect(canvas, thisLayer);
    } else if(thisLayer.type == T_BRIGHTNESS) {
      FxBrightness effect;
      canvas = effect.applyEffect(canvas, thisLayer);
    } else if(thisLayer.type == T_CONTRAST) {
      FxContrast effect;
      canvas = effect.applyEffect(canvas, thisLayer);
    } else if(thisLayer.type == T_BALANCE) {
      FxBalance effect;
      canvas = effect.applyEffect(canvas, thisLayer);
    } else if(thisLayer.type == T_OPACITY) {
      FxOpacity effect;
      canvas = effect.applyEffect(canvas, thisLayer);
    } else if(thisLayer.type == T_GAMEBOX) {
      FxGamebox effect;
      canvas = effect.applyEffect(canvas, thisLayer, game, config);
    }

    // Only draw this layer if it's actually defined
    if(thisLayer.type != T_NONE) {
      QPainter painter;
      painter.begin(&canvas);
      
      int x = 0;
      if(thisLayer.align == "center") {
	x = (layer.width / 2) - (thisLayer.width / 2);
      } else if(thisLayer.align == "right") {
	x = layer.width - thisLayer.width;
      }
      x += thisLayer.x;
      
      int y = 0;
      if(thisLayer.valign == "middle") {
	y = (layer.height / 2) - (thisLayer.height / 2);
      } else if(thisLayer.valign == "bottom") {
	y = layer.height - thisLayer.height;
      }
      y += thisLayer.y;
      
      painter.drawImage(x, y, thisCanvas);
      painter.end();
    }
  }
}

QImage Compositor::cropToFit(QImage &image)
{
  int left = image.width();
  int right = 0;
  int top = 0;
  int bottom = image.height();

  // Find lefmost edge
  for(int y = 0; y < image.height(); ++y) {
    QRgb *scanline = (QRgb *)image.scanLine(y);
    for(int x = 0; x < image.width(); ++x) {
      if(qAlpha(scanline[x]) > 0) {
	if(left > x) {
	  left = x;
	}
	if(right < x) {
	  right = x + 1;
	}
	if(top == 0) {
	  top = y;
	}
	bottom = y + 1;
      }
    }
  }
  QImage cropped = image.copy(left, top, right - left, bottom - top);
  
  return cropped;
}
