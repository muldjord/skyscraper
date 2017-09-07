/***************************************************************************
 *            thegamesdb.cpp
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

#include "thegamesdb.h"

TheGamesDb::TheGamesDb()
{
  connect(&manager, &NetComm::dataReady, &q, &QEventLoop::quit);

  baseUrl = "http://thegamesdb.net";

  searchUrlPre = "http://thegamesdb.net/api/GetGamesList.php?name=";
  
  searchResultPre = "<id>";
  urlPost = "</id>";
  titlePre.append("<GameTitle>");
  titlePost = "</GameTitle>";
  platformPre.append("<Platform>");
  platformPost = "</Platform>";

  coverPre.append("<boxart side=\"front");
  coverPre.append("\">");
  coverPost = "</boxart>";
  developerPre.append("<Developer>");
  developerPost = "</Developer>";
  ratingPre.append("<Rating>");
  ratingPost = "</Rating>";
  publisherPre.append("<Publisher>");
  publisherPost = "</Publisher>";
  playersPre.append("<Players>");
  playersPost = "</Players>";
  releaseDatePre.append("<ReleaseDate>");
  releaseDatePost = "</ReleaseDate>";
  descriptionPre.append("<Overview>");
  descriptionPost = "</Overview>";
  tagsPre.append("<genre>");
  tagsPost = "</genre>";

  screenshotPre.append("<original width=\"");
  screenshotPre.append("\">");
  screenshotPost = "</original>";
  
  screenshotCounter = "<screenshot>";

  fetchOrder.append(RELEASEDATE);
  fetchOrder.append(DESCRIPTION);
  fetchOrder.append(TAGS);
  fetchOrder.append(PLAYERS);
  fetchOrder.append(PUBLISHER);
  fetchOrder.append(DEVELOPER);
  fetchOrder.append(RATING);
  fetchOrder.append(COVER);
  fetchOrder.append(SCREENSHOT);
}

void TheGamesDb::getSearchResults(QList<GameEntry> &gameEntries,
				  QString searchName, QString platform)
{
  manager.request(searchUrlPre + searchName);
  q.exec();
  data = manager.getData();
  
  GameEntry game;

  while(data.indexOf(searchResultPre) != -1) {
    nomNom(searchResultPre);

    // Digest until url
    game.url = "http://thegamesdb.net/api/GetGame.php?id=" + data.left(data.indexOf(urlPost));

    // Digest until title
    foreach(QString nom, titlePre) {
      nomNom(nom);
    }
    game.title = data.left(data.indexOf(titlePost));

    // Digest until platform
    foreach(QString nom, platformPre) {
      nomNom(nom);
    }
    game.platform = data.left(data.indexOf(platformPost));

    if(platformMatch(game.platform, platform)) {
      gameEntries.append(game);
    }
  }
}

void TheGamesDb::getRating(GameEntry &game)
{
  if(ratingPre.isEmpty()) {
    return;
  }
  foreach(QString nom, ratingPre) {
    if(!checkNom(nom)) {
      return;
    }
  }
  foreach(QString nom, ratingPre) {
    nomNom(nom);
  }
  game.rating = data.left(data.indexOf(ratingPost));
  bool toDoubleOk = false;
  double rating = game.rating.toDouble(&toDoubleOk);
  if(toDoubleOk) {
    game.rating = QString::number(rating / 10.0);
  } else {
    game.rating = "";
  }
}

void TheGamesDb::getCover(GameEntry &game)
{
  foreach(QString nom, coverPre) {
    nomNom(nom);
  }
  QString coverUrl = data.left(data.indexOf(coverPost));
  if(coverUrl.indexOf("http") != -1) {
    manager.request(coverUrl);
  } else {
    manager.request(baseUrl + "/banners/" + coverUrl);
  }
  q.exec();
  QImage image(QImage::fromData(manager.getData()));
  if(!image.isNull()) {
    game.coverData = image;
  }
}

void TheGamesDb::getScreenshot(GameEntry &game)
{
  // Check that we have enough screenshots
  int screens = data.count(screenshotCounter.toUtf8());
  if(screens >= 1) {
    for(int a = 0; a < screens - (screens / 2); a++) {
      foreach(QString nom, screenshotPre) {
	nomNom(nom);
      }
    }
    manager.request(baseUrl + "/banners/" + data.left(data.indexOf(screenshotPost)));
    q.exec();
    QImage image(QImage::fromData(manager.getData()));
    if(!image.isNull()) {
      game.screenshotData = image;
    }
  }
}

QString TheGamesDb::actualPlatform(const QString platform)
{
  if(platform == "aga") {
    return "amiga";
  } else if(platform == "cd32") {
    return "amiga cd32";
  } else if(platform == "arcade") {
    return "arcade;neo geo;neo geo cd";
  } else if(platform == "atari2600") {
    return "atari 2600";
  } else if(platform == "atari7800") {
    return "atari 7800";
  } else if(platform == "atarist") {
    return "atari st";
  } else if(platform == "mastersystem") {
    return "sega master system";
  } else if(platform == "gamegear") {
    return "sega game gear";
  } else if(platform == "gba") {
    return "nintendo game boy advance";
  } else if(platform == "megadrive") {
    return "sega mega drive";
  } else if(platform == "coleco") {
    return "colecovision";
  } else if(platform == "c64") {
    return "commodore 64";
  } else if(platform == "genesis") {
    return "sega genesis";
  } else if(platform == "nes") {
    return "nintendo entertainment system (nes)";
  } else if(platform == "pcengine") {
    return "turbografx 16;turbografx cd";
  } else if(platform == "psx") {
    return "sony playstation";
  } else if(platform == "psp") {
    return "sony playstation portable";
  } else if(platform == "snes") {
    return "super nintendo (snes)";
  } else if(platform == "zxspectrum") {
    return "sinclair zx spectrum";
  } else if(platform == "atari5200") {
    return "atari 5200";
  } else if(platform == "apple2") {
    return "apple ii";
  } else if(platform == "atarijaguar") {
    return "atari jaguar";
  } else if(platform == "neogeo") {
    return "neo geo;neo geo cd;arcade";
  } else if(platform == "n64") {
    return "nintendo 64";
  } else if(platform == "nds") {
    return "nintendo ds";
  } else if(platform == "gb") {
    return "nintendo game boy";
  } else if(platform == "gba") {
    return "nintendo game boy advance";
  } else if(platform == "gbc") {
    return "nintendo game boy color";
  } else if(platform == "segacd") {
    return "sega cd";
  }
  return platform;
}
