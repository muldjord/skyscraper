/***************************************************************************
 *            fxsharpen.cpp
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

#include "fxsharpen.h"

FxSharpen::FxSharpen()
{
}

QImage FxSharpen::applyEffect(const QImage &src, const Layer &layer)
{
  QImage canvas = src;

  int strength = layer.value;
  if(strength == -1)
    strength = 100;

  double kern[] = {0, -1, 0,
		   -1, 5, -1,
		   0, -1, 0};

  /*
  int hue = layer.value;
  int satDelta = layer.delta;

  if(hue > 359 || hue < 0) {
    return canvas;
  }

  if(satDelta > 127 || satDelta < -127) {
    satDelta = 0;
  }
  int saturation = 127 + satDelta;
  */

  for(int y = 0; y < canvas.height(); ++y) {
    QRgb* line = (QRgb *)canvas.scanLine(y);
    for(int x = 0; x < canvas.width(); ++x) {
        QColor color(line[x]);
	color.setRgb(qRed(line[x]), qGreen(line[x]), qBlue(line[x]));
	line[x] = qPremultiply(color.rgba());
    }
  }
  
  return canvas;
}
