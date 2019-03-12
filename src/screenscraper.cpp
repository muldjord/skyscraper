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

  connect(&limitTimer, &QTimer::timeout, &limiter, &QEventLoop::quit);
  limitTimer.setInterval(1000); // 1 second request limit
  limitTimer.setSingleShot(false);
  limitTimer.start();

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
  QString platformId = getPlatformId(config->platform);
  if(platformId == "na") {
    reqRemaining = 0;
    printf("\033[0;31mPlatform not supported by ScreenScraper or it hasn't yet been included in Skyscraper for this module...\033[0m\n");
    return;
  }

  QString gameUrl = "https://www.screenscraper.fr/api2/jeuInfos.php?devid=muldjord&devpassword=" + StrTools::unMagic("204;198;236;130;203;181;203;126;191;167;200;198;192;228;169;156") + "&softname=skyscraper" VERSION + (config->user.isEmpty()?"":"&ssid=" + config->user) + (config->password.isEmpty()?"":"&sspassword=" + config->password) + (platformId.isEmpty()?"":"&systemeid=" + platformId) + "&output=xml&" + searchName;

  for(int retries = 0; retries < 4; ++retries) {
    limiter.exec();
    manager.request(gameUrl);
    q.exec();
    data = manager.getData();

    if(data.contains("<jeu")) {
      break;
    } else {
      if(data.contains("Erreur : Rom/Iso/Dossier non")) {
	return;
      } else if(data.contains("Votre quota de scrape est")) {
	printf("\033[1;31mYour screenscraper quota has been reached, exiting nicely...\033[0m\n\n");
	reqRemaining = 0;
	return;
      } else if(data.contains("API closed for non-registered members")) {
	printf("\033[1;31mThe screenscraper service is currently too busy to handle requests from unregistered users. Sign up for an account at https://www.screenscraper.fr and support them to gain more threads. Then use the credentials with Skyscraper using the '-u [user:password]' command line option or by setting 'userCreds=[user:password]' in '~/.skyscraper/config.ini'.\033[0m\n\n");
	continue;
      }
    }
  }

  // Workarounds to fix potential invalid XML returned from ScreenScraper
  QByteArray xmlBegin = "<?xml version=";
  QByteArray xmlEnd = "</Data>";
  if(data.contains(xmlBegin) && data.contains(xmlEnd)) {
    data.replace(" & ", " &amp; ");
    if(data.contains("<roms>") && data.contains("</roms>")) {
      data.remove(data.indexOf("<roms>") + 6,
		  data.indexOf("</roms>") - data.indexOf("<roms>") - 6);
    }
    // Remove messages that may have been prepended to the XML
    if(!data.startsWith(xmlBegin))
      data.remove(0, data.indexOf(xmlBegin));
    // Remove messages that may have been appended to the XML
    if(!data.endsWith(xmlEnd))
      data.remove(data.indexOf(xmlEnd) + xmlEnd.length(),
		  data.length() - data.indexOf(xmlEnd) + xmlEnd.length());
  }
  // Workarounds end
  
  if(!xmlDoc.setContent(data)) {
    printf("\033[1;31mScreenScraper APIv2 returned invalid XML\033[0m\n");
    printf("First 256 characters of answer was:\n%s\n", data.left(256).data());
    return;
  }

  GameEntry game;
  game.title = getXmlText("nom", REGION);

  // 'screenscraper' sometimes returns a faulty result with the following name. If we get this
  // result, DON'T use it. It will provide faulty data for the cache
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
  
  bool foundLang = false;
  foreach(QString lang, config->langPrios) {
    for(int a = 0; a < xmlNodes.length(); ++a) {
      QDomElement elem = xmlNodes.at(a).toElement();
      if(elem.attribute("langue") == lang) {
	game.tags.append(xmlNodes.at(a).toElement().text() + ", ");
	foundLang = true;
      }
    }
    if(foundLang)
      break;
  }
  
  game.tags = game.tags.left(game.tags.length() - 2);
}

void ScreenScraper::getCover(GameEntry &game)
{
  QString url = getXmlText("media", REGION, "box-2D");
  if(!url.isEmpty()) {
    bool moveOn = true;
    for(int retries = 0; retries < 4; ++retries) {
      limiter.exec();
      manager.request(url);
      q.exec();
      QImage image;
      if(image.loadFromData(manager.getData())) {
	game.coverData = image;
      } else {
	if(manager.getData().size() < 1000)
	  moveOn = false;
      }
      if(moveOn)
	break;
    }
  }
}

void ScreenScraper::getScreenshot(GameEntry &game)
{
  QString url = getXmlText("media", NONE, "ss");
  if(!url.isEmpty()) {
    bool moveOn = true;
    for(int retries = 0; retries < 4; ++retries) {
      limiter.exec();
      manager.request(url);
      q.exec();
      QImage image;
      if(image.loadFromData(manager.getData())) {
	game.screenshotData = image;
      } else {
	if(manager.getData().size() < 1000)
	  moveOn = false;
      }
      if(moveOn)
	break;
    }
  }
}

void ScreenScraper::getWheel(GameEntry &game)
{
  QString url = getXmlText("media", REGION, "wheel;wheel-hd");
  if(!url.isEmpty()) {
    bool moveOn = true;
    for(int retries = 0; retries < 4; ++retries) {
      limiter.exec();
      manager.request(url);
      q.exec();
      QImage image;
      if(image.loadFromData(manager.getData())) {
	game.wheelData = image;
      } else {
	if(manager.getData().size() < 1000)
	  moveOn = false;
      }
      if(moveOn)
	break;
    }
  }
}

void ScreenScraper::getMarquee(GameEntry &game)
{
  QString url = getXmlText("media", REGION, "screenmarquee");
  if(!url.isEmpty()) {
    bool moveOn = true;
    for(int retries = 0; retries < 4; ++retries) {
      limiter.exec();
      manager.request(url);
      q.exec();
      QImage image;
      if(image.loadFromData(manager.getData())) {
	game.marqueeData = image;
      } else {
	if(manager.getData().size() < 1000)
	  moveOn = false;
      }
      if(moveOn)
	break;
    }
  }
}

void ScreenScraper::getVideo(GameEntry &game)
{
  QString url = getXmlText("media", NONE, "video");
  if(!url.isEmpty()) {
    bool moveOn = true;
    for(int retries = 0; retries < 4; ++retries) {
      limiter.exec();
      manager.request(url);
      q.exec();
      game.videoData = manager.getData();
      // Make sure received data is actually a video file
      QByteArray contentType = manager.getContentType();
      if(contentType.contains("video/") && game.videoData.size() > 4096) {
	game.videoFormat = contentType.mid(contentType.indexOf("/") + 1,
					   contentType.length() - contentType.indexOf("/") + 1);
      } else {
	game.videoData = "";
	moveOn = false;
      }
      if(moveOn)
	break;
    }
  }
}

QList<QString> ScreenScraper::getSearchNames(const QFileInfo &info)
{
  QList<QString> hashList;
  QCryptographicHash md5(QCryptographicHash::Md5);
  QCryptographicHash sha1(QCryptographicHash::Sha1);
  Crc32 crc;
  crc.initInstance(1);

  bool unpack = config->unpack;

  if(unpack) {
    // Size limit for "unpack" is set to 8 megs to ensure the pi doesn't run out of memory
    if((info.suffix() == "7z" || info.suffix() == "zip") && info.size() < 81920000) {
      // For 7z (7z, zip) unpacked file reading
      {
	QProcess decProc;
	decProc.setReadChannel(QProcess::StandardOutput);

	decProc.start("7z l -so \"" + info.absoluteFilePath() + "\"");
	if(decProc.waitForFinished(30000)) {
	  if(decProc.exitStatus() != QProcess::NormalExit) {
	    printf("Getting file list from compressed file failed, falling back...\n");
	    unpack = false;
	  } else if(!decProc.readAllStandardOutput().contains(" 1 files")) {
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
    } else {
      printf("File either not a compressed file or exceeds 8 meg size limit, falling back...\n");
      unpack = false;
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

  QString crcResult = QString::number(crc.releaseInstance(1), 16);
  while(crcResult.length() < 8) {
    crcResult.prepend("0");
  }
  QString md5Result = md5.result().toHex();
  while(md5Result.length() < 32) {
    md5Result.prepend("0");
  }
  QString sha1Result = sha1.result().toHex();
  while(sha1Result.length() < 40) {
    sha1Result.prepend("0");
  }

  hashList.append(QUrl::toPercentEncoding(info.fileName()));
  hashList.append(crcResult.toUpper());
  hashList.append(md5Result.toUpper());
  hashList.append(sha1Result.toUpper());

  QList<QString> searchNames;
  if(info.size() != 0) {
    searchNames.append("romnom=" + hashList.at(0) + "&crc=" + hashList.at(1) + "&md5=" + hashList.at(2) + "&sha1=" + hashList.at(3) + "&romtaille=" + QString::number(info.size()));
  } else {
    searchNames.append("romnom=" + hashList.at(0));
  }

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
    foreach(QString region, regionPrios) {
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
    foreach(QString lang, config->langPrios) {
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
  
QString ScreenScraper::getPlatformId(const QString platform)
{
  if(platform == "3do") {
    return "29";
  } else if(platform == "amiga") {
    return "64";
  } else if(platform == "aga") {
    return "111";
  } else if(platform == "cd32") {
    return "130";
  } else if(platform == "cdtv") {
    return "129";
  } else if(platform == "amstradcpc") {
    return "65";
  } else if(platform == "apple2") {
    return "86";
  } else if(platform == "arcade") {
    return "75";
  } else if(platform == "atari800") {
    return "43";
  } else if(platform == "atari2600") {
    return "26";
  } else if(platform == "atari5200") {
    return "40";
  } else if(platform == "atari7800") {
    return "41";
  } else if(platform == "atarijaguar") {
    return "27";
  } else if(platform == "atarilynx") {
    return "28";
  } else if(platform == "atarist") {
    return "42";
  } else if(platform == "c16") {
    return "na";
  } else if(platform == "c64") {
    return "66";
  } else if(platform == "c128") {
    return "na";
  } else if(platform == "coco") {
    return "144";
  } else if(platform == "coleco") {
    return "75";
  } else if(platform == "daphne") {
    return "49";
  } else if(platform == "dragon32") {
    return "91";
  } else if(platform == "dreamcast") {
    return "23";
  } else if(platform == "fba") {
    return "75";
  } else if(platform == "fds") {
    return "106";
  } else if(platform == "gameandwatch") {
    return "52";
  } else if(platform == "gamegear") {
    return "21";
  } else if(platform == "gb") {
    return "9";
  } else if(platform == "gba") {
    return "12";
  } else if(platform == "gbc") {
    return "10";
  } else if(platform == "gc") {
    return "13";
  } else if(platform == "genesis") {
    return "1";
  } else if(platform == "intellivision") {
    return "115";
  } else if(platform == "mame-advmame") {
    return "75";
  } else if(platform == "mame-libretro") {
    return "75";
  } else if(platform == "mame-mame4all") {
    return "75";
  } else if(platform == "mastersystem") {
    return "2";
  } else if(platform == "megacd") {
    return "20";
  } else if(platform == "megadrive") {
    return "1";
  } else if(platform == "msx") {
    return "113";
  } else if(platform == "n64") {
    return "14";
  } else if(platform == "nds") {
    return "15";
  } else if(platform == "neogeo") {
    return "142";
  } else if(platform == "nes") {
    return "3";
  } else if(platform == "ngp") {
    return "25";
  } else if(platform == "ngpc") {
    return "82";
  } else if(platform == "oric") {
    return "131";
  } else if(platform == "pc") {
    return "135";
  } else if(platform == "pc88") {
    return "na";
  } else if(platform == "pcfx") {
    return "72";
  } else if(platform == "pcengine") {
    return "31";
  } else if(platform == "ports") {
    return "135";
  } else if(platform == "ps2") {
    return "58";
  } else if(platform == "psp") {
    return "61";
  } else if(platform == "psx") {
    return "57";
  } else if(platform == "saturn") {
    return "22";
  } else if(platform == "scummvm") {
    return "123";
  } else if(platform == "sega32x") {
    return "19";
  } else if(platform == "segacd") {
    return "20";
  } else if(platform == "sg-1000") {
    return "109";
  } else if(platform == "snes") {
    return "4";
  } else if(platform == "ti99") {
    return "205";
  } else if(platform == "trs-80") {
    return "144";
  } else if(platform == "vectrex") {
    return "102";
  } else if(platform == "vic20") {
    return "73";
  } else if(platform == "videopac") {
    return "104";
  } else if(platform == "virtualboy") {
    return "11";
  } else if(platform == "wii") {
    return "16";
  } else if(platform == "wonderswan") {
    return "45";
  } else if(platform == "wonderswancolor") {
    return "46";
  } else if(platform == "x68000") {
    return "79";
  } else if(platform == "x1") {
    return "na";
  } else if(platform == "zmachine") {
    return "na";
  } else if(platform == "zx81") {
    return "77";
  } else if(platform == "zxspectrum") {
    return "76";
  }
  return "na";
}
