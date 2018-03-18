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
  wheelPre.append(">banner_sha1</td>");
  wheelPre.append("<div><a href=\"");
  wheelPost = "\">";
  descriptionPre.append(">description</td>");
  descriptionPre.append("<td style='color: black;'><div>");
  descriptionPost = "</div></td>";
  developerPre.append(">developer</td>");
  developerPre.append("<td style='color: black;'><div>");
  developerPost = "</div></td>";
  coverPre.append(">front_sha1</td>");
  coverPre.append("<div><a href=\"");
  coverPost = "\">";
  playersPre.append(">players</td>");
  playersPre.append("<td style='color: black;'><div>");
  playersPost = "</div></td>";
  publisherPre.append(">publisher</td>");
  publisherPre.append("<td style='color: black;'><div>");
  publisherPost = "</div></td>";
  screenshotCounter = "<td style='width: 180px; color: black;'>screen";
  screenshotPre.append("<td style='width: 180px; color: black;'>screen");
  screenshotPre.append("<td style='color: black;'><div><a href=\"");
  screenshotPost = "\">";
  releaseDatePre.append(">year</td>");
  releaseDatePre.append("<td style='color: black;'><div>");
  releaseDatePost = "</div></td>";
  tagsPre.append(">tags</td>");

  fetchOrder.append(WHEEL);
  fetchOrder.append(DESCRIPTION);
  fetchOrder.append(DEVELOPER);
  fetchOrder.append(COVER);
  fetchOrder.append(PLAYERS);
  fetchOrder.append(PUBLISHER);
  fetchOrder.append(SCREENSHOT);
  fetchOrder.append(TAGS);
  fetchOrder.append(RELEASEDATE);
}

void OpenRetro::getSearchResults(QList<GameEntry> &gameEntries,
				 QString searchName, QString platform)
{
  manager.request(searchUrlPre + searchName + searchUrlPost);
  q.exec();
  data = manager.getData();

  if(data.isEmpty())
    return;
  
  GameEntry game;
  
  while(data.indexOf(searchResultPre) != -1) {
    nomNom(searchResultPre);

    // Digest until url
    foreach(QString nom, urlPre) {
      nomNom(nom);
    }
    game.url = baseUrl + "/" + data.left(data.indexOf(urlPost)) + "/edit";

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

void OpenRetro::getTags(GameEntry &game)
{
  foreach(QString nom, tagsPre) {
    if(!checkNom(nom)) {
      return;
    }
  }
  foreach(QString nom, tagsPre) {
    nomNom(nom);
  }
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

void OpenRetro::getCover(GameEntry &game)
{
  if(coverPre.isEmpty()) {
    return;
  }
  foreach(QString nom, coverPre) {
    if(!checkNom(nom)) {
      return;
    }
  }
  foreach(QString nom, coverPre) {
    nomNom(nom);
  }
  QString coverUrl = data.left(data.indexOf(coverPost)).replace("&amp;", "&") + "?s=512";
  if(coverUrl.left(4) != "http") {
    coverUrl.prepend(baseUrl + (coverUrl.left(1) == "/"?"":"/"));
  }
  manager.request(coverUrl);
  q.exec();
  QImage image;
  if(image.loadFromData(manager.getData())) {
    game.coverData = image;
  }
}

void OpenRetro::getWheel(GameEntry &game)
{
  if(wheelPre.isEmpty()) {
    return;
  }
  foreach(QString nom, wheelPre) {
    if(!checkNom(nom)) {
      return;
    }
  }
  foreach(QString nom, wheelPre) {
    nomNom(nom);
  }
  QString wheelUrl = data.left(data.indexOf(wheelPost)).replace("&amp;", "&") + "?s=512";
  if(wheelUrl.left(4) != "http") {
    wheelUrl.prepend(baseUrl + (wheelUrl.left(1) == "/"?"":"/"));
  }
  manager.request(wheelUrl);
  q.exec();
  QImage image;
  if(image.loadFromData(manager.getData())) {
    game.wheelData = image;
  }
}
