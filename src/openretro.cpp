/***************************************************************************
 *            openretro.cpp
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

#include "openretro.h"

OpenRetro::OpenRetro()
{
  connect(&manager, &NetComm::dataReady, &q, &QEventLoop::quit);

  baseUrl = "https://openretro.org";

  searchUrlPre = "https://openretro.org/browse?q=";
  searchUrlPost = "&unpublished=1";
  
  searchResultPre = "<div style='margin-bottom: 4px;'>";
  urlPre.append("<a href=\"/");
  urlPost = "\" target='_parent'>";
  titlePre.append("</div>");
  titlePost = " <span";
  platformPre.append("#aaaaaa'>");
  platformPost = "</span>";
  developerPre.append("<div class=\"game-info-row\"><span>Developer:</span>");
  developerPost = "</div>";
  publisherPre.append("<div class=\"game-info-row\"><span>Publisher:</span>");
  publisherPost = "</div>";
  playersPre.append("<div class=\"game-info-row\"><span>Players:</span>");
  playersPost = "</div>";
  releaseDatePre.append("<div class=\"game-info-row\"><span>Year:</span>");
  releaseDatePost = "</div>";
  descriptionPre.append("<div class='game-about'><p>");
  descriptionPost = "</p></div>";
  screenshotCounter = "2x\" data-rel='lightbox[images]'><img src=";
  screenshotPre.append("2x\" data-rel='lightbox[images]'><img src=");
  screenshotPre.append("<a href=\"");
  screenshotPost = "?s=2x";
  
  fetchOrder.append(SCREENSHOT);
  fetchOrder.append(COVER);
  fetchOrder.append(PUBLISHER);
  fetchOrder.append(RELEASEDATE);
  fetchOrder.append(DEVELOPER);
  fetchOrder.append(PLAYERS);
  fetchOrder.append(TAGS);
  fetchOrder.append(DESCRIPTION);
}

void OpenRetro::getSearchResults(QList<GameEntry> &gameEntries,
				 QString searchName, QString platform)
{
  manager.request(searchUrlPre + searchName + searchUrlPost);
  q.exec();
  data = manager.getData();
  
  GameEntry game;
  
  while(data.indexOf(searchResultPre) != -1) {
    nomNom(searchResultPre);

    // Digest until url
    foreach(QString nom, urlPre) {
      nomNom(nom);
    }
    game.url = baseUrl + "/" + data.left(data.indexOf(urlPost));

    // Digest until title
    foreach(QString nom, titlePre) {
      nomNom(nom);
    }
    game.title = data.left(data.indexOf(titlePost)).replace("[AGA]", "").simplified();

    // Digest until platform
    foreach(QString nom, platformPre) {
      nomNom(nom);
    }
    game.platform = data.left(data.indexOf(platformPost)).replace("&nbsp;", " ");

    if(platformMatch(game.platform, platform)) {
      gameEntries.append(game);
    }
  }
}

void OpenRetro::getScreenshot(GameEntry &game)
{
  // Check that we have enough screenshots
  int screens = data.count(screenshotCounter.toUtf8());
  if(screens >= 2) {
    foreach(QString nom, screenshotPre) {
      if(!checkNom(nom)) {
	return;
      }
    }
    foreach(QString nom, screenshotPre) {
      nomNom(nom);
    }
  }
  QString screenshotUrl = baseUrl + data.left(data.indexOf(screenshotPost)) + "?s=1x";
  manager.request(screenshotUrl);
  q.exec();
  QImage image(QImage::fromData(manager.getData()));
  if(!image.isNull()) {
    game.screenshotData = image;
  }
}

void OpenRetro::getCover(GameEntry &game)
{
  nomNom("</div></div><a href=\"");
  QString coverUrl = baseUrl + data.left(data.indexOf("?s=")) + "?s=1x";
  if(coverUrl.indexOf("0000000000") != -1) {
    return;
  }
  manager.request(coverUrl);
  q.exec();
  QImage image(QImage::fromData(manager.getData()));
  if(!image.isNull()) {
    game.coverData = image;
  }
}

void OpenRetro::getTags(GameEntry &game)
{
  QString tags = "";
  QString tagBegin = "<a href=\"/browse/";
  while(data.indexOf(tagBegin) != -1) {
    nomNom(tagBegin);
    nomNom("\">");
    tags.append(data.left(data.indexOf("</a>")) + ", ");
  }
  if(!tags.isEmpty()) {
    tags.chop(2); // Remove last ", "
  }

  game.tags = tags;
}

QString OpenRetro::actualPlatform(const QString platform)
{
  if(platform == "aga") {
    return "amiga";
  } else if(platform == "atari2600") {
    return "atari 2600";
  } else if(platform == "atari5200") {
    return "atari 5200";
  } else if(platform == "atari7800") {
    return "atari 7800";
  } else if(platform == "atarist") {
    return "atari st";
  } else if(platform == "gb") {
    return "game boy";
  } else if(platform == "gba") {
    return "game boy advance";
  } else if(platform == "gbc") {
    return "game boy color";
  } else if(platform == "megadrive") {
    return "mega drive";
  } else if(platform == "genesis") {
    return "mega drive";
  } else if(platform == "c64") {
    return "commodore 64";
  } else if(platform == "nes") {
    return "nintendo";
  } else if(platform == "pcengine") {
    return "turbografx-16";
  } else if(platform == "psx") {
    return "playstation";
  } else if(platform == "snes") {
    return "super nintendo";
  } else if(platform == "zxspectrum") {
    return "zx spectrum";
  }
  return platform;
}
