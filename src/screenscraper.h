/***************************************************************************
 *            screenscraper.h
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

#ifndef SCREENSCRAPER_H
#define SCREENSCRAPER_H

#include <QDomDocument>

#include "abstractscraper.h"

#define REGION 0
#define LANGUE 1
#define NONE 42

class ScreenScraper : public AbstractScraper
{
  Q_OBJECT

public:
  ScreenScraper();
  void runPasses(QList<GameEntry> &gameEntries, const QFileInfo &info,
		 QString &output, QString &marking);

private:
  void getSearchResults(QList<GameEntry> &gameEntries, QString searchName, QString platform);
  void getGameData(GameEntry &game);
  void getReleaseDate(GameEntry &game);
  void getDeveloper(GameEntry &game);
  void getPublisher(GameEntry &game);
  void getPlayers(GameEntry &game);
  void getDescription(GameEntry &game);
  void getTags(GameEntry &game);

  void getCover(GameEntry &game);
  void getScreenshot(GameEntry &game);
  void getWheel(GameEntry &game);
  void getMarquee(GameEntry &game);
  void getVideo(GameEntry &game);
  QString getHashes(const QFileInfo &info);

  QString getXmlText(QString node, int attr, QString type = "");

  QString region;
  QString lang;
  QDomDocument xmlDoc;  
  
};

#endif // SCREENSCRAPER_H
