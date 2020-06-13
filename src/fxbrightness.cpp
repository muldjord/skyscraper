/***************************************************************************
 *            fxbrightness.cpp
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

#include "fxbrightness.h"

FxBrightness::FxBrightness()
{
}

QImage FxBrightness::applyEffect(const QImage &src, const Layer &layer)
{
  QImage canvas = src;

  int brightness = layer.delta;
  int index[256];
  for(int a = 0; a < 256; ++a) {
    index[a] = truncate(a + brightness);
  }

  for(int y = 0; y < canvas.height(); ++y) {
    QRgb* line = (QRgb *)canvas.scanLine(y);
    for(int x = 0; x < canvas.width(); ++x) {
      
      line[x] = qPremultiply(qRgba(index[qRed(line[x])],
				   index[qGreen(line[x])],
				   index[qBlue(line[x])],
				   qAlpha(line[x])));
    }
  }

  return canvas;
}

int FxBrightness::truncate(int value)
{
  if(value > 255) {
    value = 255;
  }
  if(value < 0) {
    value = 0;
  }
  return value;
}
