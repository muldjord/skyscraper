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

ScreenScraper::ScreenScraper()
{
  connect(&manager, &NetComm::dataReady, &q, &QEventLoop::quit);

  baseUrl = "http://www.screenscraper.fr";

  descriptionPre.append("<synopsis>");
  descriptionPre.append("<synopsis_en>");
  descriptionPost = "</synopsis_en>";
  publisherPre.append("<editeur>");
  publisherPost = "</editeur>";
  developerPre.append("<developpeur>");
  developerPost = "</developpeur>";
  playersPre.append("<joueurs>");
  playersPost = "</joueurs>";
  releaseDatePre.append("<dates>");
  releaseDatePre.append("<date_");
  releaseDatePre.append(">");
  releaseDatePost = "</date";
  tagsPre.append("<genre_en>");
  tagsPost = "</genre_en>";
  screenshotCounter = "<media_screenshot>";
  screenshotPre.append("<media_screenshot>");
  screenshotPost = "</media_screenshot>";
  coverPre.append("<media_boxstexture>");
  coverPre.append("<media_boxtexture");
  coverPre.append(">");
  coverPost = "</media_boxtexture";
  
  fetchOrder.append(VIDEO);
  fetchOrder.append(PUBLISHER);
  fetchOrder.append(DEVELOPER);
  fetchOrder.append(PLAYERS);
  fetchOrder.append(DESCRIPTION);
  fetchOrder.append(RELEASEDATE);
  fetchOrder.append(TAGS);
  fetchOrder.append(SCREENSHOT);
  fetchOrder.append(COVER);
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
  //QString gameUrl = "https://www.screenscraper.fr/api/jeuInfos.php?devid=" + user + "&devpassword=" + password + "&softname=skyscraper" VERSION "&output=xml&" + searchName;
  QString gameUrl = "https://www.screenscraper.fr/api/jeuInfos.php?devid=muldjord&devpassword=uWu5VRc9QDVMPpD8&softname=skyscraper" VERSION "&output=xml&" + searchName;
  manager.request(gameUrl);
  q.exec();
  data = manager.getData();

  if(data.indexOf("Erreur") != -1) {
    return;
  }
  
  GameEntry game;

  if(data.indexOf("<nom_wor>") != -1) {
    nomNom("<nom_wor>");
    game.title = data.left(data.indexOf("</nom_wor>"));
  } else if(data.indexOf("<nom_us>") != -1) {
    nomNom("<nom_us>");
    game.title = data.left(data.indexOf("</nom_us>"));
  } else if(data.indexOf("<nom>") != -1) {
    nomNom("<nom>");
    game.title = data.left(data.indexOf("</nom>"));
  }
  
  game.url = gameUrl;
  
  nomNom("<systemenom>");
  game.platform = data.left(data.indexOf("</systemenom>"));
  
  if(game.platform.toLower() == actualPlatform(platform)) {
    gameEntries.append(game);
  }
}

void ScreenScraper::getVideo(GameEntry &game)
{
  nomNom("<jeu>");
  nomNom("<id>");
  QString gameId = data.left(data.indexOf("</id>"));
  nomNom("<systemeid>");
  QString systemId = data.left(data.indexOf("</systemeid>"));
  QString videoUrl = "https://www.screenscraper.fr/api/mediaVideoJeu.php?devid=muldjord&devpassword=uWu5VRc9QDVMPpD8&softname=skyscraper&systemeid=" + systemId + "&jeuid=" + gameId + "&media=video";
  manager.request(videoUrl);
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
    // Reset searchName for each pass
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
