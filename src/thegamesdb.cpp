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

#include <QDomDocument>
#include <QDomNodeList>

#include "thegamesdb.h"

TheGamesDb::TheGamesDb()
{
  connect(&manager, &NetComm::dataReady, &q, &QEventLoop::quit);

  baseUrl = "http://thegamesdb.net";

  searchUrlPre = "http://thegamesdb.net/api/GetGamesList.php?name=";
  
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
  QDomDocument xmlResults;
  xmlResults.setContent(data);
  
  QDomNodeList xmlGames = xmlResults.elementsByTagName("Game");
  
  for(int a = 0; a < xmlGames.count(); ++a) {
    GameEntry game;
    game.url = "http://thegamesdb.net/api/GetGame.php?id=" +
      xmlGames.at(a).firstChildElement("id").text();
    game.title = xmlGames.at(a).firstChildElement("GameTitle").text();
    game.platform = xmlGames.at(a).firstChildElement("Platform").text();

    if(platformMatch(game.platform, platform)) {
      gameEntries.append(game);
    }
  }
}

void TheGamesDb::getGameData(GameEntry &game)
{
  manager.request(game.url);
  q.exec();
  data = manager.getData();
  if(data.indexOf("503 Service Unavailable") != -1) {
    qDebug("It would seem that TheGamesDb is currently offline or having difficulties. You could try setting another scraping module using '-s' or wait until TheGamesDb is back in service.\nNow quitting...\n");
    exit(1);
  }
  QDomDocument xmlDoc;
  xmlDoc.setContent(data);

  xmlGame = xmlDoc.elementsByTagName("Game").at(0);
  
  for(int a = 0; a < fetchOrder.length(); ++a) {
    switch(fetchOrder.at(a)) {
    case DESCRIPTION:
      getDescription(game);
      break;
    case DEVELOPER:
      getDeveloper(game);
      break;
    case PUBLISHER:
      getPublisher(game);
      break;
    case PLAYERS:
      getPlayers(game);
      break;
    case RATING:
      getRating(game);
      break;
    case TAGS:
      getTags(game);
      break;
    case RELEASEDATE:
      getReleaseDate(game);
      break;
    case COVER:
      getCover(game);
      break;
    case SCREENSHOT:
      getScreenshot(game);
      break;
    case VIDEO:
      if(config->videos) {
	getVideo(game);
      }
      break;
    default:
      ;
    }
  }
}

void TheGamesDb::getRating(GameEntry &game)
{
  game.rating = xmlGame.firstChildElement("Rating").text();
  bool toDoubleOk = false;
  double rating = game.rating.toDouble(&toDoubleOk);
  if(toDoubleOk) {
    game.rating = QString::number(rating / 10.0);
  } else {
    game.rating = "";
  }
}

void TheGamesDb::getReleaseDate(GameEntry &game)
{
  QString releaseDate = xmlGame.firstChildElement("ReleaseDate").text();
  if(releaseDate.length() == 4 && releaseDate.toInt() != 0) {
    game.releaseDate = QDate::fromString(releaseDate, "yyyy").toString("yyyyMMdd");
  } else if(releaseDate.length() == 7 &&
	    releaseDate.left(4).toInt() != 0 &&
	    releaseDate.left(4).toInt() < 3000 &&
	    releaseDate.mid(5, 2).toInt() != 0 &&
	    releaseDate.mid(5, 2).toInt() <= 12) {
    game.releaseDate = QDate::fromString(releaseDate, "yyyy-MM").toString("yyyyMMdd");
  } else if(releaseDate.length() == 10 &&
	    releaseDate.left(4).toInt() != 0 &&
	    releaseDate.left(4).toInt() < 3000 &&
	    releaseDate.mid(5, 2).toInt() != 0 &&
	    releaseDate.mid(5, 2).toInt() <= 12 &&
	    releaseDate.mid(8, 2).toInt() != 0 &&
	    releaseDate.mid(8, 2).toInt() <= 31) {
    game.releaseDate = QDate::fromString(releaseDate, "yyyy-MM-dd").toString("yyyyMMdd");
  } else if(releaseDate.length() == 10 &&
	    releaseDate.left(2).toInt() != 0 &&
	    releaseDate.left(2).toInt() <= 12 &&
	    releaseDate.mid(3, 2).toInt() != 0 &&
	    releaseDate.mid(3, 2).toInt() <= 31 &&
	    releaseDate.mid(6, 4).toInt() != 0 &&
	    releaseDate.mid(6, 4).toInt() < 3000) {
    game.releaseDate = QDate::fromString(releaseDate, "MM/dd/yyyy").toString("yyyyMMdd");
  } else if(releaseDate.length() == 9 &&
	    (releaseDate.left(3) == "Jan" ||
	     releaseDate.left(3) == "Feb" ||
	     releaseDate.left(3) == "Mar" ||
	     releaseDate.left(3) == "Apr" ||
	     releaseDate.left(3) == "May" ||
	     releaseDate.left(3) == "Jun" ||
	     releaseDate.left(3) == "Jul" ||
	     releaseDate.left(3) == "Aug" ||
	     releaseDate.left(3) == "Sep" ||
	     releaseDate.left(3) == "Oct" ||
	     releaseDate.left(3) == "Nov" ||
	     releaseDate.left(3) == "Dec") &&
	    releaseDate.mid(5, 4).toInt() != 0 &&
	    releaseDate.mid(5, 4).toInt() < 3000) {
    game.releaseDate = QDate::fromString(releaseDate, "MMM, yyyy").toString("yyyyMMdd");
  } else if(releaseDate.length() == 12 &&
	    (releaseDate.left(3) == "Jan" ||
	     releaseDate.left(3) == "Feb" ||
	     releaseDate.left(3) == "Mar" ||
	     releaseDate.left(3) == "Apr" ||
	     releaseDate.left(3) == "May" ||
	     releaseDate.left(3) == "Jun" ||
	     releaseDate.left(3) == "Jul" ||
	     releaseDate.left(3) == "Aug" ||
	     releaseDate.left(3) == "Sep" ||
	     releaseDate.left(3) == "Oct" ||
	     releaseDate.left(3) == "Nov" ||
	     releaseDate.left(3) == "Dec") &&
	    releaseDate.mid(4, 2).toInt() != 0 &&
	    releaseDate.mid(4, 2).toInt() <= 31 &&
	    releaseDate.mid(8, 4).toInt() != 0 &&
	    releaseDate.mid(8, 4).toInt() < 3000) {
    game.releaseDate = QDate::fromString(releaseDate, "MMM dd, yyyy").toString("yyyyMMdd");
  } else {
    game.releaseDate = releaseDate;
  }
}

void TheGamesDb::getDeveloper(GameEntry &game)
{
  game.developer = xmlGame.firstChildElement("Developer").text();
}

void TheGamesDb::getPublisher(GameEntry &game)
{
  game.publisher = xmlGame.firstChildElement("Publisher").text();
}

void TheGamesDb::getDescription(GameEntry &game)
{
  game.description = xmlGame.firstChildElement("Overview").text();
}

void TheGamesDb::getPlayers(GameEntry &game)
{
  game.players = xmlGame.firstChildElement("Players").text();
}

void TheGamesDb::getTags(GameEntry &game)
{
  QDomNodeList xmlGenres = xmlGame.firstChildElement("Genres").elementsByTagName("genre");
  for(int a = 0; a < xmlGenres.count(); ++a) {
    game.tags.append(xmlGenres.at(a).toElement().text() + ", ");
  }
  game.tags = game.tags.left(game.tags.length() - 2);
}

void TheGamesDb::getCover(GameEntry &game)
{
  QDomNodeList xmlImages = xmlGame.firstChildElement("Images").elementsByTagName("boxart");
  for(int a = 0; a < xmlImages.count(); ++a) {
    if(xmlImages.at(a).toElement().attribute("side") == "front") {
      QString coverUrl = baseUrl + "/banners/" + xmlImages.at(a).toElement().text();
      manager.request(coverUrl);
      q.exec();
      QImage image(QImage::fromData(manager.getData()));
      if(!image.isNull()) {
	game.coverData = image;
      }
      break;
    }
  }
}

void TheGamesDb::getScreenshot(GameEntry &game)
{
  QDomNodeList xmlScreenshots =
    xmlGame.firstChildElement("Images").elementsByTagName("screenshot");
  if(!xmlScreenshots.isEmpty()) {
    QString screenshotUrl = baseUrl + "/banners/" + xmlScreenshots.at(0).firstChildElement("original").text();
    manager.request(screenshotUrl);
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
  } else if(platform == "scummvm") {
    return "pc;amiga;amiga cd32";
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
