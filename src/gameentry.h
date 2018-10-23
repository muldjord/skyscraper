/***************************************************************************
 *            gameentry.h
 *
 *  Wed Jun 14 12:00:00 CEST 2017
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

#ifndef GAMEENTRY_H
#define GAMEENTRY_H

#define DESCRIPTION 0
#define DEVELOPER 1
#define PUBLISHER 2
#define PLAYERS 3
#define TAGS 4
#define RELEASEDATE 5
#define COVER 6
#define SCREENSHOT 7
#define VIDEO 8
#define RATING 9
#define WHEEL 10
#define MARQUEE 11
#define AGES 12
#define TITLE 13

#include <QImage>

class GameEntry
{
public:
  GameEntry();
  int completeness(bool videoEnabled = false);
  void resetMedia();
  
  QString id = "";
  QString path = "";
  QString title = "";
  QString titleSrc = "";
  QString platform = "";
  QString platformSrc = "";
  QString description = "";
  QString descriptionSrc = "";
  QString releaseDate = "";
  QString releaseDateSrc = "";
  QString developer = "";
  QString developerSrc = "";
  QString publisher = "";
  QString publisherSrc = "";
  QString tags = "";
  QString tagsSrc = "";
  QString players = "";
  QString playersSrc = "";
  QString ages = "";
  QString agesSrc = "";
  QString rating = "";
  QString ratingSrc = "";

  QImage coverData = QImage();
  QString coverFile = "";
  QString coverSrc = "";
  QImage screenshotData = QImage();
  QString screenshotFile = "";
  QString screenshotSrc = "";
  QImage wheelData = QImage();
  QString wheelFile = "";
  QString wheelSrc = "";
  QImage marqueeData = QImage();
  QString marqueeFile = "";
  QString marqueeSrc = "";
  QByteArray videoData = "";
  QString videoFile = "";
  QString videoSrc = "";

  int searchMatch = 0;
  QString sha1 = "";
  QString source = "";
  QString url = "";
  QString sqrNotes = "";
  QString parNotes = "";
  QString videoFormat = "";
  QString baseName = "";
  bool found = true;

  QByteArray miscData = "";

  // EmulationStation specific metadata for preservation
  QString eSFavorite = "";
  QString eSHidden = "";
  QString eSPlayCount = "";
  QString eSLastPlayed = "";
  QString eSKidGame = "";

  // AttractMode specific metadata for preservation
  // #Name;Title;Emulator;CloneOf;Year;Manufacturer;Category;Players;Rotation;Control;Status;DisplayCount;DisplayType;AltRomname;AltTitle;Extra;Buttons
  QString aMCloneOf = "";
  QString aMRotation = "";
  QString aMControl = "";
  QString aMStatus = "";
  QString aMDisplayCount = "";
  QString aMDisplayType = "";
  QString aMAltRomName = "";
  QString aMAltTitle = "";
  QString aMExtra = "";
  QString aMButtons = "";

};

#endif // GAMEENTRY_H
