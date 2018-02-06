/***************************************************************************
 *            imgtools.cpp
 *
 *  Tue Feb 6 12:00:00 CEST 2018
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

#include "imgtools.h"

QImage ImgTools::cropToFit(const QImage &image)
{
  int left = image.width();
  int right = 0;
  int top = 0;
  int bottom = image.height();

  // Find lefmost edge
  for(int y = 0; y < image.height(); ++y) {
    QRgb *scanline = (QRgb *)image.scanLine(y);
    for(int x = 0; x < image.width(); ++x) {
      if(qAlpha(scanline[x]) > 0) {
	if(left > x) {
	  left = x;
	}
	if(right < x) {
	  right = x + 1;
	}
	if(top == 0) {
	  top = y;
	}
	bottom = y + 1;
      }
    }
  }
  QImage cropped = image.copy(left, top, right - left, bottom - top);

  return cropped;
}
