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
  limitTimer.setInterval(1500); // 10 second request limit
  limitTimer.setSingleShot(false);
  limitTimer.start();
  
  baseUrl = "https://api.mobygames.com";

  searchUrlPre = "https://api.mobygames.com/v1/games";
  
  fetchOrder.append(PUBLISHER);
  fetchOrder.append(RELEASEDATE);
  fetchOrder.append(TAGS);
  fetchOrder.append(PLAYERS);
  fetchOrder.append(DESCRIPTION);
  // Cover and screenshot MUST be last and in this order!
  fetchOrder.append(COVER);
  fetchOrder.append(SCREENSHOT);
}

void MobyGames::getSearchResults(QList<GameEntry> &gameEntries,
				QString searchName, QString platform)
{
  printf("Waiting as advised by MobyGames api restrictions...\n");
  limiter.exec();
  manager.request(searchUrlPre + "?api_key=" + StrTools::unMagic("175;229;170;189;188;202;211;117;164;165;185;209;164;234;180;155;199;209;224;231;193;190;173;175") + "&title=" + searchName);
  q.exec();
  data = manager.getData();

  jsonDoc = QJsonDocument::fromJson(data);
  if(jsonDoc.isEmpty()) {
    return;
  }

  QJsonArray jsonGames = jsonDoc.object().value("games").toArray();

  while(!jsonGames.isEmpty()) {
    GameEntry game;
    
    QJsonObject jsonGame = jsonGames.first().toObject();
    
    game.id = QString::number(jsonGame.value("game_id").toInt());
    game.title = jsonGame.value("title").toString();
    game.miscData = data;

    QJsonArray jsonPlatforms = jsonGame.value("platforms").toArray();
    while(!jsonPlatforms.isEmpty()) {
      QJsonObject jsonPlatform = jsonPlatforms.first().toObject();
      game.url = searchUrlPre + "/" + game.id + "/platforms/" + QString::number(jsonPlatform.value("platform_id").toInt()) + "?api_key=" + StrTools::unMagic("175;229;170;189;188;202;211;117;164;165;185;209;164;234;180;155;199;209;224;231;193;190;173;175");
      game.platform = jsonPlatform.value("platform_name").toString();
      if(platformMatch(game.platform, platform)) {
	gameEntries.append(game);
      }
      jsonPlatforms.removeFirst();
    }
    jsonGames.removeFirst();
  }
}

void MobyGames::getGameData(GameEntry &game)
{
  printf("Waiting to get game data...\n");
  limiter.exec();
  manager.request(game.url);
  q.exec();
  data = manager.getData();

  // Trim data to remove { and } before appending together into a single json doc
  data = data.trimmed();
  data = data.remove(data.length() - 1, 1).trimmed().append(",\n");
  game.miscData = game.miscData.remove(0, 2);
  // Readd data from initial search as we need it
  data.append(game.miscData);

  jsonDoc = QJsonDocument::fromJson(data);
  if(jsonDoc.isEmpty()) {
    return;
  }

  printf("DATA:\n%s\n", data.data());
  
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
  printf("Waiting to get cover data...\n");
  limiter.exec();
  manager.request(game.url.insert(game.url.indexOf("?api_key="), "/covers"));
  q.exec();
  data = manager.getData();

  jsonDoc = QJsonDocument::fromJson(data);
  if(jsonDoc.isEmpty()) {
    return;
  }

  QString coverUrl = "";
  bool found = false;

  QJsonArray jsonCoverGroups = jsonDoc.object().value("cover_groups").toArray();
  while(!jsonCoverGroups.isEmpty()) {
    QJsonArray jsonCovers = jsonCoverGroups.first().toObject().value("covers").toArray();
    while(!jsonCovers.isEmpty()) {
      QJsonObject jsonCover = jsonCovers.first().toObject();

      if(jsonCover.value("scan_of").toString().toLower() == "front cover") {
	coverUrl = jsonCover.value("image").toString();
	break;
      }
      jsonCovers.removeFirst();
    }
    if(found) {
      break;
    }
    jsonCoverGroups.removeFirst();
  }
  
  manager.request(coverUrl);
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
  printf("Waiting to get screenshot data...\n");
  limiter.exec();
  manager.request(game.url.replace("covers", "screenshots"));
  q.exec();
  data = manager.getData();

  jsonDoc = QJsonDocument::fromJson(data);
  if(jsonDoc.isEmpty()) {
    return;
  }

  QJsonArray jsonScreenshots = jsonDoc.object().value("screenshots").toArray();
  
  manager.request(jsonScreenshots.at(qrand() % jsonScreenshots.count()).toObject().value("image").toString());
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

void MobyGames::runPasses(QList<GameEntry> &gameEntries, const QFileInfo &info, QString &output, QString &, QString &debug)
{
  QString searchName = getSearchName(info);
  QString searchNameOrig = searchName;

  // searchName will be empty for files such as "[BIOS] Something.zip" and cause some scraping
  // modules to return EVERYTHING in their database. We DO NOT want this since it take ages
  // to parse it (15 minutes or more per entry) and it's faulty data anyways.
  if(searchName.isEmpty()) {
    return;
  }
  
  for(int pass = 1; pass <= 1; ++pass) {
    // Reset searchName for each pass
    searchName = searchNameOrig;
    output.append("\033[1;35mPass " + QString::number(pass) + "\033[0m ");
    switch(pass) {
    case 1:
      getSearchResults(gameEntries, searchName, config->platform);
      break;
    default:
      ;
    }
    debug.append("Search string: '" + searchName + "'\n");
    debug.append("Platform: '" + config->platform + "'\n");
    if(!gameEntries.isEmpty()) {
      break;
    }
  }
}
/*
QString MobyGames::getSearchName(QFileInfo info)
{
  return info.baseName();
}
*/
