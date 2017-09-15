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

#include <QJsonArray>

#include "arcadedb.h"
#include "strtools.h"

ArcadeDB::ArcadeDB()
{
  connect(&manager, &NetComm::dataReady, &q, &QEventLoop::quit);

  baseUrl = "http://adb.arcadeitalia.net";

  searchUrlPre = "http://adb.arcadeitalia.net/service_scraper.php?ajax=query_mame&use_parent=1&game_name=";
  
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
  
  jsonDoc = QJsonDocument::fromJson(data);
  if(jsonDoc.isEmpty()) {
    return;
  }
  qDebug("DATA:\n%s\n", data.data());
  jsonObj = jsonDoc.object().value("result").toArray().first().toObject();

  GameEntry game;
  
  game.title = jsonObj.value("title").toString();
  game.platform = platform;
  gameEntries.append(game);
}

void ArcadeDB::getGameData(GameEntry &game)
{
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

void ArcadeDB::getReleaseDate(GameEntry &game)
{
  QString releaseDate = jsonObj.value("year").toString();
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

void ArcadeDB::getPlayers(GameEntry &game)
{
  game.players = QString::number(jsonObj.value("players").toInt());
}

void ArcadeDB::getTags(GameEntry &game)
{
  game.tags = jsonObj.value("genre").toString();
}

void ArcadeDB::getPublisher(GameEntry &game)
{
  game.publisher = jsonObj.value("manufacturer").toString();
}

void ArcadeDB::getDescription(GameEntry &game)
{
  game.description = jsonObj.value("history").toString();
  if(game.description.indexOf("- TECHNICAL") != -1) {
    game.description = game.description.left(game.description.indexOf("- TECHNICAL")).trimmed();
  }
}

void ArcadeDB::getCover(GameEntry &game)
{
  manager.request(jsonObj.value("url_image_flyer").toString());
  q.exec();
  {
    QImage image(QImage::fromData(manager.getData()));
    if(!image.isNull()) {
      game.coverData = image;
      return;
    }
  }
  manager.request(jsonObj.value("url_image_title").toString());
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
  manager.request(jsonObj.value("url_image_ingame").toString());
  q.exec();
  QImage image(QImage::fromData(manager.getData()));
  if(!image.isNull()) {
    game.screenshotData = image;
  }
}

void ArcadeDB::getVideo(GameEntry &game)
{
  manager.request(jsonObj.value("url_video_shortplay").toString());
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
