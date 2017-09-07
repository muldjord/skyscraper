/***************************************************************************
 *            mamedb.cpp
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

#include "mamedb.h"

MameDB::MameDB()
{
  baseUrl = "http://mamedb.blu-ferret.co.uk";

  screenshotCounter = "Snapshot:";
  screenshotPre.append("<a href='/image/snap");
  screenshotPre.append("img src='");
  screenshotPost = "' alt";
  coverPre.append("/image/title");
  coverPre.append("<img src='");
  coverPost = "' alt";
  titlePre.append("Name:&nbsp</b>");
  titlePost = " &nbsp;";
  releaseDatePre.append("Year:&nbsp</b>");
  releaseDatePre.append("'>");
  releaseDatePost = "</a>";
  developerPre.append("Manufacturer:&nbsp</b>");
  developerPre.append("'>");
  developerPost = "</a>";
  tagsPre.append("Category:&nbsp;</b>");
  tagsPre.append("'>");
  tagsPost = "</a>";
  playersPre.append("Players:&nbsp;</b>");
  playersPost = "<br/>";
  
  connect(&manager, &NetComm::dataReady, &q, &QEventLoop::quit);

  fetchOrder.append(SCREENSHOT);
  fetchOrder.append(COVER);
  fetchOrder.append(RELEASEDATE);
  fetchOrder.append(DEVELOPER);
  fetchOrder.append(TAGS);
  fetchOrder.append(PLAYERS);
}

void MameDB::getSearchResults(QList<GameEntry> &gameEntries,
				  QString searchName, QString platform)
{
  GameEntry game;

  game.url = baseUrl + "/game/" + searchName + "/";

  manager.request(game.url);
  q.exec();
  data = manager.getData();
  
  foreach(QString nom, titlePre) {
    nomNom(nom);
  }
  game.title = data.left(data.indexOf(titlePost));

  game.platform = "arcade";

  if(platformMatch(game.platform, platform)) {
    gameEntries.append(game);
  }
}

QString MameDB::actualPlatform(const QString platform)
{
  if(platform == "neogeo") {
    return "arcade";
  }

  return platform;
}
