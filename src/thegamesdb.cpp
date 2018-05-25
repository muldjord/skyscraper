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
  fetchOrder.append(AGES);
  fetchOrder.append(PUBLISHER);
  fetchOrder.append(DEVELOPER);
  fetchOrder.append(RATING);
  fetchOrder.append(COVER);
  fetchOrder.append(SCREENSHOT);
  fetchOrder.append(WHEEL);
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
    // Remove anything at the end with a parentheses. 'thegamesdb' has a habbit of adding
    // for instance '(1993)' to the name.
    game.title = game.title.left(game.title.indexOf("(")).simplified();
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
    printf("It would seem that TheGamesDb is currently offline or having difficulties. You could try setting another scraping module using '-s' or wait until TheGamesDb is back in service.\nNow quitting...\n");
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
  game.releaseDate = xmlGame.firstChildElement("ReleaseDate").text();
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

void TheGamesDb::getAges(GameEntry &game)
{
  game.ages = xmlGame.firstChildElement("PEGI").text();
  if(!game.ages.isEmpty())
    return;
  game.ages = xmlGame.firstChildElement("ESRB").text();
  if(!game.ages.isEmpty())
    return;
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
      QImage image;
      if(image.loadFromData(manager.getData())) {
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
    QImage image;
    if(image.loadFromData(manager.getData())) {
      game.screenshotData = image;
    }
  }
}

void TheGamesDb::getWheel(GameEntry &game)
{
  QDomNodeList xmlImages = xmlGame.firstChildElement("Images").elementsByTagName("banner");
  if(xmlImages.isEmpty()) {
    // No wheel/banner, just return
    return;
  }
  QString wheelUrl = baseUrl + "/banners/" + xmlImages.at(0).toElement().text();
  manager.request(wheelUrl);
  q.exec();
  QImage image;
  if(image.loadFromData(manager.getData())) {
    game.wheelData = image;
  }
}
