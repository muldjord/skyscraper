/***************************************************************************
 *            fxrotate.cpp
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

#include "fxrotate.h"

FxRotate::FxRotate()
{
}

QImage FxRotate::applyEffect(const QImage &src, const Layer &layer)
{
  int angle = layer.delta;

  QTransform rotate;
  rotate.rotate(angle, layer.axis);
  if(layer.axis == Qt::YAxis) {
  rotate.translate(0, - src.height() / 2.0);
  } else if(layer.axis == Qt::XAxis) {
    rotate.translate(- src.width() / 2.0, 0);
  }

  QImage canvas = src.transformed(rotate, Qt::SmoothTransformation);
 
  return canvas;
}
