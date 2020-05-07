/***************************************************************************
 *            abstractscraper.cpp
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

#include "abstractscraper.h"
#include "platform.h"
#include "nametools.h"
#include "strtools.h"

#include <QRegularExpression>
#include <QDomDocument>

AbstractScraper::AbstractScraper(Settings *config)
{
  this->config = config;
}

AbstractScraper::~AbstractScraper()
{
}

void AbstractScraper::getSearchResults(QList<GameEntry> &gameEntries,
				       QString searchName, QString platform)
{
  manager.request(searchUrlPre + searchName + searchUrlPost);
  q.exec();
  data = manager.getData();
  
  GameEntry game;
  
  while(data.indexOf(searchResultPre) != -1) {
    nomNom(searchResultPre);

    // Digest until url
    for(const auto &nom: urlPre) {
      nomNom(nom);
    }
    game.url = baseUrl + "/" + data.left(data.indexOf(urlPost));

    // Digest until title
    for(const auto &nom: titlePre) {
      nomNom(nom);
    }
    game.title = data.left(data.indexOf(titlePost));

    // Digest until platform
    for(const auto &nom: platformPre) {
      nomNom(nom);
    }
    game.platform = data.left(data.indexOf(platformPost));

    if(platformMatch(game.platform, platform)) {
      gameEntries.append(game);
    }
  }
}

void AbstractScraper::getGameData(GameEntry &game)
{
  manager.request(game.url);
  q.exec();
  data = manager.getData();
  //printf("URL IS: '%s'\n", game.url.toStdString().c_str());
  //printf("DATA IS:\n'%s'\n", data.data());

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

void AbstractScraper::getDescription(GameEntry &game)
{
  if(descriptionPre.isEmpty()) {
    return;
  }
  for(const auto &nom: descriptionPre) {
    if(!checkNom(nom)) {
      return;
    }
  }
  for(const auto &nom: descriptionPre) {
    nomNom(nom);
  }

  game.description = data.left(data.indexOf(descriptionPost)).replace("&lt;", "<").replace("&gt;", ">");
  game.description = game.description.replace("\\n", "\n");

  // Remove all html tags within description
  game.description = StrTools::stripHtmlTags(game.description);
}

void AbstractScraper::getDeveloper(GameEntry &game)
{
  for(const auto &nom: developerPre) {
    if(!checkNom(nom)) {
      return;
    }
  }
  for(const auto &nom: developerPre) {
    nomNom(nom);
  }
  game.developer = data.left(data.indexOf(developerPost));
}

void AbstractScraper::getPublisher(GameEntry &game)
{
  if(publisherPre.isEmpty()) {
    return;
  }
  for(const auto &nom: publisherPre) {
    if(!checkNom(nom)) {
      return;
    }
  }
  for(const auto &nom: publisherPre) {
    nomNom(nom);
  }
  game.publisher = data.left(data.indexOf(publisherPost));
}

void AbstractScraper::getPlayers(GameEntry &game)
{
  if(playersPre.isEmpty()) {
    return;
  }
  for(const auto &nom: playersPre) {
    if(!checkNom(nom)) {
      return;
    }
  }
  for(const auto &nom: playersPre) {
    nomNom(nom);
  }
  game.players = data.left(data.indexOf(playersPost));
}

void AbstractScraper::getAges(GameEntry &game)
{
  if(agesPre.isEmpty()) {
    return;
  }
  for(const auto &nom: agesPre) {
    if(!checkNom(nom)) {
      return;
    }
  }
  for(const auto &nom: agesPre) {
    nomNom(nom);
  }
  game.ages = data.left(data.indexOf(agesPost));
}

void AbstractScraper::getTags(GameEntry &game)
{
  if(tagsPre.isEmpty()) {
    return;
  }
  for(const auto &nom: tagsPre) {
    if(!checkNom(nom)) {
      return;
    }
  }
  for(const auto &nom: tagsPre) {
    nomNom(nom);
  }
  game.tags = data.left(data.indexOf(tagsPost));
}

void AbstractScraper::getRating(GameEntry &game)
{
  if(ratingPre.isEmpty()) {
    return;
  }
  for(const auto &nom: ratingPre) {
    if(!checkNom(nom)) {
      return;
    }
  }
  for(const auto &nom: ratingPre) {
    nomNom(nom);
  }
  game.rating = data.left(data.indexOf(ratingPost));
  bool toDoubleOk = false;
  double rating = game.rating.toDouble(&toDoubleOk);
  if(toDoubleOk) {
    game.rating = QString::number(rating / 5.0);
  } else {
    game.rating = "";
  }
}

void AbstractScraper::getReleaseDate(GameEntry &game)
{
  if(releaseDatePre.isEmpty()) {
    return;
  }
  for(const auto &nom: releaseDatePre) {
    if(!checkNom(nom)) {
      return;
    }
  }
  for(const auto &nom: releaseDatePre) {
    nomNom(nom);
  }
  game.releaseDate = data.left(data.indexOf(releaseDatePost)).simplified();
}

void AbstractScraper::getCover(GameEntry &game)
{
  if(coverPre.isEmpty()) {
    return;
  }
  for(const auto &nom: coverPre) {
    if(!checkNom(nom)) {
      return;
    }
  }
  for(const auto &nom: coverPre) {
    nomNom(nom);
  }
  QString coverUrl = data.left(data.indexOf(coverPost)).replace("&amp;", "&");
  if(coverUrl.left(4) != "http") {
    coverUrl.prepend(baseUrl + (coverUrl.left(1) == "/"?"":"/"));
  }
  manager.request(coverUrl);
  q.exec();
  QImage image;
  if(manager.getError() == QNetworkReply::NoError &&
     image.loadFromData(manager.getData())) {
    game.coverData = manager.getData();
  }
}

void AbstractScraper::getScreenshot(GameEntry &game)
{
  if(screenshotPre.isEmpty()) {
    return;
  }
  // Check that we have enough screenshots
  int screens = data.count(screenshotCounter.toUtf8());
  if(screens >= 1) {
    for(int a = 0; a < screens - (screens / 2); a++) {
      for(const auto &nom: screenshotPre) {
	nomNom(nom);
      }
    }
    QString screenshotUrl = data.left(data.indexOf(screenshotPost)).replace("&amp;", "&");
    if(screenshotUrl.left(4) != "http") {
      screenshotUrl.prepend(baseUrl + (screenshotUrl.left(1) == "/"?"":"/"));
    }
    manager.request(screenshotUrl);
    q.exec();
    QImage image;
    if(manager.getError() == QNetworkReply::NoError &&
       image.loadFromData(manager.getData())) {
      game.screenshotData = manager.getData();
    }
  }
}

void AbstractScraper::getWheel(GameEntry &game)
{
  if(wheelPre.isEmpty()) {
    return;
  }
  for(const auto &nom: wheelPre) {
    if(!checkNom(nom)) {
      return;
    }
  }
  for(const auto &nom: wheelPre) {
    nomNom(nom);
  }
  QString wheelUrl = data.left(data.indexOf(wheelPost)).replace("&amp;", "&");
  if(wheelUrl.left(4) != "http") {
    wheelUrl.prepend(baseUrl + (wheelUrl.left(1) == "/"?"":"/"));
  }
  manager.request(wheelUrl);
  q.exec();
  QImage image;
  if(manager.getError() == QNetworkReply::NoError &&
     image.loadFromData(manager.getData())) {
    game.wheelData = manager.getData();
  }
}

void AbstractScraper::getMarquee(GameEntry &game)
{
  if(marqueePre.isEmpty()) {
    return;
  }
  for(const auto &nom: marqueePre) {
    if(!checkNom(nom)) {
      return;
    }
  }
  for(const auto &nom: marqueePre) {
    nomNom(nom);
  }
  QString marqueeUrl = data.left(data.indexOf(marqueePost)).replace("&amp;", "&");
  if(marqueeUrl.left(4) != "http") {
    marqueeUrl.prepend(baseUrl + (marqueeUrl.left(1) == "/"?"":"/"));
  }
  manager.request(marqueeUrl);
  q.exec();
  QImage image;
  if(manager.getError() == QNetworkReply::NoError &&
     image.loadFromData(manager.getData())) {
    game.marqueeData = manager.getData();
  }
}

void AbstractScraper::getVideo(GameEntry &game)
{
  if(videoPre.isEmpty()) {
    return;
  }
  for(const auto &nom: videoPre) {
    if(!checkNom(nom)) {
      return;
    }
  }
  for(const auto &nom: videoPre) {
    nomNom(nom);
  }
  QString videoUrl = data.left(data.indexOf(videoPost)).replace("&amp;", "&");
  if(videoUrl.left(4) != "http") {
    videoUrl.prepend(baseUrl + (videoUrl.left(1) == "/"?"":"/"));
  }
  manager.request(videoUrl);
  q.exec();
  if(manager.getError() == QNetworkReply::NoError) {
    game.videoData = manager.getData();
    game.videoFormat = videoUrl.right(3);
  }
}

void AbstractScraper::nomNom(const QString nom, bool including)
{
  data.remove(0, data.indexOf(nom) + (including?nom.length():0));
}

bool AbstractScraper::checkNom(const QString nom)
{
  if(data.indexOf(nom) != -1) {
    return true;
  }
  return false;
}

QList<QString> AbstractScraper::getSearchNames(const QFileInfo &info)
{
  QString baseName = info.completeBaseName();
  
  if(config->scraper != "import") {
    if(!config->aliasMap[baseName].isEmpty()) {
      baseName = config->aliasMap[baseName];
    } else if(info.suffix() == "lha") {
      QString nameWithSpaces = config->whdLoadMap[baseName].first;
      if(nameWithSpaces.isEmpty()) {
	baseName = NameTools::getNameWithSpaces(baseName);
      } else {
	baseName = nameWithSpaces;
      }
    } else if(config->platform == "scummvm") {
      baseName = NameTools::getScummName(baseName, config->scummIni);
    } else if((config->platform == "neogeo" ||
	       config->platform == "arcade" ||
	       config->platform == "mame-advmame" ||
	       config->platform == "mame-libretro" ||
	       config->platform == "mame-mame4all" ||
	       config->platform == "fba") && !config->mameMap[baseName].isEmpty()) {
      baseName = config->mameMap[baseName];
    }
  }

  QList<QString> searchNames;

  if(baseName.isEmpty())
    return searchNames;

  searchNames.append(NameTools::getUrlQueryName(baseName));

  if(baseName.contains(":") || baseName.contains(" - ")) {
    QString noSubtitle = baseName.left(baseName.indexOf(":")).simplified();
    noSubtitle = noSubtitle.left(noSubtitle.indexOf(" - ")).simplified();
    // Only add if longer than 3. We don't want to search for "the" for instance
    if(noSubtitle.length() > 3)
      searchNames.append(NameTools::getUrlQueryName(noSubtitle));
  }
  
  if(NameTools::hasRomanNumeral(baseName) || NameTools::hasIntegerNumeral(baseName)) {
    if(NameTools::hasRomanNumeral(baseName)) {
      baseName = NameTools::convertToIntegerNumeral(baseName);
    } else if(NameTools::hasIntegerNumeral(baseName)) {
      baseName = NameTools::convertToRomanNumeral(baseName);
    }
    searchNames.append(NameTools::getUrlQueryName(baseName));

    if(baseName.contains(":") || baseName.contains(" - ")) {
      QString noSubtitle = baseName.left(baseName.indexOf(":")).simplified();
      noSubtitle = noSubtitle.left(noSubtitle.indexOf(" - ")).simplified();
      // Only add if longer than 3. We don't want to search for "the" for instance
      if(noSubtitle.length() > 3)
	searchNames.append(NameTools::getUrlQueryName(noSubtitle));
    }
  }

  return searchNames;
}

QString AbstractScraper::getCompareTitle(QFileInfo info)
{
  QString baseName = info.completeBaseName();

  if(config->scraper != "import") {
    if(!config->aliasMap[baseName].isEmpty()) {
      baseName = config->aliasMap[baseName];
    } else if(info.suffix() == "lha") {
      QString nameWithSpaces = config->whdLoadMap[baseName].first;
      if(nameWithSpaces.isEmpty()) {
	baseName = NameTools::getNameWithSpaces(baseName);
      } else {
	baseName = nameWithSpaces;
      }
    } else if(config->platform == "scummvm") {
      baseName = NameTools::getScummName(baseName, config->scummIni);
    } else if((config->platform == "neogeo" ||
	       config->platform == "arcade" ||
	       config->platform == "mame-advmame" ||
	       config->platform == "mame-libretro" ||
	       config->platform == "mame-mame4all" ||
	       config->platform == "fba") && !config->mameMap[baseName].isEmpty()) {
      baseName = config->mameMap[baseName];
    }
  }

  // Now create actual compareTitle
  baseName = baseName.replace("_", " ").left(baseName.indexOf("(")).left(baseName.indexOf("[")).simplified();

  QRegularExpressionMatch match;

  // Always move ", The" to the beginning of the name
  match = QRegularExpression(", [Tt]he").match(baseName);
  if(match.hasMatch()) {
    baseName = baseName.replace(match.captured(0), "").prepend(match.captured(0).right(3) + " ");
  }

  // Remove "vX.XXX" versioning string if one is found
  match = QRegularExpression(" v[.]{0,1}([0-9]{1}[0-9]{0,2}[.]{0,1}[0-9]{1,4}|[IVX]{1,5})$").match(baseName);
  if(match.hasMatch() && match.capturedStart(0) != -1) {
    baseName = baseName.left(match.capturedStart(0)).simplified();
  }
    
  return baseName;
}

void AbstractScraper::runPasses(QList<GameEntry> &gameEntries, const QFileInfo &info, QString &output, QString &debug)
{
  // Reset region priorities to original list from Settings
  regionPrios = config->regionPrios;
  // Autodetect region and append to region priorities
  if(info.fileName().indexOf("(") != -1 && config->region.isEmpty()) {
    QString regionString = info.fileName().toLower().mid(info.fileName().indexOf("("), info.fileName().length());
    if(regionString.contains("europe") || regionString.contains("(e)")) {
      regionPrios.prepend("eu");
    }
    if(regionString.contains("usa") || regionString.contains("(u)")) {
      regionPrios.prepend("us");
    }
    if(regionString.contains("world")) {
      regionPrios.prepend("wor");
    }
    if(regionString.contains("japan") || regionString.contains("(j)")) {
      regionPrios.prepend("jp");
    }
    if(regionString.contains("brazil")) {
      regionPrios.prepend("br");
    }
    if(regionString.contains("korea")) {
      regionPrios.prepend("kr");
    }
    if(regionString.contains("taiwan")) {
      regionPrios.prepend("tw");
    }
    if(regionString.contains("france")) {
      regionPrios.prepend("fr");
    }
    if(regionString.contains("germany")) {
      regionPrios.prepend("de");
    }
    if(regionString.contains("italy")) {
      regionPrios.prepend("it");
    }
    if(regionString.contains("spain")) {
      regionPrios.prepend("sp");
    }
    if(regionString.contains("china")) {
      regionPrios.prepend("cn");
    }
    if(regionString.contains("australia")) {
      regionPrios.prepend("au");
    }
    if(regionString.contains("sweden")) {
      regionPrios.prepend("se");
    }
    if(regionString.contains("canada")) {
      regionPrios.prepend("ca");
    }
    if(regionString.contains("netherlands")) {
      regionPrios.prepend("nl");
    }
    if(regionString.contains("denmark")) {
      regionPrios.prepend("dk");
    }
    if(regionString.contains("asia")) {
      regionPrios.prepend("asi");
    }
  }
  
  QList<QString> searchNames;
  if(config->searchName.isEmpty()) {
    searchNames = getSearchNames(info);
  } else {
    // Add the string provided by "--query"
    searchNames.append(config->searchName);
  }

  if(searchNames.isEmpty()) {
    return;
  }

  for(int pass = 1; pass <= searchNames.size(); ++pass) {
    output.append("\033[1;35mPass " + QString::number(pass) + "\033[0m ");
    getSearchResults(gameEntries, searchNames.at(pass - 1), config->platform);
    debug.append("Tried with: '" + searchNames.at(pass - 1) + "'\n");
    debug.append("Platform: " + config->platform + "\n");
    if(!gameEntries.isEmpty()) {
      break;
    }
  }
}

bool AbstractScraper::platformMatch(QString found, QString platform) {
  for(const auto &p: Platform::getAliases(platform)) {
    if(found.toLower() == p) {
      return true;
    }
  }
  return false;
}

QString AbstractScraper::getPlatformId(const QString)
{
  return "na";
}
