/***************************************************************************
 *            fxshadow.cpp
 *
 *  Sat Feb 3 12:00:00 CEST 2018
 *  Copyright 2018 Lars Muldjord
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
#include <QPainter>

#include "fxshadow.h"

FxShadow::FxShadow()
{
}

QImage FxShadow::applyEffect(QImage &image, Layer &layer)
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

QVector<double> FxShadow::getGaussBoxes(double sigma, double n)
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

void FxShadow::boxBlur(QRgb *src, QRgb *dst, int width, int height, int radius)
{
  for(int i = 0; i < width * height; i++) {
    dst[i] = src[i];
  }
  boxBlurHorizontal(dst, src, width, height, radius);
  boxBlurTotal(src, dst, width, height, radius);
}

void FxShadow::boxBlurHorizontal(QRgb *src, QRgb *dst, int width, int height, int radius)
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

void FxShadow::boxBlurTotal(QRgb *src, QRgb *dst, int width, int height, double radius)
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
