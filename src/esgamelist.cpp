/***************************************************************************
 *            esgamelist.cpp
 *
 *  Mon Dec 17 08:00:00 CEST 2018
 *  Copyright 2018 Lars Muldjord & Martin Gerhardy
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

#include "nametools.h"
#include "esgamelist.h"

#include <QDir>

ESGameList::ESGameList(Settings *config) : AbstractScraper(config) {
  basePath = QDir::homePath() + "/RetroPie/roms/" + config->platform + "/";
  gameListXml = basePath + "gamelist.xml";
  QFile gameListFile(gameListXml);
  if(gameListFile.open(QIODevice::ReadOnly)) {
    xmlDoc.setContent(&gameListFile);
    gameListFile.close();
  }
  games = xmlDoc.elementsByTagName("game");
}

void ESGameList::getSearchResults(QList<GameEntry> &gameEntries,
				  QString searchName, QString platform) {
  gameNode = QDomNode();
  
  if(games.isEmpty())
    return;
  
  for(int i = 0; i < games.size(); ++i) {
    // Find <game> where last part of <path> matches file name
    if(games.item(i).firstChildElement("path").text().endsWith(searchName)) {
      gameNode = games.item(i);
      GameEntry game;
      game.title = gameNode.firstChildElement("name").text();
      game.platform = platform;
      gameEntries.append(game);
      break;
    }
  }
}

void ESGameList::getGameData(GameEntry &game) {
  if(games.isEmpty())
    return;
  
  game.releaseDate = gameNode.firstChildElement("releasedate").text();
  game.publisher = gameNode.firstChildElement("publisher").text();
  game.developer = gameNode.firstChildElement("developer").text();
  game.players = gameNode.firstChildElement("players").text();
  game.rating = gameNode.firstChildElement("rating").text();
  game.tags = gameNode.firstChildElement("genre").text();
  game.description = gameNode.firstChildElement("desc").text();
  game.marqueeData = loadImageData(gameNode.firstChildElement("marquee").text());
  game.coverData = loadImageData(gameNode.firstChildElement("cover").text());
  game.screenshotData = loadImageData(gameNode.firstChildElement("image").text());
  if(config->videos) {
    loadVideoData(game, gameNode.firstChildElement("video").text());
  }
}

QImage ESGameList::loadImageData(const QString fileName) {
  QString absoluteFileName = getAbsoluteFileName(fileName);
  QImage image;
  if(image.load(absoluteFileName)) {
    return image;
  }
  return QImage();
}

void ESGameList::loadVideoData(GameEntry &game, const QString fileName) {
  QString absoluteFileName = getAbsoluteFileName(fileName);
  if(absoluteFileName.isEmpty())
    return;

  QFile videoFile(absoluteFileName);
  if(videoFile.open(QIODevice::ReadOnly)) {
    game.videoData = videoFile.readAll();
    if(game.videoData.size() > 4096) {
      game.videoFormat = QFileInfo(absoluteFileName).suffix();
    }
    videoFile.close();
  }
}

QString ESGameList::getAbsoluteFileName(const QString fileName) {
  if(QFileInfo::exists(fileName)) {
    return QFileInfo(fileName).absoluteFilePath();
  }
  if(QFileInfo::exists(basePath + fileName)) {
    return QFileInfo(basePath + fileName).absoluteFilePath();
  }
  return "";
}

QList<QString> ESGameList::getSearchNames(const QFileInfo &info) {
  QList<QString> searchNames;
  searchNames.append(info.fileName());
  return searchNames;
}
