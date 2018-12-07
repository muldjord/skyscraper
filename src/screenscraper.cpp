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
#include <QProcess>

#include "screenscraper.h"
#include "strtools.h"
#include "crc32.h"

ScreenScraper::ScreenScraper(Settings *config) : AbstractScraper(config)
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
    if(elem.attribute("langue") == langPrios.first()) {
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
  QString url = getXmlText("media", REGION, "wheel;wheel-hd");
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

QList<QString> ScreenScraper::getSearchNames(const QFileInfo &info)
{
  // Reset regions as this is a new rom
  romRegions = regionPrios;
  if(info.fileName().indexOf("(") != -1 && config->region.isEmpty()) {
    QString regionString = info.fileName().toLower().mid(info.fileName().indexOf("("), info.fileName().length());
    if(regionString.indexOf("europe") != -1) {
      romRegions.prepend("eu");
    }
    if(regionString.indexOf("usa") != -1) {
      romRegions.prepend("us");
    }
    if(regionString.indexOf("world") != -1) {
      romRegions.prepend("wor");
    }
    if(regionString.indexOf("japan") != -1) {
      romRegions.prepend("jp");
    }
  }
  
  QList<QString> hashList;
  QCryptographicHash md5(QCryptographicHash::Md5);
  QCryptographicHash sha1(QCryptographicHash::Sha1);
  Crc32 crc;
  crc.initInstance(1);
  
  bool unpack = config->unpack;
  
  if(unpack && (info.suffix() == "7z" || info.suffix() == "zip") && info.size() < 20480000) {
    // For 7z (7z, zip) unpacked file reading
    {
      QProcess decProc;
      decProc.setReadChannel(QProcess::StandardOutput);
      
      decProc.start("7z l -so \"" + info.absoluteFilePath() + "\"");
      if(decProc.waitForFinished(30000)) {
	if(decProc.exitStatus() != QProcess::NormalExit) {
	  printf("Getting file list from compressed file failed, falling back...\n");
	  unpack = false;
	} else if(decProc.readAllStandardOutput().indexOf(" 1 files") == -1) {
	  printf("Compressed file contains more than 1 file, falling back...\n");
	  unpack = false;
	}
      } else {
	printf("Getting file list from compressed file timed out or failed, falling back...\n");
	unpack = false;
      }
    }
    
    if(unpack) {
      QProcess decProc;
      decProc.setReadChannel(QProcess::StandardOutput);
      
      decProc.start("7z x -so \"" + info.absoluteFilePath() + "\"");
      if(decProc.waitForFinished(30000)) {
	if(decProc.exitStatus() == QProcess::NormalExit) {
	  QByteArray allData = decProc.readAllStandardOutput();
	  md5.addData(allData);
	  sha1.addData(allData);
	  crc.pushData(1, allData.data(), allData.length());
	} else {
	  printf("Something went wrong when decompressing file to stdout, falling back...\n");
	  unpack = false;
	}
      } else {
	printf("Decompression process timed out or failed, falling back...\n");
	unpack = false;
      }
    }
  }

  if(!unpack) {
    // For normal file reading
    QFile romFile(info.absoluteFilePath());
    romFile.open(QIODevice::ReadOnly);
    while(!romFile.atEnd()) {
      
      QByteArray dataSeg = romFile.read(1024);
      md5.addData(dataSeg);
      sha1.addData(dataSeg);
      crc.pushData(1, dataSeg.data(), dataSeg.length());
    }
    romFile.close();
  }

  hashList.append(QUrl::toPercentEncoding(info.fileName()));
  hashList.append(QString::number(crc.releaseInstance(1), 16).toUpper());
  hashList.append(md5.result().toHex().toUpper());
  hashList.append(sha1.result().toHex().toUpper());

  QList<QString> searchNames;
  if(info.size() != 0) {
    searchNames.append("romnom=" + hashList.at(0) + "&crc=" + hashList.at(1) + "&md5=" + hashList.at(2) + "&romtaille=" + QString::number(info.size()));
    searchNames.append("crc=" + hashList.at(1));
    searchNames.append("md5=" + hashList.at(2));
    searchNames.append("sha1=" + hashList.at(3));
  }
  searchNames.append("romnom=" + hashList.at(0));

  return searchNames;
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
    QList<QString> types = type.split(";");
    foreach(QString region, romRegions) {
      for(int a = 0; a < xmlNodes.length(); ++a) {
	QDomElement elem = xmlNodes.at(a).toElement();
	bool typeMatch = false;
	if(type.isEmpty()) {
	  typeMatch = true;
	} else {
	  foreach(QString currentType, types) {
	    if(elem.attribute("type") == currentType) {
	      typeMatch = true;
	      break;
	    }
	  }
	}
	if(!typeMatch)
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
  
