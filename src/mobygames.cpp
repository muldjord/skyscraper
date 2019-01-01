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

MobyGames::MobyGames(Settings *config) : AbstractScraper(config)
{
  connect(&manager, &NetComm::dataReady, &q, &QEventLoop::quit);

  connect(&limitTimer, &QTimer::timeout, &limiter, &QEventLoop::quit);
  limitTimer.setInterval(10000); // 10 second request limit
  limitTimer.setSingleShot(false);
  limitTimer.start();
  
  baseUrl = "https://api.mobygames.com";

  searchUrlPre = "https://api.mobygames.com/v1/games";
  
  fetchOrder.append(PUBLISHER);
  fetchOrder.append(DEVELOPER);
  fetchOrder.append(RELEASEDATE);
  fetchOrder.append(TAGS);
  fetchOrder.append(PLAYERS);
  fetchOrder.append(DESCRIPTION);
  fetchOrder.append(AGES);
  fetchOrder.append(RATING);
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

  if(jsonDoc.object().value("code").toInt() == 429) {
    printf("\033[1;31mToo many requests!!! This is probably because some other Skyscraper user is currently using the 'mobygames' module. Please wait a while and try again.\n\nNow quitting...\033[0m\n");
    reqRemaining = 0;
  }

  QJsonArray jsonGames = jsonDoc.object().value("games").toArray();

  while(!jsonGames.isEmpty()) {
    GameEntry game;
    
    QJsonObject jsonGame = jsonGames.first().toObject();
    
    game.id = QString::number(jsonGame.value("game_id").toInt());
    game.title = jsonGame.value("title").toString();
    game.miscData = QJsonDocument(jsonGame).toJson(QJsonDocument::Compact);

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

  jsonDoc = QJsonDocument::fromJson(data);
  if(jsonDoc.isEmpty()) {
    return;
  }

  jsonObj = QJsonDocument::fromJson(game.miscData).object();

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
    case AGES:
      getAges(game);
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
    default:
      ;
    }
  }
}

void MobyGames::getReleaseDate(GameEntry &game)
{
  game.releaseDate = jsonDoc.object().value("first_release_date").toString();
}

void MobyGames::getPlayers(GameEntry &game)
{
  QJsonArray jsonAttribs = jsonDoc.object().value("attributes").toArray();
  for(int a = 0; a < jsonAttribs.count(); ++a) {
    if(jsonAttribs.at(a).toObject().value("attribute_category_name").toString() == "Number of Players Supported") {
      game.players = jsonAttribs.at(a).toObject().value("attribute_name").toString();
    }
  }
}

void MobyGames::getTags(GameEntry &game)
{
  QJsonArray jsonGenres = jsonObj.value("genres").toArray();
  for(int a = 0; a < jsonGenres.count(); ++a) {
    game.tags.append(jsonGenres.at(a).toObject().value("genre_name").toString() + ", ");
  }
  game.tags = game.tags.left(game.tags.length() - 2);
}

void MobyGames::getAges(GameEntry &game)
{
  QJsonArray jsonAges = jsonDoc.object().value("ratings").toArray();
  for(int a = 0; a < jsonAges.count(); ++a) {
    if(jsonAges.at(a).toObject().value("rating_system_name").toString() == "PEGI Rating") {
      game.ages = jsonAges.at(a).toObject().value("rating_name").toString();
      break;
    }
  }
  for(int a = 0; a < jsonAges.count(); ++a) {
    if(jsonAges.at(a).toObject().value("rating_system_name").toString() == "ELSPA Rating") {
      game.ages = jsonAges.at(a).toObject().value("rating_name").toString();
      break;
    }
  }
  for(int a = 0; a < jsonAges.count(); ++a) {
    if(jsonAges.at(a).toObject().value("rating_system_name").toString() == "ESRB Rating") {
      game.ages = jsonAges.at(a).toObject().value("rating_name").toString();
      break;
    }
  }
  for(int a = 0; a < jsonAges.count(); ++a) {
    if(jsonAges.at(a).toObject().value("rating_system_name").toString() == "USK Rating") {
      game.ages = jsonAges.at(a).toObject().value("rating_name").toString();
      break;
    }
  }
  for(int a = 0; a < jsonAges.count(); ++a) {
    if(jsonAges.at(a).toObject().value("rating_system_name").toString() == "OFLC (Australia) Rating") {
      game.ages = jsonAges.at(a).toObject().value("rating_name").toString();
      break;
    }
  }
  for(int a = 0; a < jsonAges.count(); ++a) {
    if(jsonAges.at(a).toObject().value("rating_system_name").toString() == "SELL Rating") {
      game.ages = jsonAges.at(a).toObject().value("rating_name").toString();
      break;
    }
  }
  for(int a = 0; a < jsonAges.count(); ++a) {
    if(jsonAges.at(a).toObject().value("rating_system_name").toString() == "BBFC Rating") {
      game.ages = jsonAges.at(a).toObject().value("rating_name").toString();
      break;
    }
  }
  for(int a = 0; a < jsonAges.count(); ++a) {
    if(jsonAges.at(a).toObject().value("rating_system_name").toString() == "OFLC (New Zealand) Rating") {
      game.ages = jsonAges.at(a).toObject().value("rating_name").toString();
      break;
    }
  }
  for(int a = 0; a < jsonAges.count(); ++a) {
    if(jsonAges.at(a).toObject().value("rating_system_name").toString() == "VRC Rating") {
      game.ages = jsonAges.at(a).toObject().value("rating_name").toString();
      break;
    }
  }
}

void MobyGames::getPublisher(GameEntry &game)
{
  QJsonArray jsonReleases = jsonDoc.object().value("releases").toArray();
  for(int a = 0; a < jsonReleases.count(); ++a) {
    QJsonArray jsonCompanies = jsonReleases.at(a).toObject().value("companies").toArray();
    for(int b = 0; b < jsonCompanies.count(); ++b) {
      if(jsonCompanies.at(b).toObject().value("role").toString() == "Published by") {
	game.publisher = jsonCompanies.at(b).toObject().value("company_name").toString();
	return;
      }
    }
  }
}

void MobyGames::getDeveloper(GameEntry &game)
{
  QJsonArray jsonReleases = jsonDoc.object().value("releases").toArray();
  for(int a = 0; a < jsonReleases.count(); ++a) {
    QJsonArray jsonCompanies = jsonReleases.at(a).toObject().value("companies").toArray();
    for(int b = 0; b < jsonCompanies.count(); ++b) {
      if(jsonCompanies.at(b).toObject().value("role").toString() == "Developed by") {
	game.developer = jsonCompanies.at(b).toObject().value("company_name").toString();
	return;
      }
    }
  }
}

void MobyGames::getDescription(GameEntry &game)
{
  game.description = jsonObj.value("description").toString();

  // Remove all html tags within description
  game.description = StrTools::stripHtmlTags(game.description);
}

void MobyGames::getRating(GameEntry &game)
{
  QJsonValue jsonValue = jsonObj.value("moby_score");
  if(jsonValue != QJsonValue::Undefined) {
    double rating = jsonValue.toDouble();
    if(rating != 0.0) {
      game.rating = QString::number(rating / 5.0);
    }
  }
}

void MobyGames::getCover(GameEntry &game)
{
  printf("Waiting to get cover data...\n");
  limiter.exec();
  manager.request(game.url.left(game.url.indexOf("?api_key=")) + "/covers" + game.url.mid(game.url.indexOf("?api_key="), game.url.length() - game.url.indexOf("?api_key=")));
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
	found = true;
	break;
      }
      jsonCovers.removeFirst();
    }
    if(found) {
      break;
    }
    jsonCoverGroups.removeFirst();
  }
  
  if(!coverUrl.isEmpty()) {
    manager.request(coverUrl.replace("http://", "https://"));
    q.exec();
    QImage image;
    if(image.loadFromData(manager.getData())) {
      game.coverData = image;
    }
  }
}

void MobyGames::getScreenshot(GameEntry &game)
{
  printf("Waiting to get screenshot data...\n");
  limiter.exec();
  manager.request(game.url.left(game.url.indexOf("?api_key=")) + "/screenshots" + game.url.mid(game.url.indexOf("?api_key="), game.url.length() - game.url.indexOf("?api_key=")));
  q.exec();
  data = manager.getData();

  jsonDoc = QJsonDocument::fromJson(data);
  if(jsonDoc.isEmpty()) {
    return;
  }

  QJsonArray jsonScreenshots = jsonDoc.object().value("screenshots").toArray();
  
  if(jsonScreenshots.count() < 1) {
    return;
  }
  int chosen = 1;
  if(jsonScreenshots.count() >= 3) {
    // First 2 are almost always not ingame, so skip those if we have 3 or more
    chosen = (qrand() % jsonScreenshots.count() - 3) + 3;
  }
  manager.request(jsonScreenshots.at(chosen).toObject().value("image").toString().replace("http://", "https://"));
  q.exec();
  QImage image;
  if(image.loadFromData(manager.getData())) {
    game.screenshotData = image;
  }
}
