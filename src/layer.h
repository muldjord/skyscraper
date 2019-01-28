/***************************************************************************
 *            layer.h
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

#ifndef LAYER_H
#define LAYER_H

#define T_NONE 0
#define T_OUTPUT 1
#define T_LAYER 2
#define T_SHADOW 3
#define T_MASK 4
#define T_FRAME 5
#define T_STROKE 6
#define T_ROUNDED 7
#define T_BRIGHTNESS 8
#define T_CONTRAST 9
#define T_BALANCE 10
#define T_OPACITY 11
#define T_BLUR 12
#define T_GAMEBOX 13
#define T_HUE 14
#define T_SATURATION 15
#define T_COLORIZE 16
#define T_ROTATE 17
#define T_SCANLINES 18
#define T_SHARPEN 19

#include <QImage>
#include <QPainter>

class Layer
{
public:
  Layer();

  int type = T_NONE;
  QImage canvas = QImage();
  QString resType = "";
  QString resource = "";
  QString align = "";
  QString valign = "";
  QString scaling = "";
  int x = 0;
  int y = 0;
  int width = -1;
  int height = -1;
  double mPixels = -1.0;
  int value = -1; // Use this for a value that can only be positive
  int delta = 0; // Use this for a value that can both be positive and negative
  int red = -1;
  int green = -1;
  int blue = -1;
  int distance = -1;
  int softness = -1;
  int opacity = -1;
  QPainter::CompositionMode mode = QPainter::CompositionMode_SourceOver;
  Qt::Axis axis = Qt::ZAxis;
  int saturation = 127;
  
  // Setters
  void setMode(const QString &mode);
  void setAxis(const QString &axis);
  void setType(const int &type);
  void setResType(const QString &resType);
  void setCanvas(const QImage &canvas);
  void setResource(const QString &resource);
  void setAlign(const QString &align);
  void setVAlign(const QString &valign);
  void setScaling(const QString &value);
  void setX(const int &x);
  void setY(const int &y);
  void setWidth(const int &width);
  void setHeight(const int &height);
  void setMPixels(const double &mPixels);
  void setDelta(const int &delta);
  void setValue(const int &value);
  void setRed(const int &red);
  void setGreen(const int &green);
  void setBlue(const int &blue);
  void setDistance(const int &distance);
  void setSoftness(const int &softness);
  void setOpacity(const int &opacity);

  void addLayer(const Layer &layer);
  QList<Layer> getLayers();

  void makeTransparent();
  void scale();
  void premultiply();
  void updateSize();
  bool hasLayers();
  bool save(QString filename);

  void colorFromHex(QString color);

private:
  // Contains all nested layers and effects
  QList<Layer> layers;
};

#endif // LAYER_H
