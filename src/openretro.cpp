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
#include "nametools.h"

#include <QRegularExpression>

OpenRetro::OpenRetro(Settings *config) : AbstractScraper(config)
{
  connect(&manager, &NetComm::dataReady, &q, &QEventLoop::quit);

  baseUrl = "https://openretro.org";

  searchUrlPre = "https://openretro.org";
  searchUrlPost = "&unpublished=1";
  
  searchResultPre = "<div style='margin-bottom: 4px;'>";
  urlPre.append("<a href=\"/");
  urlPost = "\" target='_parent'>";
  titlePre.append("</div>");
  titlePost = " <span";
  platformPre.append("#aaaaaa'>");
  platformPost = "</span>";
  marqueePre.append(">banner_sha1</td>");
  marqueePre.append("<div><a href=\"");
  marqueePost = "\">";
  // Check getDescription function for special case __long_description scrape
  // For description
  descriptionPre.append(">description</td>");
  descriptionPre.append("<td style='color: black;'><div>");
  // For __long_description
  descriptionPre.append("__long_description</td>");
  descriptionPre.append("<td style='color: black;'><div>");
  descriptionPost = "</div></td>";
  developerPre.append("black;'>developer</td>");
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

  fetchOrder.append(MARQUEE);
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
  manager.request(searchUrlPre + searchName + (searchName.left(6) == "/game/"?"":searchUrlPost));
  q.exec();
  while(!manager.getRedirUrl().isEmpty()) {
    manager.request(manager.getRedirUrl());
    q.exec();
  }
  data = manager.getData();

  if(data.isEmpty())
    return;

  if(data.contains("Error: 500 Internal Server Error") ||
     data.contains("Error: 404 Not Found"))
    return;

  GameEntry game;
  
  if(searchName.left(6) == "/game/") {
    QByteArray tempData = data;
    nomNom("<td style='width: 180px; color: black;'>game_name</td>");
    nomNom("<td style='color: black;'><div>");
    game.title = data.left(data.indexOf("</div></td>"));
    data = tempData;
    game.platform = platform;
    gameEntries.append(game);
  } else {
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
}

void OpenRetro::getGameData(GameEntry &game)
{
  if(!game.url.isEmpty()) {
    manager.request(game.url);
    q.exec();
    data = manager.getData();
  }
  
  // Remove all the variants so we don't choose between their screenshots
  data = data.left(data.indexOf("</table></div><div id='"));
  
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

void OpenRetro::getDescription(GameEntry &game)
{
  if(descriptionPre.isEmpty()) {
    return;
  }
  QByteArray tempData = data;

  // Check for __long_description is ordinary description isn't found
  if(data.indexOf(descriptionPre.at(0)) != -1) {
    nomNom(descriptionPre.at(0));
    nomNom(descriptionPre.at(1));
  } else if(data.indexOf(descriptionPre.at(2)) != -1) {
    nomNom(descriptionPre.at(2));
    nomNom(descriptionPre.at(3));
  } else {
    return;
  }

  game.description = data.left(data.indexOf(descriptionPost)).replace("&lt;", "<").replace("&gt;", ">");
  // Revert data back to pre-description
  data = tempData;

  while(game.description.contains("<") && game.description.contains(">") && game.description.indexOf("<") < game.description.indexOf(">")) {
    game.description = game.description.remove(game.description.indexOf("<"), game.description.indexOf(">") + 1 - game.description.indexOf("<"));
  }
  game.description = game.description.simplified();
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

void OpenRetro::getMarquee(GameEntry &game)
{
  if(marqueePre.isEmpty()) {
    return;
  }
  foreach(QString nom, marqueePre) {
    if(!checkNom(nom)) {
      return;
    }
  }
  foreach(QString nom, marqueePre) {
    nomNom(nom);
  }
  QString marqueeUrl = data.left(data.indexOf(marqueePost)).replace("&amp;", "&") + "?s=512";
  if(marqueeUrl.left(4) != "http") {
    marqueeUrl.prepend(baseUrl + (marqueeUrl.left(1) == "/"?"":"/"));
  }
  manager.request(marqueeUrl);
  q.exec();
  QImage image;
  if(image.loadFromData(manager.getData())) {
    game.marqueeData = image;
  }
}

QList<QString> OpenRetro::getSearchNames(const QFileInfo &info)
{
  QString baseName = info.completeBaseName();
  QList<QString> searchNames;
  bool isAga = false;
  // Look for '_aga_', ' aga ' or '[aga]'
  if(QRegularExpression("[_[ ]{1}(aga|AGA)[_\\] ]{1}").match(baseName).hasMatch()) {
    isAga = true;
  }
  
  if(config->scraper != "import") {
    if(info.suffix() == "lha") {
      // Pass 1 is uuid from whdload_db.xml 
      if(whdLoadMap.contains(baseName)) {
	searchNames.append("/game/" + whdLoadMap[baseName].second);
      }
      // Pass 2 is either from <name> tag in whdload_db.xml or by adding spaces
      QString nameWithSpaces = whdLoadMap[baseName].first;
      if(nameWithSpaces.isEmpty()) {
	baseName = NameTools::getNameWithSpaces(baseName);
      } else {
	baseName = nameWithSpaces;
      }
    }
    if(config->platform == "scummvm") {
      baseName = NameTools::getScummName(baseName);
    }
    if(config->platform == "neogeo" ||
       config->platform == "arcade" ||
       config->platform == "fba") {
      baseName = NameTools::getMameName(baseName, mameMap);
    }
  }

  // Remove everything in brackets
  baseName = baseName.left(baseName.indexOf("(")).simplified();
  baseName = baseName.left(baseName.indexOf("[")).simplified();
  // Always remove everything after a ':' since it's always a subtitle
  //baseName = baseName.left(baseName.indexOf(":")).simplified();
  // Always remove everything after a ' - ' since it's always a subtitle
  //baseName = baseName.left(baseName.indexOf(" - ")).simplified();

  QRegularExpressionMatch match;
  // Remove " rev.X" instances
  match = QRegularExpression(" rev\\.(\\d.*\\d*|[IVX]{1,5})$").match(baseName);
  if(match.hasMatch()) {
    baseName = baseName.left(baseName.indexOf(match.captured(0))).simplified();
  }
  // Remove " v.X" instances
  match = QRegularExpression(" v\\.(\\d.*\\d*|[IVX]{1,5})$").match(baseName);
  if(match.hasMatch()) {
    baseName = baseName.left(baseName.indexOf(match.captured(0))).simplified();
  }

  // If we have the first game in a series, remove the ' I' for more search results
  if(baseName.right(2) == " I") {
    baseName = baseName.left(baseName.length() - 2);
  }

  baseName = baseName.toLower();

  // Always remove 'the' from beginning or end if equal to or longer than 10 chars.
  // If it's shorter the 'the' is of more significance and shouldn't be removed.
  if(baseName.length() >= 10) {
    if(baseName.simplified().left(4) == "the ") {
      baseName = baseName.simplified().remove(0, 4);
    }
    if(baseName.simplified().right(5) == ", the") {
      baseName = baseName.simplified().left(baseName.indexOf(", the"));
    }
  }

  baseName = baseName.replace("_", " ");
  baseName = baseName.replace(" - ", ": ");
  baseName = baseName.replace(",", " ");
  baseName = baseName.replace("&", "%26");
  baseName = baseName.replace("+", "");
  // A few game names have faulty "s's". Fix them to "s'"
  baseName = baseName.replace("s's", "s'");
  baseName = baseName.replace("'", "%27");
  // Finally change all spaces to plusses since that's what most search engines understand
  baseName = baseName.simplified().replace(" ", "+");

  // Implement special cases here
  if(baseName == "ik") {
    baseName = "international+karate";
  }
  if(baseName == "arkanoid+revenge+of+doh") {
    baseName = "arkanoid%3A+revenge+of+doh";
  }
  if(baseName == "lemmings+3") {
    baseName = "all+new+world+of+lemmings";
  }

  if(!baseName.isEmpty()) {
    if(isAga) {
      searchNames.append("/browse?q=" + baseName + "+aga");
    }
    searchNames.append("/browse?q=" + baseName);
  }

  if(baseName.indexOf(":") != -1 || baseName.indexOf("-") != -1) {
    baseName = baseName.left(baseName.indexOf(":")).simplified();
    baseName = baseName.left(baseName.indexOf("-")).simplified();
    searchNames.append("/browse?q=" + baseName);
  }

  return searchNames;
}
