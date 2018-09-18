/***************************************************************************
 *            importscraper.cpp
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

#include <QDir>

#include "importscraper.h"

ImportScraper::ImportScraper(Settings *config) : AbstractScraper(config)
{
  fetchOrder.append(TITLE);
  fetchOrder.append(DEVELOPER);
  fetchOrder.append(PUBLISHER);
  fetchOrder.append(COVER);
  fetchOrder.append(SCREENSHOT);
  fetchOrder.append(WHEEL);
  fetchOrder.append(MARQUEE);
  fetchOrder.append(VIDEO);
  fetchOrder.append(RELEASEDATE);
  fetchOrder.append(TAGS);
  fetchOrder.append(PLAYERS);
  fetchOrder.append(AGES);
  fetchOrder.append(RATING);
  fetchOrder.append(DESCRIPTION);
  
  covers = QDir(config->importFolder + "/covers", "*.*",
		QDir::Name, QDir::Files | QDir::NoDotAndDotDot).entryInfoList();
  screenshots = QDir(config->importFolder + "/screenshots", "*.*",
	       QDir::Name, QDir::Files | QDir::NoDotAndDotDot).entryInfoList();
  wheels = QDir(config->importFolder + "/wheels", "*.*",
		QDir::Name, QDir::Files | QDir::NoDotAndDotDot).entryInfoList();
  marquees = QDir(config->importFolder + "/marquees", "*.*",
		  QDir::Name, QDir::Files | QDir::NoDotAndDotDot).entryInfoList();
  videos = QDir(config->importFolder + "/videos", "*.*",
		QDir::Name, QDir::Files | QDir::NoDotAndDotDot).entryInfoList();
  textual = QDir(config->importFolder + "/textual", "*.*",
		 QDir::Name, QDir::Files | QDir::NoDotAndDotDot).entryInfoList();
  loadDefinitions();
}

void ImportScraper::getGameData(GameEntry &game)
{
  // Always reset game title at this point, to avoid saving the dummy title in localdb
  game.title = "";

  loadData();
  
  QByteArray dataOrig = data;
  for(int a = 0; a < fetchOrder.length(); ++a) {
    switch(fetchOrder.at(a)) {
    case TITLE:
      getTitle(game);
      data = dataOrig;
      break;
    case DESCRIPTION:
      getDescription(game);
      data = dataOrig;
      break;
    case DEVELOPER:
      getDeveloper(game);
      data = dataOrig;
      break;
    case PUBLISHER:
      getPublisher(game);
      data = dataOrig;
      break;
    case PLAYERS:
      getPlayers(game);
      data = dataOrig;
      break;
    case AGES:
      getAges(game);
      data = dataOrig;
      break;
    case RATING:
      getRating(game);
      data = dataOrig;
      break;
    case TAGS:
      getTags(game);
      data = dataOrig;
      break;
    case RELEASEDATE:
      getReleaseDate(game);
      data = dataOrig;
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

void ImportScraper::runPasses(QList<GameEntry> &gameEntries, const QFileInfo &info, QString &, QString &)
{
  data = "";
  textualFile = "";
  screenshotFile = "";
  coverFile = "";
  wheelFile = "";
  marqueeFile = "";
  videoFile = "";
  GameEntry game;
  bool textualFound = checkType(info.completeBaseName(), textual, textualFile);
  bool screenshotFound = checkType(info.completeBaseName(), screenshots, screenshotFile);
  bool coverFound = checkType(info.completeBaseName(), covers, coverFile);
  bool wheelFound = checkType(info.completeBaseName(), wheels, wheelFile);
  bool marqueeFound = checkType(info.completeBaseName(), marquees, marqueeFile);
  bool videoFound = checkType(info.completeBaseName(), videos, videoFile);
  if(textualFound || screenshotFound || coverFound || wheelFound || marqueeFound || videoFound) {
    game.title = info.completeBaseName();
    game.platform = config->platform;
    gameEntries.append(game);
  }
}

QString ImportScraper::getCompareTitle(QFileInfo info, QString &, QString &)
{
  return info.completeBaseName();
}

void ImportScraper::getCover(GameEntry &game)
{
  if(!coverFile.isEmpty()) {
    QImage image(coverFile);
    if(!image.isNull()) {
      game.coverData = image;
    }
  }
}

void ImportScraper::getScreenshot(GameEntry &game)
{
  if(!screenshotFile.isEmpty()) {
    QImage image(screenshotFile);
    if(!image.isNull()) {
      game.screenshotData = image;
    }
  }
}

void ImportScraper::getWheel(GameEntry &game)
{
  if(!wheelFile.isEmpty()) {
    QImage image(wheelFile);
    if(!image.isNull()) {
      game.wheelData = image;
    }
  }
}

void ImportScraper::getMarquee(GameEntry &game)
{
  if(!marqueeFile.isEmpty()) {
    QImage image(marqueeFile);
    if(!image.isNull()) {
      game.marqueeData = image;
    }
  }
}

void ImportScraper::getVideo(GameEntry &game)
{
  if(!videoFile.isEmpty()) {
    QFile f(videoFile);
    if(f.open(QIODevice::ReadOnly)) {
      QFileInfo i(videoFile);
      game.videoData = f.readAll();
      game.videoFormat = i.suffix();
      f.close();
    }
  }
}

void ImportScraper::getTitle(GameEntry &game)
{
  if(titlePre.isEmpty()) {
    return;
  }
  foreach(QString nom, titlePre) {
    if(!checkNom(nom)) {
      return;
    }
  }
  foreach(QString nom, titlePre) {
    nomNom(nom);
  }
  game.title = data.left(data.indexOf(titlePost)).simplified();
}

void ImportScraper::loadData()
{
  if(!textualFile.isEmpty()) {
    QFile f(textualFile);
    if(f.open(QIODevice::ReadOnly)) {
      data = f.readAll();
      f.close();
    }
  }
}

bool ImportScraper::loadDefinitions()
{
  // Check for textual resource file
  QFile defFile(config->importFolder + "/definitions.dat");
  if(defFile.open(QIODevice::ReadOnly)) {
    while(!defFile.atEnd()) {
      QString line(defFile.readLine());
      checkForTag(titlePre, titlePost, titleTag, line);
      checkForTag(publisherPre, publisherPost, publisherTag, line);
      checkForTag(developerPre, developerPost, developerTag, line);
      checkForTag(playersPre, playersPost, playersTag, line);
      checkForTag(agesPre, agesPost, agesTag, line);
      checkForTag(ratingPre, ratingPost, ratingTag, line);
      checkForTag(tagsPre, tagsPost, tagsTag, line);
      checkForTag(releaseDatePre, releaseDatePost, releaseDateTag, line);
      checkForTag(descriptionPre, descriptionPost, descriptionTag, line);
    }
    defFile.close();
    return true;
  }
  return false;
}

void ImportScraper::checkForTag(QList<QString> &pre, QString &post, QString &tag, QString &line)
{
  if(line.indexOf(tag) != -1 && line.indexOf(tag) != 0) {
    pre.append(line.left(line.indexOf(tag)));
    post = line.right(line.length() - (line.indexOf(tag) + tag.length()));
  }
}

bool ImportScraper::checkType(QString baseName, QList<QFileInfo> &infos, QString &inputFile)
{
  foreach(QFileInfo i, infos) {
    if(i.completeBaseName() == baseName) {
      inputFile = i.absoluteFilePath();
      return true;
    }
  }
  return false;
}
