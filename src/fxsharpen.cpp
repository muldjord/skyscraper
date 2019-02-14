/***************************************************************************
 *            fxsharpen.cpp
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

#include "fxsharpen.h"

FxSharpen::FxSharpen()
{
}

QImage FxSharpen::applyEffect(const QImage &src, const Layer &)
{
  QImage buffer1 = src;
  /*
  double strength = 5.0;
  if(layer.value >= 0 && layer.value <= 100) {
    strength = layer.value * 0.1;
  }

  double kernel[] = {0.0, -strength, 0.0,
		     -strength, 1.0 + 4.0 * strength, -strength,
		     0.0, -strength, 0.0};

  QRgb *buffer1Bits = (QRgb *)buffer1.bits();

  QImage buffer2(buffer1.width(), buffer1.height(), QImage::Format_ARGB32_Premultiplied);
  QRgb *buffer2Bits = (QRgb *)buffer2.bits();
  
  int width = buffer1.width(), height = buffer1.height();
  
  for(int y = 1; y < height - 1; ++y) {
    for(int x = 1; x < width - 1; ++x) {
      for(int a = 0; a < 9; ++a) {
	
      }
    }
  }
  */  
  return buffer1;
}

/*
 for(int i=1;i<b.getHeight()-1;i++) {
       for(int j=1;j<b.getWidth()-1;j++) {
          double linc_r=0, linc_g=0,linc_b=0;
          for(int k=0; k<=2; k++) {
             for(int l=0; l<=2; l++) {
                linc_r +=( (b.getPixel(j+l-1,i+k-1)).getRed()  * mask[
k*3 + l] );
                linc_g +=( (b.getPixel(j+l-1,i+k-1)).getGreen()* mask[
k*3 + l] );
                linc_b +=( (b.getPixel(j+l-1,i+k-1)).getBlue() * mask[
k*3 + l] );
             }
          }
          //Check ranges etc
          ....

          R = (byte)round(linc_r);
          G = (byte)round(linc_g);
          B = (byte)round(linc_b);

          result.drawPixel(Pixel(R,G,B), j,i);
       }
    }
    }
*/
