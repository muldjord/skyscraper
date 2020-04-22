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

ArcadeDB::ArcadeDB(Settings *config) : AbstractScraper(config)
{
  connect(&manager, &NetComm::dataReady, &q, &QEventLoop::quit);

  baseUrl = "http://adb.arcadeitalia.net";

  searchUrlPre = "http://adb.arcadeitalia.net/service_scraper.php?ajax=query_mame&lang=en&use_parent=1&game_name=";
  
  fetchOrder.append(PUBLISHER);
  fetchOrder.append(RELEASEDATE);
  fetchOrder.append(TAGS);
  fetchOrder.append(PLAYERS);
  fetchOrder.append(DESCRIPTION);
  fetchOrder.append(SCREENSHOT);
  fetchOrder.append(COVER);
  fetchOrder.append(WHEEL);
  fetchOrder.append(MARQUEE);
  fetchOrder.append(VIDEO);
}

void ArcadeDB::getSearchResults(QList<GameEntry> &gameEntries,
				QString searchName, QString platform)
{
  manager.request(searchUrlPre + searchName);
  q.exec();
  data = manager.getData();

  if(data.indexOf("{\"release\":1,\"result\":[]}") != -1) {
    return;
  }
  jsonDoc = QJsonDocument::fromJson(data);
  if(jsonDoc.isEmpty()) {
    return;
  }
  jsonObj = jsonDoc.object().value("result").toArray().first().toObject();

  if(jsonObj.value("title") == QJsonValue::Undefined) {
    return;
  }

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
      if(config->cacheCovers) {
	getCover(game);
      }
      break;
    case SCREENSHOT:
      if(config->cacheScreenshots) {
	getScreenshot(game);
      }
      break;
    case WHEEL:
      if(config->cacheWheels) {
	getWheel(game);
      }
      break;
    case MARQUEE:
      if(config->cacheMarquees) {
	getMarquee(game);
      }
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
  game.releaseDate = jsonObj.value("year").toString();
}

void ArcadeDB::getPlayers(GameEntry &game)
{
  game.players = QString::number(jsonObj.value("players").toInt());
}

void ArcadeDB::getTags(GameEntry &game)
{
  game.tags = jsonObj.value("genre").toString().replace(" / ", ", ");
}

void ArcadeDB::getPublisher(GameEntry &game)
{
  game.publisher = jsonObj.value("manufacturer").toString();
}

void ArcadeDB::getDescription(GameEntry &game)
{
  game.description = jsonObj.value("history").toString();
  if(game.description.contains("- TECHNICAL")) {
    game.description = game.description.left(game.description.indexOf("- TECHNICAL")).trimmed();
  }
}

void ArcadeDB::getCover(GameEntry &game)
{
  manager.request(jsonObj.value("url_image_flyer").toString());
  q.exec();
  {
    QImage image;
    if(image.loadFromData(manager.getData())) {
      game.coverData = manager.getData();
      return;
    }
  }
  manager.request(jsonObj.value("url_image_title").toString());
  q.exec();
  {
    QImage image;
    if(image.loadFromData(manager.getData())) {
      game.coverData = manager.getData();
      return;
    }
  }
}

void ArcadeDB::getScreenshot(GameEntry &game)
{
  manager.request(jsonObj.value("url_image_ingame").toString());
  q.exec();
  QImage image;
  if(image.loadFromData(manager.getData())) {
    game.screenshotData = manager.getData();
  }
}

void ArcadeDB::getWheel(GameEntry &game)
{
  manager.request("http://adb.arcadeitalia.net/media/mame.current/decals/" + jsonObj["game_name"].toString() + ".png");
  q.exec();
  QImage image;
  if(image.loadFromData(manager.getData())) {
    game.wheelData = manager.getData();
  }
}

void ArcadeDB::getMarquee(GameEntry &game)
{
  manager.request(jsonObj.value("url_image_marquee").toString());
  q.exec();
  QImage image;
  if(image.loadFromData(manager.getData())) {
    game.marqueeData = manager.getData();
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

QList<QString> ArcadeDB::getSearchNames(const QFileInfo &info)
{
  QList<QString> searchNames;
  searchNames.append(info.baseName());
  return searchNames;
}
