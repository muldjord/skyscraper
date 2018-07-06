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

#include <QJsonArray>

#include "thegamesdb.h"
#include "strtools.h"

TheGamesDb::TheGamesDb()
{
  connect(&manager, &NetComm::dataReady, &q, &QEventLoop::quit);

  baseUrl = "https://api.thegamesdb.net";

  searchUrlPre = "https://api.thegamesdb.net/Games/ByGameName?name=";
  
  fetchOrder.append(RELEASEDATE);
  fetchOrder.append(DESCRIPTION);
  fetchOrder.append(TAGS);
  fetchOrder.append(PLAYERS);
  fetchOrder.append(AGES);
  fetchOrder.append(PUBLISHER);
  fetchOrder.append(DEVELOPER);
  //fetchOrder.append(RATING);
  fetchOrder.append(COVER);
  fetchOrder.append(SCREENSHOT);
  fetchOrder.append(WHEEL);
}

void TheGamesDb::getSearchResults(QList<GameEntry> &gameEntries,
				  QString searchName, QString platform)
{
  manager.request(searchUrlPre + searchName + "&apikey=" + config->userCreds);
  q.exec();
  data = manager.getData();

  jsonDoc = QJsonDocument::fromJson(data);
  if(jsonDoc.isEmpty()) {
    return;
  }
  if(jsonDoc.object().value("status").toString() != "Success") {
    return;
  }
  if(jsonDoc.object().value("data").toObject().value("count").toInt() < 1) {
    return;
  }
  if(jsonDoc.object().value("remaining_monthly_allowance").toInt() +
     jsonDoc.object().value("extra_allowance").toInt() < 1) {
    printf("You've exceeded your monthly request quota from 'thegamesdb', now quitting...\n");
    exit(1);
  }

  QJsonArray jsonGames = jsonDoc.object().value("data").toObject().value("games").toArray();

  while(!jsonGames.isEmpty()) {
    printf("HERE!\n");
    QJsonObject jsonGame = jsonGames.first().toObject();
    
    GameEntry game;
    // https://api.thegamesdb.net/Games/ByGameID?id=88&apikey=XXX&fields=game_title,players,release_date,developer,publisher,genres,overview,rating,platform
    game.id = QString::number(jsonGame.value("id").toInt());
    printf("ID='%s'\n", game.id.toStdString().c_str());
    game.url = "https://api.thegamesdb.net/Games/ByGameID?id=" + game.id + "&apikey=" + config->userCreds + "&fields=game_title,players,release_date,developer,publisher,genres,overview,rating";
    game.title = jsonGame.value("game_title").toString();
    printf("TITLE='%s'\n", game.title.toStdString().c_str());
    // Remove anything at the end with a parentheses. 'thegamesdb' has a habit of adding
    // for instance '(1993)' to the name.
    game.title = game.title.left(game.title.indexOf("(")).simplified();
    game.platform = idToPlatform(jsonGame.value("platform").toString());
    if(platformMatch(game.platform, platform)) {
      gameEntries.append(game);
    }
    jsonGames.removeFirst();
  }
}

void TheGamesDb::getGameData(GameEntry &game)
{
  manager.request(game.url);
  printf("URL:\n%s\n", game.url.toStdString().c_str());
  q.exec();
  data = manager.getData();
  jsonDoc = QJsonDocument::fromJson(data);
  if(jsonDoc.isEmpty()) {
    printf("No returned json data, is 'thegamesdb' down? Now quitting...\n");
    exit(1);
  }
  if(jsonDoc.object().value("data").toObject().value("count").toInt() < 1) {
    printf("No returned json game document, is 'thegamesdb' down? Now quitting...\n");
    exit(1);
  }
  
  if(jsonDoc.object().value("remaining_monthly_allowance").toInt() +
     jsonDoc.object().value("extra_allowance").toInt() < 1) {
    printf("You've reached the limit of your monthly request quota from 'thegamesdb', please wait until next month to try again, now quitting...\n");
    exit(1);
  }

  jsonObj = jsonDoc.object().value("data").toObject().value("games").toObject().value("0").toObject();

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
    case AGES:
      getAges(game);
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

void TheGamesDb::getReleaseDate(GameEntry &game)
{
  game.releaseDate = jsonObj.value("release_date").toString();
}

void TheGamesDb::getDeveloper(GameEntry &game)
{
  game.developer = jsonObj.value("developer").toString();
}

void TheGamesDb::getPublisher(GameEntry &game)
{
  game.publisher = jsonObj.value("publisher").toString();
}

void TheGamesDb::getDescription(GameEntry &game)
{
  game.description = jsonObj.value("overview").toString();
}

void TheGamesDb::getPlayers(GameEntry &game)
{
  game.players = QString::number(jsonObj.value("players").toInt());
}

void TheGamesDb::getAges(GameEntry &game)
{
  game.ages = jsonObj.value("rating").toString();
}

void TheGamesDb::getTags(GameEntry &game)
{
  QStringList genres = jsonObj.value("genres").toString().split(',');
  foreach(QString genre, genres) {
    game.tags.append(idToGenre(genre) + ", ");
  }
  game.tags = game.tags.left(game.tags.length() - 2);
}

void TheGamesDb::getCover(GameEntry &game)
{
  // https://api.thegamesdb.net/Games/Boxart?games_id=88&apikey=XXX&filter=boxart,screenshot
  // https://cdn.thegamesdb.net/images/original/boxart/front/[gameid]-1.jpg
  manager.request("https://cdn.thegamesdb.net/images/original/boxart/front/" + game.id + "-1.jpg");
  q.exec();
  QImage image;
  if(image.loadFromData(manager.getData())) {
    game.coverData = image;
  }
}

void TheGamesDb::getScreenshot(GameEntry &game)
{
  // https://api.thegamesdb.net/Games/Boxart?games_id=88&apikey=XXX&filter=boxart,screenshot
  // https://cdn.thegamesdb.net/images/original/screenshots/[gameid]-1.jpg
  manager.request("https://cdn.thegamesdb.net/images/original/screenshots/" + game.id + "-1.jpg");
  q.exec();
  QImage image;
  if(image.loadFromData(manager.getData())) {
    game.coverData = image;
  }
}

QString TheGamesDb::idToPlatform(QString id)
{
  return "snes";
}

QString TheGamesDb::idToGenre(QString id)
{
  return "Action";
}
