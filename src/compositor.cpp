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
#include <QFile>

#include "compositor.h"
#include "strtools.h"

Compositor::Compositor(Settings &config)
{
  this->config = config;
}

bool Compositor::processXml(QString filename)
{
  Layer newOutputs;
  
  QFile xmlFile(filename);
  if(!xmlFile.open(QIODevice::ReadOnly)) {
    return false;
  }
  QXmlStreamReader xml(&xmlFile);
  
  // Init recursive parsing
  addLayer(newOutputs, xml);

  xmlFile.close();

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
	printf("%sAdded output\n", indentation.toStdString().c_str());
	indentation += "  ";
	addLayer(newLayer, xml);
	layer.layers.append(newLayer);
      }
    } else if(xml.isStartElement() && xml.name() == "layer") {
      QXmlStreamAttributes attribs = xml.attributes();
      if(attribs.hasAttribute("resource")) {
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
	printf("%sAdded layer\n", indentation.toStdString().c_str());
	indentation += "  ";
	addLayer(newLayer, xml);
	layer.layers.append(newLayer);
      }
    } else if(xml.isStartElement() && xml.name() == "shadow") {
      QXmlStreamAttributes attribs = xml.attributes();
      if(attribs.hasAttribute("distance") &&
	 attribs.hasAttribute("softness") &&
	 attribs.hasAttribute("opacity")) {
	newLayer.type = T_SHADOW;
	newLayer.shadowDistance = attribs.value("distance").toInt();
	newLayer.shadowSoftness = attribs.value("softness").toInt();
	newLayer.shadowOpacity = attribs.value("opacity").toInt();
	layer.layers.append(newLayer);
	printf("%sAdded shadow\n", indentation.toStdString().c_str());
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
	layer.layers.append(newLayer);
	printf("%sAdded mask\n", indentation.toStdString().c_str());
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
	printf("%sAdded frame\n", indentation.toStdString().c_str());
      }
    } else if(xml.isEndElement() && xml.name() == "layer") {
      indentation = indentation.left(indentation.length() - 2);
      printf("%sEnding layer\n", indentation.toStdString().c_str());
      return;
    } else if(xml.isEndElement() && xml.name() == "output") {
      indentation = indentation.left(indentation.length() - 2);
      printf("%sEnding output\n", indentation.toStdString().c_str());
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
      if(canvas.convertToFormat(QImage::Format_ARGB6666_Premultiplied).save(config.coversFolder + "/" + completeBaseName + ".png"))
	game.coverFile = StrTools::xmlUnescape(config.coversFolder + "/" +
					       completeBaseName + ".png");
    } else if(output.resource == "screenshot") {
      if(canvas.convertToFormat(QImage::Format_ARGB6666_Premultiplied).save(config.screenshotsFolder + "/" + completeBaseName + ".png"))
	game.screenshotFile = StrTools::xmlUnescape(config.screenshotsFolder + "/" +
						    completeBaseName + ".png");
    } else if(output.resource == "wheel") {
      if(canvas.convertToFormat(QImage::Format_ARGB6666_Premultiplied).save(config.wheelsFolder + "/" + completeBaseName + ".png"))
	game.wheelFile = StrTools::xmlUnescape(config.wheelsFolder + "/" +
					       completeBaseName + ".png");
    } else if(output.resource == "marquee") {
      if(canvas.convertToFormat(QImage::Format_ARGB6666_Premultiplied).save(config.marqueesFolder + "/" + completeBaseName + ".png"))
	game.marqueeFile = StrTools::xmlUnescape(config.marqueesFolder + "/" +
						 completeBaseName + ".png");
    }
  }
}

void Compositor::compositeLayer(GameEntry &game, QImage &canvas, Layer &layer)
{
  for(int a = 0; a < layer.layers.length(); ++a) {
    QImage thisCanvas;
    Layer thisLayer = layer.layers.at(a);

    switch(thisLayer.type) {
    case T_LAYER:
      if(thisLayer.resource == "cover") {
	printf("Res: cover\n");
	thisCanvas = game.coverData;
      } else if(thisLayer.resource == "screenshot") {
	printf("Res: screenshot\n");
	thisCanvas = game.screenshotData;
      } else if(thisLayer.resource == "wheel") {
	printf("Res: wheel\n");
	thisCanvas = game.wheelData;
      } else if(thisLayer.resource == "marquee") {
	printf("Resource: marquee\n");
	thisCanvas = game.marqueeData;
      } else {
	printf("Res: file\n");
	thisCanvas = QImage("resources/" + thisLayer.resource);
      }

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
      break;
    case T_SHADOW:
      canvas = applyShadow(canvas, thisLayer);
      break;
    case T_MASK:
      canvas = applyMask(canvas, thisLayer);
      break;
    case T_FRAME:
      canvas = applyFrame(canvas, thisLayer);
      break;
    }
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

QImage Compositor::applyMask(QImage &image, Layer &layer)
{
  QString file = layer.resource;

  QImage mask("resources/" + file);
  mask = mask.convertToFormat(QImage::Format_ARGB32_Premultiplied);

  if(layer.width == -1 && layer.height == -1) {
    mask = mask.scaled(image.width(), image.height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
  } else if(layer.width == -1 && layer.height != -1) {
    mask = mask.scaledToHeight(layer.height, Qt::SmoothTransformation);
  } else if(layer.width != -1 && layer.height == -1) {
    mask = mask.scaledToWidth(layer.width, Qt::SmoothTransformation);
  } else if(layer.width != -1 && layer.height != -1) {
    mask = mask.scaled(layer.width, layer.height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
  }
  
  QPainter painter;
  painter.begin(&image);
  painter.setCompositionMode(QPainter::CompositionMode_DestinationAtop);
  painter.drawImage(0, 0, mask);
  painter.end();

  return image;
}

QImage Compositor::applyFrame(QImage &image, Layer &layer)
{
  QString file = layer.resource;

  QImage frame("resources/" + file);
  frame = frame.convertToFormat(QImage::Format_ARGB32_Premultiplied);

  if(layer.width == -1 && layer.height == -1) {
    frame = frame.scaled(image.width(), image.height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
  } else if(layer.width == -1 && layer.height != -1) {
    frame = frame.scaledToHeight(layer.height, Qt::SmoothTransformation);
  } else if(layer.width != -1 && layer.height == -1) {
    frame = frame.scaledToWidth(layer.width, Qt::SmoothTransformation);
  } else if(layer.width != -1 && layer.height != -1) {
    frame = frame.scaled(layer.width, layer.height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
  }
  
  QPainter painter;
  painter.begin(&image);
  painter.drawImage(0, 0, frame);
  painter.end();

  return image;
}

QVector<double> Compositor::getGaussBoxes(double sigma, double n)
{
  double wIdeal = sqrt((12.0 * sigma * sigma / n) + 1.0);
  double wl = floor(wIdeal);
  if((int)wl % 2 == 0) {
    wl--;
  }
  double wu = wl + 2;
  double mIdeal = (12.0 * sigma * sigma - n * wl * wl - 4.0 * n * wl - 3.0 * n) / (-4.0 * wl - 4.0);
  int m = round(mIdeal);
  
  QVector<double> sizes;
  for(int i = 0; i < n; i++) {
    sizes.append(i < m?wl:wu);
  }
  return sizes;
}

void Compositor::boxBlur(QRgb *src, QRgb *dst, int width, int height, int radius)
{
  for(int i = 0; i < width * height; i++) {
    dst[i] = src[i];
  }
  boxBlurHorizontal(dst, src, width, height, radius);
  boxBlurTotal(src, dst, width, height, radius);
}

void Compositor::boxBlurHorizontal(QRgb *src, QRgb *dst, int width, int height, int radius)
{
  int span = radius + radius + 1;
  for(int y = 0; y < height; y++) {
    int currentIdx = y * width, frontIdx = currentIdx, backIdx = currentIdx + radius;
    int firstVal = qAlpha(src[currentIdx]), lastVal = qAlpha(src[currentIdx + width - 1]);

    // Initial 'value' fill at leftmost edge
    int value = (radius + 1) * firstVal;
    for(int x = 0; x < radius; x++) {
      value += qAlpha(src[currentIdx + x]);
    }
    
    for(int x = 0; x <= radius ; x++) {
      value += qAlpha(src[backIdx++]) - firstVal;
      dst[currentIdx++] = QColor(0, 0, 0, value / span).rgba();
    }
    for(int x = radius + 1; x < width - radius; x++) {
      value += qAlpha(src[backIdx++]) - qAlpha(src[frontIdx++]);
      dst[currentIdx++] = QColor(0, 0, 0, value / span).rgba();
    }
    for(int x = width- radius; x < width; x++) {
      value += lastVal - qAlpha(src[frontIdx++]);
      dst[currentIdx++] = QColor(0, 0, 0, value / span).rgba();
    }
  }
}

void Compositor::boxBlurTotal(QRgb *src, QRgb *dst, int width, int height, double radius)
{
  int span = radius + radius + 1;
  for(int i = 0; i < width; i++) {
    int currentIdx = i, frontIdx = currentIdx, backIdx = currentIdx + radius * width;
    int firstVal = qAlpha(src[currentIdx]),
      lastVal = qAlpha(src[currentIdx + width * (height - 1)]);

    // Initial 'value' fill at topmost edge
    int value = (radius + 1) * firstVal;
    for(int j = 0; j < radius; j++) {
      value += qAlpha(src[currentIdx + j * width]);
    }
    
    for(int j = 0; j <= radius ; j++) {
      value += qAlpha(src[backIdx] - firstVal);
      dst[currentIdx] = QColor(0, 0, 0, value / span).rgba();
      backIdx += width;
      currentIdx += width;
    }
    for(int j = radius + 1; j < height - radius; j++) {
      value += qAlpha(src[backIdx]) - qAlpha(src[frontIdx]);
      dst[currentIdx] = QColor(0, 0, 0, value / span).rgba();
      frontIdx += width;
      backIdx += width;
      currentIdx += width;
    }
    for(int j = height - radius; j < height; j++) {
      value += lastVal - qAlpha(src[frontIdx]);
      dst[currentIdx] = QColor(0, 0, 0, value / span).rgba();
      frontIdx += width;
      currentIdx += width;
    }
  }
}

QImage Compositor::applyShadow(QImage &image, Layer &layer)
{
  int distance = layer.shadowDistance;
  int softness = layer.shadowSoftness;
  int opacity = layer.shadowOpacity;

  if(distance == -1)
    distance = 0;
  if(softness == -1)
    softness = 5;
  if(opacity == -1)
    opacity = 50;

  if(image.isNull() || image.width() == 0 || image.height() == 0) {
    return image;
  }
  
  QImage buffer1(image.width() + softness * 2, image.height() + softness * 2,
		 QImage::Format_ARGB32_Premultiplied);
  buffer1.fill(Qt::transparent);
  QPainter painter;
  painter.begin(&buffer1);
  painter.drawImage(softness, softness, image);
  painter.end();
  
  QRgb *buffer1Bits = (QRgb *)buffer1.bits();
  for(int a = 0; a < buffer1.width() * buffer1.height(); ++a) {
    buffer1Bits[a] = QColor(0, 0, 0, qAlpha(buffer1Bits[a])).rgba();
  }
  QImage buffer2 = buffer1;
  QRgb *buffer2Bits = (QRgb *)buffer2.bits();
  
  int width = buffer1.width(), height = buffer1.height();
  
  QVector<double> boxes = getGaussBoxes((double)softness, 3.0);

  // Pass 1
  boxBlur(buffer1Bits, buffer2Bits, width, height, (boxes[0] - 1) / 4);
  // Pass 2
  boxBlur(buffer2Bits, buffer1Bits, width, height, (boxes[1] - 1) / 4);
  // Pass 3, after this pass, we have a true gauss shadow
  boxBlur(buffer1Bits, buffer2Bits, width, height, (boxes[2] - 1) / 4);
  
  QImage resultImage(image.width() + distance + softness,
		     image.height() + distance + softness,
		     QImage::Format_ARGB32_Premultiplied);
  resultImage.fill(Qt::transparent);
  painter.begin(&resultImage);
  painter.setOpacity(opacity * 0.01);
  painter.drawImage(distance - softness, distance - softness, buffer2);
  painter.setOpacity(1.0);
  painter.drawImage(0, 0, image);
  painter.end();
  
  return resultImage;
}
