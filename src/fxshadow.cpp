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

QImage FxShadow::applyEffect(const QImage &src, const Layer &layer)
{
  int distance = layer.distance;
  int softness = layer.softness;
  int opacity = layer.opacity;

  if(distance == -1)
    distance = 0;
  if(softness == -1)
    softness = 5;
  if(opacity == -1)
    opacity = 50;

  QImage buffer1(src.width() + softness * 2, src.height() + softness * 2,
		 QImage::Format_ARGB32_Premultiplied);
  buffer1.fill(Qt::transparent);
  QPainter painter;
  painter.begin(&buffer1);
  painter.drawImage(softness, softness, src);
  painter.end();
  
  QRgb *buffer1Bits = (QRgb *)buffer1.bits();
  // Paint everything black but preserve alpha
  for(int a = 0; a < buffer1.width() * buffer1.height(); ++a) {
    buffer1Bits[a] = qPremultiply(qRgba(0, 0, 0, qAlpha(buffer1Bits[a])));
  }

  QImage buffer2(buffer1.width(), buffer1.height(), QImage::Format_ARGB32_Premultiplied);
  QRgb *buffer2Bits = (QRgb *)buffer2.bits();
  
  int width = buffer1.width(), height = buffer1.height();
  
  boxBlur(buffer1Bits, buffer2Bits, width, height, softness);
  
  QImage resultImage(src.width() + distance + softness,
		     src.height() + distance + softness,
		     QImage::Format_ARGB32_Premultiplied);
  resultImage.fill(Qt::transparent);
  painter.begin(&resultImage);
  painter.setOpacity(opacity * 0.01);
  painter.drawImage(distance - softness, distance - softness, buffer2);
  painter.setOpacity(1.0);
  painter.drawImage(0, 0, src);
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

void FxShadow::boxBlur(QRgb *buffer1, QRgb *buffer2, int width, int height, int radius)
{
  for(int i = 0; i < width * height; i++) {
    buffer2[i] = buffer1[i];
  }
  boxBlurHorizontal(buffer2, buffer1, width, height, radius);
  boxBlurTotal(buffer1, buffer2, width, height, radius);
}

void FxShadow::boxBlurHorizontal(QRgb *buffer1, QRgb *buffer2, int width, int height, int radius)
{
  int span = radius + radius + 1;
  for(int y = 0; y < height; y++) {
    int currentIdx = y * width, frontIdx = currentIdx, backIdx = currentIdx + radius;
    int firstVal = qAlpha(buffer1[currentIdx]), lastVal = qAlpha(buffer1[currentIdx + width - 1]);

    // Initial 'value' fill at leftmost edge
    int value = (radius + 1) * firstVal;
    for(int x = 0; x < radius; x++) {
      value += qAlpha(buffer1[currentIdx + x]);
    }
    
    for(int x = 0; x <= radius ; x++) {
      value += qAlpha(buffer1[backIdx++]) - firstVal;
      buffer2[currentIdx++] = qPremultiply(qRgba(0, 0, 0, value / span));
    }
    for(int x = radius + 1; x < width - radius; x++) {
      value += qAlpha(buffer1[backIdx++]) - qAlpha(buffer1[frontIdx++]);
      buffer2[currentIdx++] = qPremultiply(qRgba(0, 0, 0, value / span));
    }
    for(int x = width- radius; x < width; x++) {
      value += lastVal - qAlpha(buffer1[frontIdx++]);
      buffer2[currentIdx++] = qPremultiply(qRgba(0, 0, 0, value / span));
    }
  }
}

void FxShadow::boxBlurTotal(QRgb *buffer1, QRgb *buffer2, int width, int height, int radius)
{
  int span = radius + radius + 1;
  for(int x = 0; x < width; x++) {
    int currentIdx = x, frontIdx = currentIdx, backIdx = currentIdx + radius * width;
    int firstVal = qAlpha(buffer1[currentIdx]),
      lastVal = qAlpha(buffer1[currentIdx + width * (height - 1)]);

    // Initial 'value' fill at topmost edge
    int value = (radius + 1) * firstVal;
    for(int y = 0; y < radius; y++) {
      value += qAlpha(buffer1[currentIdx + y * width]);
    }
    
    for(int y = 0; y <= radius ; y++) {
      value += qAlpha(buffer1[backIdx]) - firstVal;
      buffer2[currentIdx] = qPremultiply(qRgba(0, 0, 0, value / span));
      backIdx += width;
      currentIdx += width;
    }
    for(int y = radius + 1; y < height - radius; y++) {
      value += qAlpha(buffer1[backIdx]) - qAlpha(buffer1[frontIdx]);
      buffer2[currentIdx] = qPremultiply(qRgba(0, 0, 0, value / span));
      frontIdx += width;
      backIdx += width;
      currentIdx += width;
    }
    for(int y = height - radius; y < height; y++) {
      value += lastVal - qAlpha(buffer1[frontIdx]);
      buffer2[currentIdx] = qPremultiply(qRgba(0, 0, 0, value / span));
      frontIdx += width;
      currentIdx += width;
    }
  }
}
