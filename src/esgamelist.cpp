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

ESGameList::ESGameList(Settings *config) :
  AbstractScraper(config) {
  basePath = QDir::homePath() + "/RetroPie/roms/" + config->platform + "/";
  gamelistXml = basePath + "gamelist.xml";
  QFile file(gamelistXml);
  file.open(QIODevice::ReadOnly);
  doc.setContent(&file);
}

void ESGameList::getSearchResults(QList<GameEntry> &gameEntries,
				  QString searchName, QString platform) {
  QDomNodeList games = doc.elementsByTagName("game");
  for (int i = 0; i < games.size(); i++) {
    const QDomNode& gamenode = games.item(i);
    const QDomElement& pathnode = gamenode.firstChildElement("path");

    if (pathnode.text().endsWith(searchName)) {
      GameEntry game;
      game.id = gamenode.attributes().namedItem("id").nodeValue();
      game.title = gamenode.firstChildElement("name").text();
      game.releaseDate = gamenode.firstChildElement("releasedate").text();
      game.publisher = gamenode.firstChildElement("publisher").text();
      game.developer = gamenode.firstChildElement("developer").text();
      game.players = gamenode.firstChildElement("players").text();
      game.rating = gamenode.firstChildElement("rating").text();
      game.description = gamenode.firstChildElement("desc").text();
      game.eSFavorite = gamenode.firstChildElement("favorite").text();
      game.eSHidden = gamenode.firstChildElement("hidden").text();
      game.eSPlayCount = gamenode.firstChildElement("playcount").text();
      game.eSLastPlayed = gamenode.firstChildElement("lastplayed").text();
      game.eSKidGame = gamenode.firstChildElement("kidgame").text();
      game.eSSortName = gamenode.firstChildElement("sortname").text();
      game.marqueeData = loadImageData(gamenode, "marquee");
      game.coverData = loadImageData(gamenode, "cover");
      game.screenshotData = loadImageData(gamenode, "image");
      game.tags = gamenode.firstChildElement("genre").text();
      if (config->videos) {
	QString videoFilePath = loadVideoData(gamenode);
	if (!videoFilePath.isEmpty()) {
	  QFile videoFile(videoFilePath);
	  if (videoFile.open(QIODevice::ReadOnly)) {
	    game.videoData = videoFile.readAll();
	    if (game.videoData.size() > 4096) {
	      game.videoFormat = QFileInfo(videoFilePath).suffix();
	    }
	  }
	}
      }
      game.platform = platform;
      game.found = true;

      gameEntries.append(game);
      return;
    }
  }
}

QImage ESGameList::loadImageData(const QDomNode& gamenode, QString tag) {
  const QString& name = checkName(gamenode.firstChildElement(tag).text());
  QImage image;
  if (image.load(name)) {
    return image;
  }
  return QImage();
}

QString ESGameList::checkName(QString value) {
  if (value.isEmpty()) {
    return "";
  }
  QFileInfo file(value);
  if (file.isAbsolute()) {
    return value;
  }
  return basePath + value;
}

QString ESGameList::loadVideoData(const QDomNode& gamenode) {
  return checkName(gamenode.firstChildElement("video").text());
}

void ESGameList::getGameData(GameEntry &) {
}

QList<QString> ESGameList::getSearchNames(const QFileInfo &info) {
  QList<QString> searchNames;
  searchNames.append(info.fileName());
  return searchNames;
}
