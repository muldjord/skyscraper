/***************************************************************************
 *            fxblur.cpp
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

#include "fxblur.h"

FxBlur::FxBlur()
{
}

QImage FxBlur::applyEffect(const QImage &src, const Layer &layer)
{
  int softness = layer.softness;

  if(softness == -1)
    softness = 3;

  QImage buffer1 = src;
  QRgb *buffer1Bits = (QRgb *)buffer1.bits();

  QImage buffer2(buffer1.width(), buffer1.height(), QImage::Format_ARGB32_Premultiplied);
  QRgb *buffer2Bits = (QRgb *)buffer2.bits();

  int width = buffer1.width(), height = buffer1.height();

  boxBlur(buffer1Bits, buffer2Bits, width, height, softness);

  return buffer2;
}

void FxBlur::boxBlur(QRgb *buffer1, QRgb *buffer2, int width, int height, int radius)
{
  for(int i = 0; i < width * height; i++) {
    buffer2[i] = buffer1[i];
  }
  boxBlurHorizontal(buffer2, buffer1, width, height, radius);
  boxBlurTotal(buffer1, buffer2, width, height, radius);
}

void FxBlur::boxBlurHorizontal(QRgb *buffer1, QRgb *buffer2, int width, int height, int radius)
{
  int span = radius + radius + 1;
  for(int y = 0; y < height; y++) {
    int currentIdx = y * width, frontIdx = currentIdx, backIdx = currentIdx + radius;
    int firstValR = qRed(buffer1[currentIdx]),
      lastValR = qRed(buffer1[currentIdx + width - 1]);
    int firstValG = qGreen(buffer1[currentIdx]),
      lastValG = qGreen(buffer1[currentIdx + width - 1]);
    int firstValB = qBlue(buffer1[currentIdx]),
      lastValB = qBlue(buffer1[currentIdx + width - 1]);
    int firstValA = qAlpha(buffer1[currentIdx]),
      lastValA = qAlpha(buffer1[currentIdx + width - 1]);

    // Initial 'value' fill at leftmost edge
    int valueR = (radius + 1) * firstValR;
    int valueG = (radius + 1) * firstValG;
    int valueB = (radius + 1) * firstValB;
    int valueA = (radius + 1) * firstValA;
    for(int x = 0; x < radius; x++) {
      valueR += qRed(buffer1[currentIdx + x]);
      valueG += qGreen(buffer1[currentIdx + x]);
      valueB += qBlue(buffer1[currentIdx + x]);
      valueA += qAlpha(buffer1[currentIdx + x]);
    }
    
    for(int x = 0; x <= radius ; x++) {
      valueR += qRed(buffer1[backIdx]) - firstValR;
      valueG += qGreen(buffer1[backIdx]) - firstValG;
      valueB += qBlue(buffer1[backIdx]) - firstValB;
      valueA += qAlpha(buffer1[backIdx]) - firstValA;
      buffer2[currentIdx++] = qPremultiply(qRgba(valueR / span, valueG / span,
						 valueB / span, valueA / span));
      backIdx++;
    }
    for(int x = radius + 1; x < width - radius; x++) {
      valueR += qRed(buffer1[backIdx]) - qRed(buffer1[frontIdx]);
      valueG += qGreen(buffer1[backIdx]) - qGreen(buffer1[frontIdx]);
      valueB += qBlue(buffer1[backIdx]) - qBlue(buffer1[frontIdx]);
      valueA += qAlpha(buffer1[backIdx]) - qAlpha(buffer1[frontIdx]);
      buffer2[currentIdx++] = qPremultiply(qRgba(valueR / span, valueG / span,
						 valueB / span, valueA / span));
      frontIdx++;
      backIdx++;
    }
    for(int x = width- radius; x < width; x++) {
      valueR += lastValR - qRed(buffer1[frontIdx]);
      valueG += lastValG - qGreen(buffer1[frontIdx]);
      valueB += lastValB - qBlue(buffer1[frontIdx]);
      valueA += lastValA - qAlpha(buffer1[frontIdx]);
      buffer2[currentIdx++] = qPremultiply(qRgba(valueR / span, valueG / span,
						 valueB / span, valueA / span));
      frontIdx++;
    }
  }
}

void FxBlur::boxBlurTotal(QRgb *buffer1, QRgb *buffer2, int width, int height, int radius)
{
  int span = radius + radius + 1;
  for(int x = 0; x < width; x++) {
    int currentIdx = x, frontIdx = currentIdx, backIdx = currentIdx + radius * width;
    int firstValR = qRed(buffer1[currentIdx]), lastValR = qRed(buffer1[currentIdx + width * (height - 1)]);
    int firstValG = qGreen(buffer1[currentIdx]), lastValG = qGreen(buffer1[currentIdx + width * (height - 1)]);
    int firstValB = qBlue(buffer1[currentIdx]), lastValB = qBlue(buffer1[currentIdx + width * (height - 1)]);
    int firstValA = qAlpha(buffer1[currentIdx]), lastValA = qAlpha(buffer1[currentIdx + width * (height - 1)]);

    // Initial 'value' fill at topmost edge
    int valueR = (radius + 1) * firstValR;
    int valueG = (radius + 1) * firstValG;
    int valueB = (radius + 1) * firstValB;
    int valueA = (radius + 1) * firstValA;
    for(int y = 0; y < radius; y++) {
      valueR += qRed(buffer1[currentIdx + y * width]);
      valueG += qGreen(buffer1[currentIdx + y * width]);
      valueB += qBlue(buffer1[currentIdx + y * width]);
      valueA += qAlpha(buffer1[currentIdx + y * width]);
    }
    
    for(int y = 0; y <= radius ; y++) {
      valueR += qRed(buffer1[backIdx]) - firstValR;
      valueG += qGreen(buffer1[backIdx]) - firstValG;
      valueB += qBlue(buffer1[backIdx]) - firstValB;
      valueA += qAlpha(buffer1[backIdx]) - firstValA;
      buffer2[currentIdx] = qPremultiply(qRgba(valueR / span, valueG / span,
					       valueB / span, valueA / span));
      backIdx += width;
      currentIdx += width;
    }
    for(int y = radius + 1; y < height - radius; y++) {
      valueR += qRed(buffer1[backIdx]) - qRed(buffer1[frontIdx]);
      valueG += qGreen(buffer1[backIdx]) - qGreen(buffer1[frontIdx]);
      valueB += qBlue(buffer1[backIdx]) - qBlue(buffer1[frontIdx]);
      valueA += qAlpha(buffer1[backIdx]) - qAlpha(buffer1[frontIdx]);
      buffer2[currentIdx] = qPremultiply(qRgba(valueR / span, valueG / span,
					       valueB / span, valueA / span));
      frontIdx += width;
      backIdx += width;
      currentIdx += width;
    }
    for(int y = height - radius; y < height; y++) {
      valueR += lastValR - qRed(buffer1[frontIdx]);
      valueG += lastValG - qGreen(buffer1[frontIdx]);
      valueB += lastValB - qBlue(buffer1[frontIdx]);
      valueA += lastValA - qAlpha(buffer1[frontIdx]);
      buffer2[currentIdx] = qPremultiply(qRgba(valueR / span, valueG / span,
					       valueB / span, valueA / span));
      frontIdx += width;
      currentIdx += width;
    }
  }
}
