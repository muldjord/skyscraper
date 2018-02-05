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

struct Layer {
  int type = T_NONE;
  QString resource = "";
  QString align = "";
  QString valign = "";
  int x = 0;
  int y = 0;
  int width = -1;
  int height = -1;
  int delta = 0;
  int red = -1;
  int green = -1;
  int blue = -1;
  int distance = 0;
  int softness = 0;
  int opacity = 0;
  QList<Layer> layers;
};

#endif // LAYER_H
