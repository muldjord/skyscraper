/***************************************************************************
 *            compositor.cpp
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

#include <QSettings>
#include <QPainter>

#include "compositor.h"

Compositor::Compositor()
{
}

QImage Compositor::composite(QImage cover, QImage screenshot, Settings config)
{
  if(config.coverYSet == false) {
    config.coverY = config.finalImageHeight - config.coverHeight -
      (config.coverShadowEnabled?config.coverShadowDistance:0);
  }
  if(config.screenshotXSet == false) {
    config.screenshotX = config.finalImageWidth - config.screenshotWidth -
      (config.screenshotShadowEnabled?config.screenshotShadowDistance:0);
  }

  if(cover.isNull()) {
    config.coverEnabled = false;
    cover = QImage(config.coverWidth, config.coverHeight, QImage::Format_ARGB32);
    cover.fill(Qt::transparent);
  }
  if(cover.width() > cover.height()) {
    cover = cover.scaled(config.coverWidth, config.coverHeight,
			 Qt::KeepAspectRatioByExpanding,
			 Qt::SmoothTransformation);
  } else {
    cover = cover.scaled(config.coverWidth, config.coverHeight,
			 Qt::IgnoreAspectRatio,
			 Qt::SmoothTransformation);
  }
  if(config.coverShadowEnabled) {
    applyShadow(cover, config.coverShadowDistance, config.coverShadowSoftness,
		config.coverShadowOpacity);
  }

  if(screenshot.isNull()) {
    config.screenshotEnabled = false;
    screenshot = QImage(config.screenshotWidth, config.screenshotHeight, QImage::Format_ARGB32);
    screenshot.fill(Qt::transparent);
  }
  screenshot = screenshot.scaled(config.screenshotWidth, config.screenshotHeight,
				 Qt::IgnoreAspectRatio,
				 Qt::SmoothTransformation);

  if(config.screenshotShadowEnabled) {
    applyShadow(screenshot, config.screenshotShadowDistance,
		config.screenshotShadowSoftness, config.screenshotShadowOpacity);
  }

  QImage finalImage(config.finalImageWidth, config.finalImageHeight, QImage::Format_ARGB32);
  finalImage.fill(Qt::transparent);

  QPainter painter(&finalImage);

  if(config.screenshotEnabled) {
    painter.drawImage(config.screenshotX, config.screenshotY, screenshot);
  }
  if(config.coverEnabled) {
    painter.drawImage(config.coverX, config.coverY, cover);
  }
  
  return finalImage;
}

void Compositor::applyShadow(QImage &image, int distance, int softness, int opacity)
{
  QPainter painter;
  
  QImage shadow(image.width(), image.height(), QImage::Format_ARGB32);
  
  shadow.fill(Qt::transparent);
  painter.begin(&shadow);
  painter.setBrush(QColor(0, 0, 0, 255 / softness));
  painter.setPen(QColor(0, 0, 0, 0));
  for(qreal a = softness; a >= 0.0; --a) {
    painter.drawRoundedRect(softness - a, softness - a,
			    shadow.width() + ((a - softness) * 2),
			    image.height() + ((a - softness) * 2),
			    a, a);
  }
  painter.end();
  
  QImage shadowImage(image.width() + distance,
		     image.height() + distance,
		     QImage::Format_ARGB32);
  shadowImage.fill(Qt::transparent);
  painter.begin(&shadowImage);
  painter.setOpacity(opacity * 0.01);
  painter.drawImage(distance, distance, shadow);
  painter.setOpacity(1.0);
  painter.drawImage(0, 0, image);
  painter.end();
  
  image = shadowImage;
}
