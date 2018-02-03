/***************************************************************************
 *            fxcontrast.cpp
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

#include "fxcontrast.h"

FxContrast::FxContrast()
{
}

QImage FxContrast::applyEffect(QImage &image, Layer &layer)
{
  int contrast = layer.delta;

  double factor = (259.0 * ((double)contrast + 255.0)) / (255.0 * (259.0 - (double)contrast));

  double index[256];
  for(int a = 0; a < 256; ++a) {
    index[a] = truncate(round(factor * a));
  }
  
  for(int y = 0; y < image.height(); ++y) {
    QRgb* line = (QRgb *)image.scanLine(y);
    for(int x = 0; x < image.width(); ++x) {
      
      line[x] = qPremultiply(qRgba(index[qRed(line[x])],
				   index[qGreen(line[x])],
				   index[qBlue(line[x])],
				   qAlpha(line[x])));
    }
  }

  return image;
}

int FxContrast::truncate(int value)
{
  if(value > 255) {
    value = 255;
  }
  if(value < 0) {
    value = 0;
  }
  return value;
}
