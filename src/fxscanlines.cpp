/***************************************************************************
 *            fxscanlines.cpp
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

#include "fxscanlines.h"

FxScanlines::FxScanlines()
{
}

QImage FxScanlines::applyEffect(const QImage &src, const Layer &layer, Settings *config)
{
  QImage canvas = src;
  QString resource = layer.resource;
  double scaling = 1.0;
  int opacity = layer.opacity;

  if(!layer.scaling.isEmpty()) {
    bool isDouble = false;
    layer.scaling.toDouble(&isDouble);
    if(isDouble)
      scaling = layer.scaling.toDouble();
  }

  if(scaling < 0.1)
    scaling = 0.1;
  if(scaling > 2.0)
    scaling = 2.0;
  /*
  if(!layer.scaling.isEmpty()) {
    if(layer.scaling == "auto") {
    } else {
      bool isDouble = false;
      layer.scaling.toDouble(&isDouble);
      if(isDouble)
	scaling = layer.scaling.toDouble();
    }
  }
  */

  if(resource.isEmpty())
    resource = "scanlines1.png";
  if(opacity == -1)
    opacity = 100;
  
  QPainter painter;
  painter.begin(&canvas);
  painter.setOpacity(opacity * 0.01);
  painter.setCompositionMode(layer.mode);
  painter.drawImage(0, 0, (scaling != 1.0?config->resources[resource].scaledToWidth((int)((double)config->resources[resource].width() * scaling), Qt::FastTransformation):config->resources[resource]));
  painter.end();

  return canvas;
}
