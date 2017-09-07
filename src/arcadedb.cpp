/***************************************************************************
 *            arcadedb.cpp
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

#include "arcadedb.h"
#include "strtools.h"

ArcadeDB::ArcadeDB()
{
  connect(&manager, &NetComm::dataReady, &q, &QEventLoop::quit);

  baseUrl = "http://adb.arcadeitalia.net";

  searchUrlPre = "http://adb.arcadeitalia.net/dettaglio_mame.php?game_name=";
  //searchUrlPre = "http://adb.arcadeitalia.net/?search=mame&arcade_only=0&ricerca=";
  //searchUrlPre = "http://adb.arcadeitalia.net/dettaglio_mame.php?game_name=";
  //searchUrlPost = "&arcade_only=0&autosearch=1";
  
  titlePre.append("id=\"game_description\"");
  titlePre.append("\">");
  titlePost = "</div>";
  coverPre.append("<meta property=\"og:image\" content=\"");
  coverPost = "\" />";
  publisherPre.append("Manufacturer:");
  publisherPre.append("class=\"dettaglio\">");
  publisherPost = "</span>";
  releaseDatePre.append("Year:");
  releaseDatePre.append("class=\"dettaglio\">");
  releaseDatePost = "</span>";
  tagsPre.append("Category:");
  tagsPre.append("class=\"dettaglio\">");
  tagsPost = "</span>";
  playersPre.append("Players:");
  playersPre.append("class=\"dettaglio\">");
  playersPre.append("<b>");
  playersPost = "</b>";
  descriptionPre.append("<div id=\"history_detail\"");
  descriptionPre.append("\">");
  descriptionPost = "TECHNICAL";
  
  fetchOrder.append(PUBLISHER);
  fetchOrder.append(RELEASEDATE);
  fetchOrder.append(TAGS);
  fetchOrder.append(PLAYERS);
  //fetchOrder.append(RATING);
  fetchOrder.append(DESCRIPTION);
  fetchOrder.append(SCREENSHOT);
  fetchOrder.append(COVER);
  fetchOrder.append(VIDEO);
}

void ArcadeDB::getSearchResults(QList<GameEntry> &gameEntries,
				 QString searchName, QString platform)
{
  manager.request(searchUrlPre + searchName);
  q.exec();
  data = manager.getData();
  
  if(data.indexOf("<h2>Error") != -1) {
    return;
  }
  GameEntry game;
  
  foreach(QString nom, titlePre) {
    nomNom(nom);
  }
  game.title = data.left(data.indexOf(titlePost));
  game.title = game.title.left(game.title.indexOf("(")).simplified();
  game.platform = platform;
  game.url = searchUrlPre + searchName;
  gameEntries.append(game);
}

void ArcadeDB::getCover(GameEntry &game)
{
  manager.request("http://adb.arcadeitalia.net/media/mame.current/flyers/" + game.baseName + ".png");
  q.exec();
  {
    QImage image(QImage::fromData(manager.getData()));
    if(!image.isNull()) {
      game.coverData = image;
      return;
    }
  }
  manager.request("http://adb.arcadeitalia.net/media/mame.current/titles/" + game.baseName + ".png");
  q.exec();
  {
    QImage image(QImage::fromData(manager.getData()));
    if(!image.isNull()) {
      game.coverData = image;
      return;
    }
  }
}

void ArcadeDB::getScreenshot(GameEntry &game)
{
  manager.request("http://adb.arcadeitalia.net/media/mame.current/ingames/" + game.baseName + ".png");
  q.exec();
  QImage image(QImage::fromData(manager.getData()));
  if(!image.isNull()) {
    game.screenshotData = image;
  }
}

void ArcadeDB::getVideo(GameEntry &game)
{
  manager.request("http://adb.arcadeitalia.net/download_file.php?tipo=mame_current&codice=" + game.baseName + "&entity=shortplay&oper=streaming&filler=" + game.baseName + ".mp4");
  q.exec();
  game.videoData = manager.getData();
  if(game.videoData.length() > (1024 * 500)) {
    game.videoFormat = "mp4";
  } else {
    game.videoData = "";
  }
}

QString ArcadeDB::getSearchName(QString baseName)
{
  return baseName;
}

// http://adb.arcadeitalia.net/media/mame.current/ingames/bomblord.png
// http://adb.arcadeitalia.net/media/mame.current/titles/bomblord.png

// http://adb.arcadeitalia.net/download_file.php?tipo=mame_current&codice=bublbobl&entity=shortplay&oper=streaming&filler=bublbobl.mp4
