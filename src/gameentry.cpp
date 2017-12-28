/***************************************************************************
 *            gameentry.cpp
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

#include "gameentry.h"

GameEntry::GameEntry()
{
}

int GameEntry::completeness(bool videoEnabled)
{
  double completeness = 100.0;
  int noOfTypes = 13;
  if(videoEnabled) {
    noOfTypes += 1;
  }
  double valuePerType = completeness / (double)noOfTypes;
  if(title.isEmpty()) {
    completeness -= valuePerType;
  }
  if(platform.isEmpty()) {
    completeness -= valuePerType;
  }
  if(coverData.isNull()) {
    completeness -= valuePerType;
  }
  if(screenshotData.isNull()) {
    completeness -= valuePerType;
  }
  if(wheelData.isNull()) {
    completeness -= valuePerType;
  }
  if(marqueeData.isNull()) {
    completeness -= valuePerType;
  }
  if(description.isEmpty()) {
    completeness -= valuePerType;
  }
  if(releaseDate.isEmpty()) {
    completeness -= valuePerType;
  }
  if(developer.isEmpty()) {
    completeness -= valuePerType;
  }
  if(publisher.isEmpty()) {
    completeness -= valuePerType;
  }
  if(tags.isEmpty()) {
    completeness -= valuePerType;
  }
  if(rating.isEmpty()) {
    completeness -= valuePerType;
  }
  if(players.isEmpty()) {
    completeness -= valuePerType;
  }
  if(videoEnabled && videoFormat.isEmpty()) {
    completeness -= valuePerType;
  }

  return (int)completeness;
}
