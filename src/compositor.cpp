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
      if(canvas.convertToFormat(QImage::Format_ARGB4444_Premultiplied).save(config.coversFolder + "/" + completeBaseName + ".png"))
	game.coverFile = StrTools::xmlUnescape(config.coversFolder + "/" +
					       completeBaseName + ".png");
    } else if(output.type == "screenshot") {
      if(canvas.convertToFormat(QImage::Format_ARGB4444_Premultiplied).save(config.screenshotsFolder + "/" + completeBaseName + ".png"))
	game.screenshotFile = StrTools::xmlUnescape(config.screenshotsFolder + "/" +
						    completeBaseName + ".png");
    } else if(output.type == "wheel") {
      if(canvas.convertToFormat(QImage::Format_ARGB4444_Premultiplied).save(config.wheelsFolder + "/" + completeBaseName + ".png"))
	game.wheelFile = StrTools::xmlUnescape(config.wheelsFolder + "/" +
					       completeBaseName + ".png");
    } else if(output.type == "marquee") {
      if(canvas.convertToFormat(QImage::Format_ARGB4444_Premultiplied).save(config.marqueesFolder + "/" + completeBaseName + ".png"))
	game.marqueeFile = StrTools::xmlUnescape(config.marqueesFolder + "/" +
						 completeBaseName + ".png");
    }
  }
}

QVector<double> Compositor::boxesForGauss(double sigma, double n)
{
  double wIdeal = sqrt((12.0 * sigma * sigma / n) + 1.0);
  double wl = floor(wIdeal);
  if((int)wl % 2 == 0) {
    wl--;
  }
  double wu = wl + 2;
  double mIdeal = (12.0 * sigma * sigma - n * wl * wl - 4.0 * n * wl - 3.0 * n) / (-4.0 * wl - 4.0);
  int m = round(mIdeal);
  // var sigmaActual = Math.sqrt( (m*wl*wl + (n-m)*wu*wu - n)/12 );
  
  QVector<double> sizes;
  for(int i = 0; i < n; i++) {
    sizes.append(i < m?wl:wu);
    printf("BOX: '%f'\n", sizes.last());
  }
  return sizes;
}

void Compositor::boxBlur(QRgb *src, QRgb *dst, int w, int h, int r)
{
  for(int i = 0; i < w * h; i++) {
    dst[i] = src[i];
  }
  printf("RADIUS: '%d'\n", r);
  boxBlurHorizontal(dst, src, w, h, r);
  //boxBlurTotal(src, dst, w, h, r);
}

/*
  bool doneI = false;
  for(int a = 0; a < w * h; ++a) {
    if(!doneI) {
      printf("I: '%d'\n", i);
      doneI = true;
    }
    if(qAlpha(tcl[a]) != 0) {
      printf("ALPHA: '%d'\n", qAlpha(tcl[a]));
    }
  }
*/
void Compositor::boxBlurHorizontal(QRgb *src, QRgb *dst, int width, int height, int radius)
{
  int span = radius + radius + 1;
  for(int y = 0; y < height; y++) {
    int currentIdx = y * width, frontIdx = currentIdx, backIdx = currentIdx + radius;
    int firstVal = qAlpha(src[currentIdx]), lastVal = qAlpha(src[currentIdx + width - 1]);

    // Initial 'value' fill at leftmost edge of line
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

void Compositor::boxBlurTotal(QRgb *src, QRgb *dst, int w, int h, double r)
{
  double iarr = 1.0 / (r + r + 1);
  for(int i = 0; i < w; i++) {
    int ti = i;
    int li = ti;
    int ri = ti + r * w;
    int fv = qAlpha(src[ti]);
    int lv = qAlpha(src[ti + w * (h - 1)]);
    int val = (r + 1) * fv;
    for(int j = 0; j < r; j++) {
      val += qAlpha(src[ti + j * w]);
    }
    for(int j = 0; j <= r ; j++) {
      val += qAlpha(src[ri] - fv);
      dst[ti] = QColor(0, 0, 0, round(val * iarr)).rgba();
      ri += w;
      ti += w;
    }
    for(int j = r + 1; j < h - r; j++) {
      val += qAlpha(src[ri]) - qAlpha(src[li]);
      dst[ti] = QColor(0, 0, 0, round(val * iarr)).rgba();
      li += w;
      ri += w;
      ti += w;
    }
    for(int j = h - r; j < h; j++) {
      val += lv - qAlpha(src[li]);
      dst[ti] = QColor(0, 0, 0, round(val * iarr)).rgba();
      li += w;
      ti += w;
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

  QImage buffer1(image.width() + softness * 2, image.height() + softness * 2,
		      QImage::Format_ARGB32_Premultiplied);
  buffer1.fill(Qt::transparent);
  
  QPainter painter;
  painter.begin(&buffer1);
  painter.drawImage(softness, softness, image);
  painter.end();
  
  QRgb *buffer1Bits = (QRgb *)buffer1.constBits();
  for(int a = 0; a < buffer1.width() * buffer1.height(); ++a) {
    buffer1Bits[a] = QColor(0, 0, 0, qAlpha(buffer1Bits[a])).rgba();
  }
  QImage buffer2 = buffer1;
  QRgb *buffer2Bits = (QRgb *)buffer2.constBits();
  
  int w = buffer1.width();
  int h = buffer1.height();
  
  QVector<double> boxes = boxesForGauss((double)softness, 3.0);

  boxBlur(buffer1Bits, buffer2Bits, w, h, (boxes[0] - 1) / 2);
  buffer1.save("buffer1.png");
  buffer2.save("buffer2.png");
  //boxBlur(buffer2Bits, buffer1Bits, w, h, (boxes[1] - 1) / 2);
  //boxBlur(buffer1Bits, buffer2Bits, w, h, (boxes[2] - 1) / 2);

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

/*
QImage Compositor::applyShadow(QImage &image, int distance, int softness, int opacity)
{
  if(distance == -1)
    distance = 0;
  if(softness == -1)
    softness = 5;
  if(opacity == -1)
    opacity = 50;

  int kernelWidth = softness * 2 + 1;

  QImage shadow(image.width() + softness * 2, image.height() + softness * 2,
		QImage::Format_ARGB32_Premultiplied);
  
  shadow.fill(Qt::black);

  double kernel[kernelWidth][kernelWidth];

  // ----- Kernel matrix creation begin -----

  double r, s = (double)softness;
  
  // sum is for normalization
  double kernelSum = 0.0;
  
  if(softness != 0) {
    // generate kernel
    for (int x = -softness; x <= softness; x++) {
      for(int y = -softness; y <= softness; y++) {
	r = sqrt(x * x + y * y);
	kernel[x + softness][y + softness] = (exp(-(r * r) / s)) / (3.1415927 * s);
	kernelSum += kernel[x + softness][y + softness];
      }
    }
  } else {
    kernel[0][0] = 1.0;
    kernelSum = 1.0;
  }
  
  // ----- Kernel creation end -----
  
  QRgb *imageBits = (QRgb *)shadow.constBits();
  
  for(int y = 0; y < shadow.height(); ++y) {
    for(int x = 0; x < shadow.width(); ++x) {
      double alpha = 0.0;
      for(int i = 0; i < kernelWidth; ++i) {
	for(int j = 0; j < kernelWidth; ++j) {
	  alpha += kernel[i][j] * qAlpha(image.pixel(x - softness + i - kernelWidth / 2,
						     y - softness + j - kernelWidth / 2));
	}
      }
      alpha /= kernelSum;
      *imageBits = QColor(0, 0, 0, alpha).rgba();
      imageBits++; // Advance to next pixel
    }
  }

  QImage shadowImage(image.width() + distance + softness,
		     image.height() + distance + softness,
		     QImage::Format_ARGB32_Premultiplied);
  shadowImage.fill(Qt::transparent);
  QPainter painter;
  painter.begin(&shadowImage);
  painter.setOpacity(opacity * 0.01);
  painter.drawImage(distance - softness, distance - softness, shadow);
  painter.setOpacity(1.0);
  painter.drawImage(0, 0, image);
  painter.end();
  
  return shadowImage;
}
*/
