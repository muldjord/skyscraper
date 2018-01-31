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

#include "settings.h"
#include "gameentry.h"

struct Layer {
  QString resource = "";
  QString align = "";
  QString valign = "";
  int x = 0;
  int y = 0;
  int width = -1;
  int height = -1;
  int shadowDistance = -1;
  int shadowSoftness = -1;
  int shadowOpacity = -1;
};

struct Output {
  QString type = "";
  int width = -1;
  int height = -1;
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
  QVector<double> getGaussBoxes(double sigma, double n);
  void boxBlur(QRgb *src, QRgb *dst, int width, int height, int radius);
  void boxBlurHorizontal(QRgb *src, QRgb *dst, int width, int height, int radius);
  void boxBlurTotal(QRgb *src, QRgb *dst, int width, int height, double radius);
  QImage applyShadow(QImage &image, int distance, int softness, int opacity);
  Settings config;
  QList<Output> outputs;
  
};

#endif // COMPOSITOR_H
