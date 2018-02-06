/***************************************************************************
 *            layer.cpp
 *
 *  Wed Jun 18 12:00:00 CEST 2017
 *  Copyright 2017 Lars Muldjord
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

#include "layer.h"

Layer::Layer()
{
}

// Setters
void Layer::setType(const int &type)
{
  this->type = type;
}

void Layer::setCanvas(const QImage &canvas)
{
  this->canvas = canvas;
}

void Layer::setResource(const QString &resource)
{
  this->resource = resource;
}

void Layer::setAlign(const QString &align)
{
  this->align = align;
}

void Layer::setVAlign(const QString &valign)
{
  this->valign = valign;
}

void Layer::setX(const int &x)
{
  this->x = x;
}

void Layer::setY(const int &y)
{
  this->y = y;
}

void Layer::setWidth(const int &width)
{
  this->width = width;
}

void Layer::setHeight(const int &height)
{
  this->height = height;
}

void Layer::setDelta(const int &delta)
{
  this->delta = delta;
}

void Layer::setRed(const int &red)
{
  this->red = red;
}

void Layer::setGreen(const int &green)
{
  this->green = green;
}

void Layer::setBlue(const int &blue)
{
  this->blue = blue;
}

void Layer::setDistance(const int &distance)
{
  this->distance = distance;
}

void Layer::setSoftness(const int &softness)
{
  this->softness = softness;
}

void Layer::setOpacity(const int &opacity)
{
  this->opacity = opacity;
}

// Add new layer
void Layer::addLayer(const Layer &layer)
{
  this->layers.append(layer);
}

QList<Layer> Layer::getLayers()
{
  return layers;
}

void Layer::makeTransparent()
{
  canvas.fill(Qt::transparent);
}

void Layer::scale()
{
  if(width == -1 && height != -1) {
    canvas = canvas.scaledToHeight(height, Qt::SmoothTransformation);
  } else if(width != -1 && height == -1) {
    canvas = canvas.scaledToWidth(width, Qt::SmoothTransformation);
  } else if(width != -1 && height != -1) {
    canvas = canvas.scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
  }
}

void Layer::premultiply()
{
  canvas = canvas.convertToFormat(QImage::Format_ARGB32_Premultiplied);
}

void Layer::updateSize()
{
  width = canvas.width();
  height = canvas.height();
}

bool Layer::hasLayers()
{
  if(layers.isEmpty()) {
    return false;
  }
  return true;
}

bool Layer::save(QString filename)
{
  canvas = canvas.convertToFormat(QImage::Format_ARGB6666_Premultiplied);
  if(canvas.save(filename)) {
    return true;
  }
  return false;
}
