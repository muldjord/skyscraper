/***************************************************************************
 *            fxmask.cpp
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

#include "fxmask.h"

FxMask::FxMask()
{
}

QImage FxMask::applyEffect(QImage &image, Layer &layer, Settings *config)
{
  QImage mask(config->resources[layer.resource]);
  mask = mask.convertToFormat(QImage::Format_ARGB32_Premultiplied);

  if(layer.width == -1 && layer.height == -1) {
    mask = mask.scaled(image.width(), image.height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
  } else if(layer.width == -1 && layer.height != -1) {
    mask = mask.scaledToHeight(layer.height, Qt::SmoothTransformation);
  } else if(layer.width != -1 && layer.height == -1) {
    mask = mask.scaledToWidth(layer.width, Qt::SmoothTransformation);
  } else if(layer.width != -1 && layer.height != -1) {
    mask = mask.scaled(layer.width, layer.height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
  }
  
  QPainter painter;
  painter.begin(&image);
  painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
  painter.drawImage(layer.x, layer.y, mask);
  painter.setCompositionMode(QPainter::CompositionMode_DestinationOut);
  painter.fillRect(0, 0, layer.x, image.height(), QColor(0, 0, 0));
  painter.fillRect(0, 0, image.width(), layer.y, QColor(0, 0, 0));
  painter.fillRect(layer.x + mask.width(), 0,
		   image.width() - layer.x - mask.width(), image.height(), QColor(0, 0, 0));
  painter.fillRect(0, layer.y + mask.height(),
		   image.width(), image.height() - layer.y - mask.height(), QColor(0, 0, 0));
  painter.end();

  return image;
}
