/***************************************************************************
 *            fxgamebox.cpp
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
#include <QTransform>

#include "fxgamebox.h"

FxGamebox::FxGamebox()
{
}

QImage FxGamebox::applyEffect(QImage &image, Layer &layer, GameEntry &game, Settings *config)
{
  QPainter painter;
  QTransform trans;

  QImage front = image;
  QImage overlayFront(config->resources["boxfront.png"]);
  overlayFront = overlayFront.scaled(front.width(), front.height(),
				     Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

  painter.begin(&front);
  painter.drawImage(0, 0, overlayFront);
  painter.end();

  QImage overlaySide(config->resources["boxside.png"]);
  overlaySide = overlaySide.scaledToHeight(front.height(), Qt::SmoothTransformation);

  QImage side(overlaySide.width(), overlaySide.height(), QImage::Format_ARGB32_Premultiplied);
  QImage sideImage;

  double avgRed = 0;
  double avgGreen = 0;
  double avgBlue = 0;
  double fromTop = 20;
  if(image.height() < 20) {
    fromTop = image.height();
  }
  for(int y = 0; y < fromTop; ++y) {
    QRgb *scanline = (QRgb *)image.scanLine(y);
    avgRed += qRed(scanline[3]);
    avgGreen += qGreen(scanline[3]);
    avgBlue += qBlue(scanline[3]);
  }
  avgRed /= fromTop;
  avgGreen /= fromTop;
  avgBlue /= fromTop;
  
  side.fill(QColor(avgRed, avgGreen, avgBlue));

  if(layer.resource == "cover") {
    sideImage = game.coverData;
  } else if(layer.resource == "screenshot") {
    sideImage = game.screenshotData;
  } else if(layer.resource == "wheel") {
    sideImage = game.wheelData;
  } else if(layer.resource == "marquee") {
    sideImage = game.marqueeData;
  } else {
    sideImage = QImage(config->resources[layer.resource]);
  }
  sideImage = sideImage.convertToFormat(QImage::Format_ARGB32_Premultiplied);
  
  trans.reset();
  trans.rotate(layer.delta, Qt::ZAxis);
  sideImage = sideImage.transformed(trans, Qt::SmoothTransformation);
  sideImage = sideImage.scaledToWidth(side.width(), Qt::SmoothTransformation);
  
  painter.begin(&side);
  painter.drawImage(0, side.height() / 2 - sideImage.height() / 2, sideImage);
  painter.drawImage(0, 0, overlaySide);
  painter.end();
  
  // At this point both side and front are done and ready to be rotated in 3D
  trans.reset();
  trans.rotate(-30, Qt::YAxis);
  trans.translate(0, - front.height() / 2.0);
  front = front.transformed(trans, Qt::SmoothTransformation);

  trans.reset();
  trans.rotate(70, Qt::YAxis);
  trans.translate(0, - side.height() / 2.0);
  side = side.transformed(trans, Qt::SmoothTransformation);
  side = side.scaledToHeight(front.height(), Qt::SmoothTransformation);

  image.fill(Qt::transparent);
  painter.begin(&image);
  painter.drawImage(0, 0, side);
  painter.drawImage(side.width() - 1, 0, front);
  
  return image;
}
