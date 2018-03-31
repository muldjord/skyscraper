/***************************************************************************
 *            mobygames.cpp
 *
 *  Fri Mar 30 12:00:00 CEST 2018
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

#include <QJsonArray>

#include "mobygames.h"
#include "strtools.h"

MobyGames::MobyGames()
{
  connect(&manager, &NetComm::dataReady, &q, &QEventLoop::quit);

  connect(&limitTimer, &QTimer::timeout, &limiter, &QEventLoop::quit);
  limitTimer.setInterval(10000); // 10 second request limit
  limitTimer.setSingleShot(false);
  limitTimer.start();
  
  baseUrl = "https://api.mobygames.com";

  searchUrlPre = "https://api.mobygames.com/v1/games";
  
  fetchOrder.append(PUBLISHER);
  fetchOrder.append(RELEASEDATE);
  fetchOrder.append(TAGS);
  fetchOrder.append(PLAYERS);
  fetchOrder.append(DESCRIPTION);
  fetchOrder.append(SCREENSHOT);
  fetchOrder.append(COVER);
  fetchOrder.append(MARQUEE);
  fetchOrder.append(VIDEO);
}

void MobyGames::getSearchResults(QList<GameEntry> &gameEntries,
				QString searchName, QString platform)
{
  // Wait here until timeout, which will happen at the most every 10 seconds
  limiter.exec();
  
  manager.request(searchUrlPre + "?api_key=" + StrTools::unMagic("175;229;170;189;188;202;211;117;164;165;185;209;164;234;180;155;199;209;224;231;193;190;173;175") + "&title=" + searchName);
  q.exec();
  data = manager.getData();

  jsonDoc = QJsonDocument::fromJson(data);
  if(jsonDoc.isEmpty()) {
    return;
  }
  jsonObj = jsonDoc.object().value("games").toArray().first().toObject();

  if(jsonObj.value("title") == QJsonValue::Undefined) {
    return;
  }

  GameEntry game;
  
  game.title = jsonObj.value("title").toString();
  game.platform = platform;
  gameEntries.append(game);
}

void MobyGames::getGameData(GameEntry &game)
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
    case WHEEL:
      getWheel(game);
      break;
    case MARQUEE:
      getMarquee(game);
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

void MobyGames::getReleaseDate(GameEntry &game)
{
  game.releaseDate = jsonObj.value("year").toString();
}

void MobyGames::getPlayers(GameEntry &game)
{
  game.players = QString::number(jsonObj.value("players").toInt());
}

void MobyGames::getTags(GameEntry &game)
{
  game.tags = jsonObj.value("genre").toString();
}

void MobyGames::getPublisher(GameEntry &game)
{
  game.publisher = jsonObj.value("manufacturer").toString();
}

void MobyGames::getDescription(GameEntry &game)
{
  game.description = jsonObj.value("history").toString();
  if(game.description.indexOf("- TECHNICAL") != -1) {
    game.description = game.description.left(game.description.indexOf("- TECHNICAL")).trimmed();
  }
}

void MobyGames::getCover(GameEntry &game)
{
  manager.request(jsonObj.value("url_image_flyer").toString());
  q.exec();
  {
    QImage image;
    if(image.loadFromData(manager.getData())) {
      game.coverData = image;
      return;
    }
  }
  manager.request(jsonObj.value("url_image_title").toString());
  q.exec();
  {
    QImage image;
    if(image.loadFromData(manager.getData())) {
      game.coverData = image;
      return;
    }
  }
}

void MobyGames::getScreenshot(GameEntry &game)
{
  manager.request(jsonObj.value("url_image_ingame").toString());
  q.exec();
  QImage image;
  if(image.loadFromData(manager.getData())) {
    game.screenshotData = image;
  }
}

void MobyGames::getMarquee(GameEntry &game)
{
  manager.request(jsonObj.value("url_image_marquee").toString());
  q.exec();
  QImage image;
  if(image.loadFromData(manager.getData())) {
    game.marqueeData = image;
  }
}

void MobyGames::getVideo(GameEntry &game)
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

QString MobyGames::getSearchName(QFileInfo info)
{
  return info.baseName();
}
