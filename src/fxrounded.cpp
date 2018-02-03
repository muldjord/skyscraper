/***************************************************************************
 *            fxrounded.cpp
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

#include "fxrounded.h"

FxRounded::FxRounded()
{
}

QImage FxRounded::applyEffect(QImage &image, Layer &layer)
{
  QImage mask(image.width(), image.height(), QImage::Format_ARGB32_Premultiplied);
  mask.fill(Qt::transparent);

  QPainter painter;

  painter.begin(&mask);
  painter.setRenderHint(QPainter::Antialiasing);
  QPainterPath path;
  path.addRoundedRect(0, 0, image.width(), image.height(), layer.width, layer.width);
  painter.fillPath(path, Qt::black);
  painter.drawPath(path);
  painter.end();

  painter.begin(&image);
  painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
  painter.drawImage(0, 0, mask);
  painter.end();

  return image;
}
