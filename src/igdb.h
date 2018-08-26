/***************************************************************************
 *            igdb.h
 *
 *  Sun Aug 26 12:00:00 CEST 2018
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

#ifndef IGDB_H
#define IGDB_H

#include <QJsonDocument>
#include <QJsonObject>

#include "abstractscraper.h"

class Igdb : public AbstractScraper
{
  Q_OBJECT

public:
  Igdb();
  void runPasses(QList<GameEntry> &gameEntries, const QFileInfo &info, QString &output, QString &, QString &debug);

private:
  void getSearchResults(QList<GameEntry> &gameEntries,
			QString searchName, QString platform);
  void getGameData(GameEntry &game);
  void getReleaseDate(GameEntry &game);
  void getPlayers(GameEntry &game);
  void getTags(GameEntry &game);
  void getDeveloper(GameEntry &game);
  void getPublisher(GameEntry &game);
  void getDescription(GameEntry &game);
  void getAges(GameEntry &game);
  void getRating(GameEntry &game);
  void getCover(GameEntry &game);
  void getScreenshot(GameEntry &game);
  //QString getSearchName(QFileInfo info);

  void loadMaps();

  QJsonDocument jsonDoc;
  QJsonObject jsonObj;
  
  QMap<int, QString> platformMap;
  QMap<int, QString> genreMap;
};

#endif // IGDB_H
