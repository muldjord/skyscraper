/***************************************************************************
 *            fxframe.cpp
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

#include "fxframe.h"

FxFrame::FxFrame()
{
}

QImage FxFrame::applyEffect(const QImage &src, const Layer &layer, Settings *config)
{
  QImage canvas = src;

  QImage frame(config->resources[layer.resource]);
  frame = frame.convertToFormat(QImage::Format_ARGB32_Premultiplied);

  if(layer.width == -1 && layer.height == -1) {
    frame = frame.scaled(src.width(), src.height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
  } else if(layer.width == -1 && layer.height != -1) {
    frame = frame.scaledToHeight(layer.height, Qt::SmoothTransformation);
  } else if(layer.width != -1 && layer.height == -1) {
    frame = frame.scaledToWidth(layer.width, Qt::SmoothTransformation);
  } else if(layer.width != -1 && layer.height != -1) {
    frame = frame.scaled(layer.width, layer.height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
  }
  
  QPainter painter;
  painter.begin(&canvas);
  painter.drawImage(0, 0, frame);
  painter.end();

  return canvas;
}
