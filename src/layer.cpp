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

void Layer::setMode(const QString &mode)
{
  if(mode == "multiply") {
    this->mode = QPainter::CompositionMode_Multiply;
  } else if(mode == "screen") {
    this->mode = QPainter::CompositionMode_Screen;
  } else if(mode == "overlay") {
    this->mode = QPainter::CompositionMode_Overlay;
  } else if(mode == "darken") {
    this->mode = QPainter::CompositionMode_Darken;
  } else if(mode == "lighten") {
    this->mode = QPainter::CompositionMode_Lighten;
  } else if(mode == "colordodge") {
    this->mode = QPainter::CompositionMode_ColorDodge;
  } else if(mode == "colorburn") {
    this->mode = QPainter::CompositionMode_ColorBurn;
  } else if(mode == "hardlight") {
    this->mode = QPainter::CompositionMode_HardLight;
  } else if(mode == "softlight") {
    this->mode = QPainter::CompositionMode_SoftLight;
  } else if(mode == "difference") {
    this->mode = QPainter::CompositionMode_Difference;
  } else if(mode == "exclusion") {
    this->mode = QPainter::CompositionMode_Exclusion;
  } else if(mode == "sourcein") {
    this->mode = QPainter::CompositionMode_SourceIn;
  } else if(mode == "destinationin") {
    this->mode = QPainter::CompositionMode_DestinationIn;
  } else if(mode == "sourceout") {
    this->mode = QPainter::CompositionMode_SourceOut;
  } else if(mode == "destinationout") {
    this->mode = QPainter::CompositionMode_DestinationOut;
  } else if(mode == "sourceatop") {
    this->mode = QPainter::CompositionMode_SourceAtop;
  } else if(mode == "destinationatop") {
    this->mode = QPainter::CompositionMode_DestinationAtop;
  } else if(mode == "xor") {
    this->mode = QPainter::CompositionMode_Xor;
  }
}

void Layer::setAxis(const QString &axis)
{
  if(axis == "x") {
    this->axis = Qt::XAxis;
  } else if(axis == "y") {
    this->axis = Qt::YAxis;
  } else if(axis == "z") {
    this->axis = Qt::ZAxis;
  }
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

void Layer::setScaling(const QString &scaling)
{
  this->scaling = scaling;
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

void Layer::setValue(const int &value)
{
  this->value = value;
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
  if(opacity > 100)
    this->opacity = 100;
  if(opacity < 0)
    this->opacity = 0;
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

void Layer::colorFromHex(QString color)
{
  color = color.replace("#", "");
  
  if(color.length() != 6)
    return;

  red = color.left(2).toInt(Q_NULLPTR, 16);
  green = color.mid(2, 2).toInt(Q_NULLPTR, 16);
  blue = color.right(2).toInt(Q_NULLPTR, 16);
}
