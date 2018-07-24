/***************************************************************************
 *            screenscraper.cpp
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

#include <QFileInfo>

#include "screenscraper.h"
#include "strtools.h"

ScreenScraper::ScreenScraper()
{
  connect(&manager, &NetComm::dataReady, &q, &QEventLoop::quit);

  baseUrl = "http://www.screenscraper.fr";

  fetchOrder.append(PUBLISHER);
  fetchOrder.append(DEVELOPER);
  fetchOrder.append(PLAYERS);
  fetchOrder.append(AGES);
  fetchOrder.append(RATING);
  fetchOrder.append(DESCRIPTION);
  fetchOrder.append(RELEASEDATE);
  fetchOrder.append(TAGS);
  fetchOrder.append(SCREENSHOT);
  fetchOrder.append(COVER);
  fetchOrder.append(WHEEL);
  fetchOrder.append(MARQUEE);
  fetchOrder.append(VIDEO);
}

void ScreenScraper::getSearchResults(QList<GameEntry> &gameEntries,
				     QString searchName, QString platform)
{
  /*
  if(user.isEmpty() || password.isEmpty()) {
    printf("'screenscraper' requires a userID and password to function. Please use '-u' to set them. Check '--help' for more information.\n");
    exit(1);
  }
  */
  QString gameUrl = "https://www.screenscraper.fr/api2/jeuInfos.php?devid=muldjord&devpassword=" + StrTools::unMagic("204;198;236;130;203;181;203;126;191;167;200;198;192;228;169;156") + "&softname=skyscraper" VERSION "&ssid=" + config->user + "&sspassword=" + config->password + "&output=xml&" + searchName;
  manager.request(gameUrl);
  q.exec();
  data = manager.getData();

  if(data.indexOf("Erreur") != -1) {
    return;
  }
  
  GameEntry game;

  // Check if xml is valid. If not, skip
  if(!xmlDoc.setContent(data)) {
    return;
  }

  game.title = getXmlText("nom", REGION);
  
  // 'screenscraper' sometimes returns a faulty result with the following name. If we get this
  // result, DON'T use it. It will provide faulty data for localdb
  if(game.title.toLower().indexOf("hack") != -1 && game.title.toLower().indexOf("link") != -1) {
    return;
  }

  // If title still unset, no acceptable rom was found, so return with no results
  if(game.title.isNull()) {
    return;
  }

  game.url = gameUrl;
  game.platform = xmlDoc.elementsByTagName("systeme").at(0).toElement().text();

  if(platformMatch(game.platform, platform)) {
    gameEntries.append(game);
  }
}

void ScreenScraper::getGameData(GameEntry &game)
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

void ScreenScraper::getReleaseDate(GameEntry &game)
{
  game.releaseDate = getXmlText("date", REGION);
}

void ScreenScraper::getDeveloper(GameEntry &game)
{
  game.developer = xmlDoc.elementsByTagName("developpeur").at(0).toElement().text();
}

void ScreenScraper::getPublisher(GameEntry &game)
{
  game.publisher = xmlDoc.elementsByTagName("editeur").at(0).toElement().text();
}

void ScreenScraper::getDescription(GameEntry &game)
{
  game.description = getXmlText("synopsis", LANGUE);
}

void ScreenScraper::getPlayers(GameEntry &game)
{
  game.players = xmlDoc.elementsByTagName("joueurs").at(0).toElement().text();
}

void ScreenScraper::getAges(GameEntry &game)
{
  QDomNodeList xmlNodes = xmlDoc.elementsByTagName("classification");

  if(xmlNodes.isEmpty())
    return;
  
  // First look for PEGI
  for(int a = 0; a < xmlNodes.length(); ++a) {
    QDomElement elem = xmlNodes.at(a).toElement();
    if(elem.attribute("type") == "PEGI") {
      game.ages = xmlNodes.at(a).toElement().text();
      return;
    }
  }
  // Then look for ESRB
  for(int a = 0; a < xmlNodes.length(); ++a) {
    QDomElement elem = xmlNodes.at(a).toElement();
    if(elem.attribute("type") == "ESRB") {
      game.ages = xmlNodes.at(a).toElement().text();
      return;
    }
  }
  // Then look for SS
  for(int a = 0; a < xmlNodes.length(); ++a) {
    QDomElement elem = xmlNodes.at(a).toElement();
    if(elem.attribute("type") == "SS") {
      game.ages = xmlNodes.at(a).toElement().text();
      return;
    }
  }
}

void ScreenScraper::getRating(GameEntry &game)
{
  game.rating = xmlDoc.elementsByTagName("note").at(0).toElement().text();
  bool toDoubleOk = false;
  double rating = game.rating.toDouble(&toDoubleOk);
  if(toDoubleOk) {
    game.rating = QString::number(rating / 20.0);
  } else {
    game.rating = "";
  }
}

void ScreenScraper::getTags(GameEntry &game)
{
  QDomNodeList xmlNodes = xmlDoc.elementsByTagName("genre");

  if(xmlNodes.isEmpty())
    return;
  
  for(int a = 0; a < xmlNodes.length(); ++a) {
    QDomElement elem = xmlNodes.at(a).toElement();
    if(elem.attribute("langue") == config->lang) {
      game.tags.append(xmlNodes.at(a).toElement().text() + ", ");
    }
  }
  
  game.tags = game.tags.left(game.tags.length() - 2);
}

void ScreenScraper::getCover(GameEntry &game)
{
  QString url = getXmlText("media", REGION, "box-2D");
  if(!url.isEmpty()) {
    manager.request(url);
    q.exec();
    QImage image;
    if(image.loadFromData(manager.getData())) {
      game.coverData = image;
    }
  }
}

void ScreenScraper::getScreenshot(GameEntry &game)
{
  QString url = getXmlText("media", NONE, "ss");
  if(!url.isEmpty()) {
    manager.request(url);
    q.exec();
    QImage image;
    if(image.loadFromData(manager.getData())) {
      game.screenshotData = image;
    }
  }
}

void ScreenScraper::getWheel(GameEntry &game)
{
  QString url = getXmlText("media", REGION, "wheel");
  if(url.isEmpty()) {
    url = getXmlText("media", REGION, "wheel-hd");
  }
  if(!url.isEmpty()) {
    manager.request(url);
    q.exec();
    QImage image;
    if(image.loadFromData(manager.getData())) {
      game.wheelData = image;
    }
  }
}

void ScreenScraper::getMarquee(GameEntry &game)
{
  QString url = getXmlText("media", REGION, "screenmarquee");
  if(!url.isEmpty()) {
    manager.request(url);
    q.exec();
    QImage image;
    if(image.loadFromData(manager.getData())) {
      game.marqueeData = image;
    }
  }
}

void ScreenScraper::getVideo(GameEntry &game)
{
  QString url = getXmlText("media", NONE, "video");
  if(!url.isEmpty()) {
    manager.request(url);
    q.exec();
    game.videoData = manager.getData();
    // Make sure recieved data is actually a video file
    QByteArray contentType = manager.getContentType();
    if(contentType.indexOf("video/") != -1 && game.videoData.size() > 4096) {
      game.videoFormat = contentType.mid(contentType.indexOf("/") + 1,
					 contentType.length() - contentType.indexOf("/") + 1);
    } else {
      game.videoData = "";
    } 
  }
}

void ScreenScraper::runPasses(QList<GameEntry> &gameEntries, const QFileInfo &info, QString &output, QString &, QString &debug)
{
  QList<QString> hashList = getHashes(info);

  for(int pass = 1; pass <= 3; ++pass) {
    output.append("\033[1;35mPass " + QString::number(pass) + "\033[0m ");
    switch(pass) {
    case 1:
      if(info.size() != 0) {
	debug.append("Tried with sha1: " + hashList.at(2) + "\n");
	getSearchResults(gameEntries, "sha1=" + hashList.at(2), config->platform);
      }
      break;
    case 2:
      if(info.size() != 0) {
	debug.append("Tried with md5: " + hashList.at(1) + "\n");
	getSearchResults(gameEntries, "md5=" + hashList.at(1), config->platform);
      }
      break;
    case 3:
      debug.append("Tried with name: " + hashList.at(0) + "\n");
      getSearchResults(gameEntries, "romnom=" + hashList.at(0), config->platform);
      break;
    default:
      ;
    }
    debug.append("Platform: " + config->platform + "\n");
    if(!gameEntries.isEmpty()) {
      break;
    }
  }
}

QList<QString> ScreenScraper::getHashes(const QFileInfo &info)
{
  QList<QString> hashList;
  QCryptographicHash md5(QCryptographicHash::Md5);
  QCryptographicHash sha1(QCryptographicHash::Sha1);
  QFile romFile(info.absoluteFilePath());
  romFile.open(QIODevice::ReadOnly);
  while(!romFile.atEnd()) {
    QByteArray dataSeg = romFile.read(1024);
    md5.addData(dataSeg);
    sha1.addData(dataSeg);
  }
  romFile.close();
  hashList.append(QUrl::toPercentEncoding(info.fileName()));
  hashList.append(md5.result().toHex().toUpper());
  hashList.append(sha1.result().toHex().toUpper());

  return hashList;
}

QString ScreenScraper::getXmlText(QString node, int attr, QString type)
{
  QDomNodeList xmlNodes = xmlDoc.elementsByTagName(node);
  if(attr == NONE) {
    for(int a = 0; a < xmlNodes.length(); ++a) {
      QDomElement elem = xmlNodes.at(a).toElement();
      if(type != "" && elem.attribute("type") == type) {
	return elem.text();
      }
    }
  } else if(attr == REGION) {
    foreach(QString region, regionPrios) {
      for(int a = 0; a < xmlNodes.length(); ++a) {
	QDomElement elem = xmlNodes.at(a).toElement();
	if(type != "" && elem.attribute("type") != type)
	  continue;
	if(elem.attribute("region") == region) {
	  return elem.text();
	}
      }
    }
  } else if(attr == LANGUE) {
    foreach(QString lang, langPrios) {
      for(int a = 0; a < xmlNodes.length(); ++a) {
	QDomElement elem = xmlNodes.at(a).toElement();
	if(type != "" && elem.attribute("type") != type)
	  continue;
	if(elem.attribute("langue") == lang) {
	  return elem.text();
	}
      }
    }
  }
  return QString();
}
  
