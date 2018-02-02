/***************************************************************************
 *            compositor.h
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

#ifndef COMPOSITOR_H
#define COMPOSITOR_H

#include <QImage>
#include <QXmlStreamReader>

#include "settings.h"
#include "gameentry.h"

#define T_NONE 0
#define T_OUTPUT 1
#define T_LAYER 2
#define T_SHADOW 3
#define T_MASK 4
#define T_FRAME 5
#define T_STROKE 6

struct Layer {
  int type = T_NONE;
  QString resource = "";
  QString align = "";
  QString valign = "";
  int x = 0;
  int y = 0;
  int width = -1;
  int height = -1;
  int shadowDistance = 0;
  int shadowSoftness = 0;
  int shadowOpacity = 0;
  int strokeWidth = 0;
  QList<Layer> layers;
};

class Compositor : public QObject
{
  Q_OBJECT

public:
  Compositor(Settings &config);
  bool processXml(QString filename);
  void saveAll(GameEntry &game, QString completeBaseName);

private:
  void addLayer(Layer &layer, QXmlStreamReader &xml);
  void compositeLayer(GameEntry &game, QImage &canvas, Layer &layer);
  QImage applyMask(QImage &image, Layer &layer);
  QImage applyFrame(QImage &image, Layer &layer);
  QVector<double> getGaussBoxes(double sigma, double n);
  void boxBlur(QRgb *src, QRgb *dst, int width, int height, int radius);
  void boxBlurHorizontal(QRgb *src, QRgb *dst, int width, int height, int radius);
  void boxBlurTotal(QRgb *src, QRgb *dst, int width, int height, double radius);
  QImage applyShadow(QImage &image, Layer &layer);
  Settings config;
  Layer outputs;
  
};

#endif // COMPOSITOR_H
