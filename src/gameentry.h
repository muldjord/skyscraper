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

constexpr int DESCRIPTION = 0;
constexpr int DEVELOPER = 1;
constexpr int PUBLISHER = 2;
constexpr int PLAYERS = 3;
constexpr int TAGS = 4;
constexpr int RELEASEDATE = 5;
constexpr int COVER = 6;
constexpr int SCREENSHOT = 7;
constexpr int VIDEO = 8;
constexpr int RATING = 9;
constexpr int WHEEL = 10;
constexpr int MARQUEE = 11;
constexpr int AGES = 12;
constexpr int TITLE = 13;

#include <QImage>

class GameEntry
{
public:
  GameEntry();
  void calculateCompleteness(bool videoEnabled = false);
  int getCompleteness() const;
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

  QByteArray coverData = QByteArray();
  QString coverFile = "";
  QString coverSrc = "";
  QByteArray screenshotData = QByteArray();
  QString screenshotFile = "";
  QString screenshotSrc = "";
  QByteArray wheelData = QByteArray();
  QString wheelFile = "";
  QString wheelSrc = "";
  QByteArray marqueeData = QByteArray();
  QString marqueeFile = "";
  QString marqueeSrc = "";
  QByteArray videoData = "";
  QString videoFile = "";
  QString videoSrc = "";

  int searchMatch = 0;
  QString cacheId = "";
  QString source = "";
  QString url = "";
  QString sqrNotes = "";
  QString parNotes = "";
  QString videoFormat = "";
  QString baseName = "";
  QString absoluteFilePath = "";
  bool found = true;

  QByteArray miscData = "";

  // EmulationStation specific metadata for preservation
  QString eSFavorite = "";
  QString eSHidden = "";
  QString eSPlayCount = "";
  QString eSLastPlayed = "";
  QString eSKidGame = "";
  QString eSSortName = "";

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

  // Pegasus specific metadata for preservation
  QList<QPair<QString, QString> > pSValuePairs;

private:
  double completeness = 0;
  
};

#endif // GAMEENTRY_H
