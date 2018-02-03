/***************************************************************************
 *            fxstroke.cpp
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

#include "fxstroke.h"

FxStroke::FxStroke()
{
}

QImage FxStroke::applyEffect(QImage &image, Layer &layer)
{
  QImage buffer1(image.width() + layer.width * 2, image.height() + layer.width * 2,
		 QImage::Format_ARGB32_Premultiplied);
  buffer1.fill(Qt::transparent);
  QPainter painter;
  painter.begin(&buffer1);
  painter.drawImage(layer.width, layer.width, image);
  painter.end();
  
  QImage buffer2(image.width() + layer.width * 2, image.height() + layer.width * 2,
		 QImage::Format_ARGB32_Premultiplied);
  buffer2.fill(Qt::transparent);

  QRgb *buffer1Bits = (QRgb *)buffer1.bits();
  QRgb *buffer2Bits = (QRgb *)buffer2.bits();

  int width = buffer1.width();
  int height = buffer1.height();
  
  // Now dilate each pixel layer.width number of times
  for(int a = 0; a < layer.width; ++a) {
    for(int y = 0; y < height; ++y) {
      for(int x = 0; x < width; ++x) {
	int alpha = qAlpha(buffer1Bits[y * width + x]);
	if(alpha != 0) {
	  for(int i = -1; i <= 1; ++i) {
	    for(int j = -1; j <= 1; ++j) {
	      if(qAlpha(buffer2Bits[(y + i) * width + x + j]) < alpha) {
		buffer2Bits[(y + i) * width + x + j] =
		  qPremultiply(qRgba(layer.red, layer.green, layer.blue, alpha));
	      }
	    }
	  }
	}
      }
    }
    for(int y = 0; y < height; ++y) {
      for(int x = 0; x < width; ++x) {
	buffer1Bits[y * width + x] = buffer2Bits[y * width + x];
      }
    }
  }

  painter.begin(&buffer2);
  painter.drawImage(layer.width, layer.width, image);
  painter.end();

  return buffer2;
}
