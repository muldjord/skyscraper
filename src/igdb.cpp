/***************************************************************************
 *            igdb.cpp
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

#include <QJsonArray>

#include "igdb.h"
#include "strtools.h"

Igdb::Igdb()
{
  connect(&manager, &NetComm::dataReady, &q, &QEventLoop::quit);

  loadMaps();

  baseUrl = "https://api-endpoint.igdb.com";

  searchUrlPre = "https://api-endpoint.igdb.com/games/";
  
  fetchOrder.append(RELEASEDATE);
  fetchOrder.append(RATING);
  fetchOrder.append(PUBLISHER);
  fetchOrder.append(DEVELOPER);
  fetchOrder.append(DESCRIPTION);
  fetchOrder.append(PLAYERS);
  fetchOrder.append(TAGS);
  //fetchOrder.append(AGES);
  //fetchOrder.append(COVER);
  //fetchOrder.append(SCREENSHOT);
}

void Igdb::getSearchResults(QList<GameEntry> &gameEntries,
				QString searchName, QString platform)
{
  // Request list of games but don't allow re-releases (version_parent->not_exists=1)
  manager.request(searchUrlPre + "?search=" + searchName + "&fields=*&filter[version_parent][not_exists]=1", "", "user-key", config->userCreds);
  //manager.request("https://api-endpoint.igdb.com/genres/?id=*&fields=name&limit=50", "", "user-key", config->userCreds);
  q.exec();
  data = manager.getData();
  printf("DATA:\n%s\n", data.data());
  jsonDoc = QJsonDocument::fromJson(data);
  if(jsonDoc.isEmpty()) {
    return;
  }

  if(jsonDoc.object().value("status").toInt() == 403) {
    printf("\033[1;31mYour monthly limit for the 'igdb' scraping module has been reached. If you want more you can upgrade your plan at 'https://api.igdb.com/' under 'API credentials'.\033[0m\n");
    reqRemaining = 0;
  }

  QJsonArray jsonGames = jsonDoc.array();

  foreach(const QJsonValue &jsonGame, jsonGames) {
    GameEntry game;
    
    game.title = jsonGame.toObject().value("name").toString();
    game.miscData = QJsonDocument(jsonGame.toObject()).toJson(QJsonDocument::Compact);

    QJsonArray jsonPlatforms = jsonGame.toObject().value("platforms").toArray();
    foreach(const QJsonValue &jsonPlatform, jsonPlatforms) {
      game.id = QString::number(jsonPlatform.toInt());
      game.platform = platformMap[jsonPlatform.toInt()];
      if(platformMatch(game.platform, platform)) {
	gameEntries.append(game);
      }
    }
  }
}

void Igdb::getGameData(GameEntry &game)
{
  jsonDoc = QJsonDocument::fromJson(game.miscData);
  if(jsonDoc.isEmpty()) {
    return;
  }

  jsonObj = jsonDoc.object();
  
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

void Igdb::getReleaseDate(GameEntry &game)
{
  QJsonArray jsonDates = jsonObj.value("release_dates").toArray();
  foreach(const QJsonValue &jsonDate, jsonDates) {
    if(QString::number(jsonDate.toObject().value("platform").toInt()) == game.id) {
      game.releaseDate = jsonDate.toObject().value("human").toString();
      break;
    }
  }
}

void Igdb::getPlayers(GameEntry &game)
{
  // This is a bit of a hack. The unique identifiers are as follows:
  // 1 = Single Player
  // 2 = Multiplayer
  // 3 = Cooperative
  // 4 = Split screen
  // 5 = MMO
  // So basically if != 1 it's at least 2 players. That's all we can gather from this
  game.players = "1";
  QJsonArray jsonPlayers = jsonObj.value("game_modes").toArray();
  foreach(const QJsonValue &jsonPlayer, jsonPlayers) {
    if(QString::number(jsonPlayer.toInt()) != 1) {
      game.players = "2";
      break;
    }
  }
}

void Igdb::getTags(GameEntry &game)
{
  QJsonArray jsonGenres = jsonObj.value("genres").toArray();
  foreach(const QJsonValue &jsonGenre, jsonGenres) {
    game.tags.append(genreMap[jsonGenre.toInt()] + ", ");
  }
  game.tags = game.tags.left(game.tags.length() - 2);
}

void Igdb::getAges(GameEntry &game)
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

void Igdb::getPublisher(GameEntry &game)
{
  QJsonArray jsonPublishers = jsonObj.value("publishers").toArray();
  if(jsonPublishers.count() == 1) {
    game.publisher = QString::number(jsonPublishers.first().toInt());
  }
}

void Igdb::getDeveloper(GameEntry &game)
{
  QJsonArray jsonDevelopers = jsonObj.value("developers").toArray();
  if(jsonDevelopers.count() == 1) {
    game.developer = QString::number(jsonDevelopers.first().toInt());
  }
}

void Igdb::getDescription(GameEntry &game)
{
  game.description = jsonObj.value("summary").toString();

  // Remove all html tags within description
  while(game.description.indexOf("<") != -1 && game.description.indexOf(">") != -1 && game.description.indexOf("<") < game.description.indexOf(">")) {
    game.description = game.description.remove(game.description.indexOf("<"), game.description.indexOf(">") + 1 - game.description.indexOf("<"));
  }
}

void Igdb::getRating(GameEntry &game)
{
  QJsonValue jsonValue = jsonObj.value("total_rating");
  if(jsonValue != QJsonValue::Undefined) {
    double rating = jsonValue.toDouble();
    if(rating != 0.0) {
      game.rating = QString::number(rating / 100.0);
    }
  }
}

void Igdb::getCover(GameEntry &game)
{
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
    manager.request(coverUrl);
    q.exec();
    QImage image;
    if(image.loadFromData(manager.getData())) {
      game.coverData = image;
    }
  }
}

void Igdb::getScreenshot(GameEntry &game)
{
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
  manager.request(jsonScreenshots.at(chosen).toObject().value("image").toString());
  q.exec();
  QImage image;
  if(image.loadFromData(manager.getData())) {
    game.screenshotData = image;
  }
}

void Igdb::runPasses(QList<GameEntry> &gameEntries, const QFileInfo &info, QString &output, QString &, QString &debug)
{
  QString searchName = getSearchName(info);
  QString searchNameOrig = searchName;

  // searchName will be empty for files such as "[BIOS] Something.zip" and cause some scraping
  // modules to return EVERYTHING in their database. We DO NOT want this since it take ages
  // to parse it (15 minutes or more per entry) and it's faulty data anyways.
  if(searchName.isEmpty()) {
    return;
  }
  
  for(int pass = 1; pass <= 2; ++pass) {
    // Reset searchName for each pass
    searchName = searchNameOrig;
    output.append("\033[1;35mPass " + QString::number(pass) + "\033[0m ");
    switch(pass) {
    case 1:
      getSearchResults(gameEntries, searchName, config->platform);
      break;
    case 2:
      if(searchName.indexOf(":") != -1 || searchName.indexOf("-")) {
	// Remove everything after a dash or a colon for more results
	searchName = searchName.left(searchName.indexOf(":")).simplified();
	searchName = searchName.left(searchName.indexOf("-")).simplified();
	getSearchResults(gameEntries, searchName, config->platform);
	break;
      }
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

void Igdb::loadMaps()
{
  genreMap[11] = "Real Time Strategy (RTS)";
  genreMap[9] = "Puzzle";
  genreMap[25] = "Hack and slash/Beat 'em up";
  genreMap[30] = "Pinball";
  genreMap[12] = "Role-playing (RPG)";
  genreMap[31] = "Adventure";
  genreMap[5] = "Shooter";
  genreMap[7] = "Music";
  genreMap[2] = "Point-and-click";
  genreMap[13] = "Simulator";
  genreMap[26] = "Quiz/Trivia";
  genreMap[33] = "Arcade";
  genreMap[24] = "Tactical";
  genreMap[4] = "Fighting";
  genreMap[15] = "Strategy";
  genreMap[16] = "Turn-based strategy (TBS)";
  genreMap[8] = "Platform";
  genreMap[10] = "Racing";
  genreMap[14] = "Sport";
  genreMap[32] = "Indie";
  
 {
    QFile jsonFile("/usr/local/etc/skyscraper/igdb_platforms.json");
    if(jsonFile.open(QIODevice::ReadOnly)) {
      QJsonArray jsonPlatforms = QJsonDocument::fromJson(jsonFile.readAll()).array();
      foreach(const QJsonValue &jsonPlatform, jsonPlatforms) {
	platformMap[jsonPlatform.toObject().value("id").toInt()] =
	  jsonPlatform.toObject().value("slug").toString();
      }
      jsonFile.close();
    }
  }
}
