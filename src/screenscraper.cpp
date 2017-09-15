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
  fetchOrder.append(DESCRIPTION);
  fetchOrder.append(RELEASEDATE);
  fetchOrder.append(TAGS);
  fetchOrder.append(SCREENSHOT);
  fetchOrder.append(COVER);
  fetchOrder.append(VIDEO);
}

void ScreenScraper::getSearchResults(QList<GameEntry> &gameEntries,
				     QString searchName, QString platform)
{
  /*
  if(user.isEmpty() || password.isEmpty()) {
    qDebug("'screenscraper' requires a userID and password to function. Please use '-u' to set them. Check '--help' for more information.\n");
    exit(1);
  }
  */
  QString gameUrl = "https://www.screenscraper.fr/api/jeuInfos.php?devid=muldjord&devpassword=" + StrTools::unMagic("204;198;236;130;203;181;203;126;191;167;200;198;192;228;169;156") + "&softname=skyscraper" VERSION "&output=xml&" + searchName;

  manager.request(gameUrl);
  q.exec();
  data = manager.getData();

  if(data.indexOf("Erreur") != -1) {
    return;
  }
  
  region = "wor";
  lang = "en";
  GameEntry game;

  xmlDoc.setContent(data);

  QDomNode xmlRegionNoms = xmlDoc.elementsByTagName("noms").at(0);

  if(!xmlRegionNoms.firstChildElement("nom_" + region).isNull()) {
    game.title = xmlRegionNoms.firstChildElement("nom_" + region).text();
  } else if(!xmlDoc.elementsByTagName("nom").at(0).toElement().isNull()) {
    game.title = xmlDoc.elementsByTagName("nom").at(0).toElement().text();
  } else if(!xmlRegionNoms.firstChildElement("nom_wor").isNull()) {
    game.title = xmlRegionNoms.firstChildElement("nom_wor").text();
  } else if(!xmlRegionNoms.firstChildElement("nom_us").isNull()) {
    game.title = xmlRegionNoms.firstChildElement("nom_us").text();
  }

  // If title still unset, no acceptable rom was found, so return with no results
  if(game.title.isNull()) {
    return;
  }

  game.url = gameUrl;
  
  game.platform = xmlDoc.elementsByTagName("systemenom").at(0).toElement().text();
  
  if(game.platform.toLower() == actualPlatform(platform)) {
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
  QDomNode xmlNode = xmlDoc.elementsByTagName("dates").at(0);
  QString releaseDate;
  if(!xmlNode.firstChildElement("date_" + region).isNull()) {
    releaseDate = xmlNode.firstChildElement("date_" + region).text();
  } else if(!xmlNode.firstChildElement("date_wor").isNull()) {
    releaseDate = xmlNode.firstChildElement("date_wor").text();
  } else if(!xmlNode.firstChildElement("date_us").isNull()) {
    releaseDate = xmlNode.firstChildElement("date_us").text();
  }

  if(releaseDate.length() == 4 && releaseDate.toInt() != 0) {
    game.releaseDate = QDate::fromString(releaseDate, "yyyy").toString("yyyyMMdd");
  } else if(releaseDate.length() == 7 &&
	    releaseDate.left(4).toInt() != 0 &&
	    releaseDate.left(4).toInt() < 3000 &&
	    releaseDate.mid(5, 2).toInt() != 0 &&
	    releaseDate.mid(5, 2).toInt() <= 12) {
    game.releaseDate = QDate::fromString(releaseDate, "yyyy-MM").toString("yyyyMMdd");
  } else if(releaseDate.length() == 10 &&
	    releaseDate.left(4).toInt() != 0 &&
	    releaseDate.left(4).toInt() < 3000 &&
	    releaseDate.mid(5, 2).toInt() != 0 &&
	    releaseDate.mid(5, 2).toInt() <= 12 &&
	    releaseDate.mid(8, 2).toInt() != 0 &&
	    releaseDate.mid(8, 2).toInt() <= 31) {
    game.releaseDate = QDate::fromString(releaseDate, "yyyy-MM-dd").toString("yyyyMMdd");
  } else if(releaseDate.length() == 10 &&
	    releaseDate.left(2).toInt() != 0 &&
	    releaseDate.left(2).toInt() <= 12 &&
	    releaseDate.mid(3, 2).toInt() != 0 &&
	    releaseDate.mid(3, 2).toInt() <= 31 &&
	    releaseDate.mid(6, 4).toInt() != 0 &&
	    releaseDate.mid(6, 4).toInt() < 3000) {
    game.releaseDate = QDate::fromString(releaseDate, "MM/dd/yyyy").toString("yyyyMMdd");
  } else if(releaseDate.length() == 9 &&
	    (releaseDate.left(3) == "Jan" ||
	     releaseDate.left(3) == "Feb" ||
	     releaseDate.left(3) == "Mar" ||
	     releaseDate.left(3) == "Apr" ||
	     releaseDate.left(3) == "May" ||
	     releaseDate.left(3) == "Jun" ||
	     releaseDate.left(3) == "Jul" ||
	     releaseDate.left(3) == "Aug" ||
	     releaseDate.left(3) == "Sep" ||
	     releaseDate.left(3) == "Oct" ||
	     releaseDate.left(3) == "Nov" ||
	     releaseDate.left(3) == "Dec") &&
	    releaseDate.mid(5, 4).toInt() != 0 &&
	    releaseDate.mid(5, 4).toInt() < 3000) {
    game.releaseDate = QDate::fromString(releaseDate, "MMM, yyyy").toString("yyyyMMdd");
  } else if(releaseDate.length() == 12 &&
	    (releaseDate.left(3) == "Jan" ||
	     releaseDate.left(3) == "Feb" ||
	     releaseDate.left(3) == "Mar" ||
	     releaseDate.left(3) == "Apr" ||
	     releaseDate.left(3) == "May" ||
	     releaseDate.left(3) == "Jun" ||
	     releaseDate.left(3) == "Jul" ||
	     releaseDate.left(3) == "Aug" ||
	     releaseDate.left(3) == "Sep" ||
	     releaseDate.left(3) == "Oct" ||
	     releaseDate.left(3) == "Nov" ||
	     releaseDate.left(3) == "Dec") &&
	    releaseDate.mid(4, 2).toInt() != 0 &&
	    releaseDate.mid(4, 2).toInt() <= 31 &&
	    releaseDate.mid(8, 4).toInt() != 0 &&
	    releaseDate.mid(8, 4).toInt() < 3000) {
    game.releaseDate = QDate::fromString(releaseDate, "MMM dd, yyyy").toString("yyyyMMdd");
  } else {
    game.releaseDate = releaseDate;
  }
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
  QDomNode xmlNode = xmlDoc.elementsByTagName("synopsis").at(0);
  if(!xmlNode.firstChildElement("synopsis_" + lang).isNull()) {
    game.description = xmlNode.firstChildElement("synopsis_" + lang).text();
  } else if(!xmlNode.firstChildElement("synopsis_en").isNull()) {
    game.description = xmlNode.firstChildElement("synopsis_en").text();
  }
}

void ScreenScraper::getPlayers(GameEntry &game)
{
  game.players = xmlDoc.elementsByTagName("joueurs").at(0).toElement().text();
}

void ScreenScraper::getTags(GameEntry &game)
{
  QDomNodeList xmlNodes = xmlDoc.elementsByTagName("genre_" + lang);

  for(int a = 0; a < xmlNodes.count(); ++a) {
    game.tags.append(xmlNodes.at(a).toElement().text() + ", ");
  }
  game.tags = game.tags.left(game.tags.length() - 2);
}

void ScreenScraper::getCover(GameEntry &game)
{
  QDomElement xmlElem;
  xmlElem = xmlDoc.elementsByTagName("media_box2d_" + region).at(0).toElement();
  if(xmlElem.isNull()) {
    xmlElem = xmlDoc.elementsByTagName("media_box2d_wor").at(0).toElement();
  }
  if(xmlElem.isNull()) {
    xmlElem = xmlDoc.elementsByTagName("media_box2d_eu").at(0).toElement();
  }
  if(xmlElem.isNull()) {
    xmlElem = xmlDoc.elementsByTagName("media_box2d_us").at(0).toElement();
  }
  if(!xmlElem.isNull()) {
    manager.request(xmlElem.text());
    q.exec();
    QImage image(QImage::fromData(manager.getData()));
    if(!image.isNull()) {
      game.coverData = image;
    }
  }
}

void ScreenScraper::getScreenshot(GameEntry &game)
{
  QDomElement xmlElem = xmlDoc.elementsByTagName("media_screenshot").at(0).toElement();
  if(!xmlElem.isNull()) {
    manager.request(xmlElem.text());
    q.exec();
    QImage image(QImage::fromData(manager.getData()));
    if(!image.isNull()) {
      game.screenshotData = image;
    }
  }
}

void ScreenScraper::getVideo(GameEntry &game)
{
  QDomElement xmlElem = xmlDoc.elementsByTagName("media_video").at(0).toElement();
  manager.request(xmlElem.text());
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

QString ScreenScraper::actualPlatform(const QString platform)
{
  if(platform == "aga") {
    return "amiga (aga)";
  } else if(platform == "cdtv") {
    return "amiga cdtv";
  } else if(platform == "cd32") {
    return "amiga cd32";
  } else if(platform == "atari2600") {
    return "atari 2600";
  } else if(platform == "atari5200") {
    return "atari 5200";
  } else if(platform == "atari7800") {
    return "atari 7800";
  } else if(platform == "mastersystem") {
    return "master system";
  } else if(platform == "gamegear") {
    return "game gear";
  } else if(platform == "gba") {
    return "game boy advance";
  } else if(platform == "c64") {
    return "c64";
  } else if(platform == "pcengine") {
    return "pc engine;pc engine cd-rom;pc engine supergrafx";
  } else if(platform == "psx") {
    return "playstation";
  } else if(platform == "snes") {
    return "super nintendo";
  } else if(platform == "zxspectrum") {
    return "zx spectrum";
  } else if(platform == "apple2") {
    return "apple ii";
  } else if(platform == "atarijaguar") {
    return "jaguar";
  } else if(platform == "neogeo") {
    return "neo-geo";
  } else if(platform == "n64") {
    return "nintendo 64";
  } else if(platform == "coleco") {
    return "colecovision";
  } else if(platform == "nds") {
    return "nintendo ds";
  } else if(platform == "gb") {
    return "game boy";
  } else if(platform == "gba") {
    return "game boy advance";
  } else if(platform == "gbc") {
    return "game boy color";
  } else if(platform == "segacd") {
    return "sega cd";
  }
  return platform;
}

void ScreenScraper::runPasses(QList<GameEntry> &gameEntries, const QFileInfo &info, QString &output, QString &)
{
  QString hashes = getHashes(info.absoluteFilePath());
  QList<QString> hashList = hashes.split(":");

  for(int pass = 1; pass <= 3; ++pass) {
    output.append("\033[1;35mPass " + QString::number(pass) + "\033[0m ");
    switch(pass) {
    case 1:
      getSearchResults(gameEntries, "md5=" + hashList.at(1), config->platform);
      break;
    case 2:
      getSearchResults(gameEntries, "sha1=" + hashList.at(2), config->platform);
      break;
    case 3:
      getSearchResults(gameEntries, "romnom=" + hashList.at(0), config->platform);
      break;
    default:
      ;
    }
    if(!gameEntries.isEmpty()) {
      break;
    }
  }
}

QString ScreenScraper::getHashes(const QString fileName)
{
  QCryptographicHash md5(QCryptographicHash::Md5);
  QCryptographicHash sha1(QCryptographicHash::Sha1);
  QFile romFile(fileName);
  romFile.open(QIODevice::ReadOnly);
  QByteArray allData = romFile.readAll();
  md5.addData(allData);
  sha1.addData(allData);
  romFile.close();

  return QUrl::toPercentEncoding(romFile.fileName(), "()") + ":" + md5.result().toHex().toUpper() + ":" + sha1.result().toHex().toUpper();
}
