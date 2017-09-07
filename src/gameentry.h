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

#include <QImage>

class GameEntry
{
public:
  GameEntry();
  int completeness(bool videoEnabled = false);
  
  // Used in gamelists
  QString path = "";
  QString title = "";
  QString imageFile = "";
  QString videoFile = "";
  QString description = "";
  QString releaseDate = "";
  QString developer = "";
  QString publisher = "";
  QString tags = "";
  QString players = "";
  QString rating = "";

  // Extras needed internally
  int searchMatch = 0;
  QString sha1 = "";
  QString source = "";
  QString platform = "";
  QString url = "";
  QString sqrNotes = "";
  QString parNotes = "";
  QImage coverData = QImage();
  QImage screenshotData = QImage();
  QByteArray videoData = "";
  QString videoFormat = "";
  QString baseName = "";
  bool found = true;
};

#endif // GAMEENTRY_H
