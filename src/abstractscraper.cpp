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
    foreach(QString nom, urlPre) {
      nomNom(nom);
    }
    game.url = baseUrl + "/" + data.left(data.indexOf(urlPost));

    // Digest until title
    foreach(QString nom, titlePre) {
      nomNom(nom);
    }
    game.title = data.left(data.indexOf(titlePost));

    // Digest until platform
    foreach(QString nom, platformPre) {
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
  foreach(QString nom, descriptionPre) {
    if(!checkNom(nom)) {
      return;
    }
  }
  foreach(QString nom, descriptionPre) {
    nomNom(nom);
  }

  game.description = data.left(data.indexOf(descriptionPost)).replace("&lt;", "<").replace("&gt;", ">");

  // Remove all html tags within description
  while(game.description.indexOf("<") != -1 && game.description.indexOf(">") != -1 && game.description.indexOf("<") < game.description.indexOf(">")) {
    game.description = game.description.remove(game.description.indexOf("<"), game.description.indexOf(">") + 1 - game.description.indexOf("<"));
  }
  game.description = game.description.simplified();
}

void AbstractScraper::getDeveloper(GameEntry &game)
{
  foreach(QString nom, developerPre) {
    if(!checkNom(nom)) {
      return;
    }
  }
  foreach(QString nom, developerPre) {
    nomNom(nom);
  }
  game.developer = data.left(data.indexOf(developerPost));
}

void AbstractScraper::getPublisher(GameEntry &game)
{
  if(publisherPre.isEmpty()) {
    return;
  }
  foreach(QString nom, publisherPre) {
    if(!checkNom(nom)) {
      return;
    }
  }
  foreach(QString nom, publisherPre) {
    nomNom(nom);
  }
  game.publisher = data.left(data.indexOf(publisherPost));
}

void AbstractScraper::getPlayers(GameEntry &game)
{
  if(playersPre.isEmpty()) {
    return;
  }
  foreach(QString nom, playersPre) {
    if(!checkNom(nom)) {
      return;
    }
  }
  foreach(QString nom, playersPre) {
    nomNom(nom);
  }
  game.players = data.left(data.indexOf(playersPost));
}

void AbstractScraper::getAges(GameEntry &game)
{
  if(agesPre.isEmpty()) {
    return;
  }
  foreach(QString nom, agesPre) {
    if(!checkNom(nom)) {
      return;
    }
  }
  foreach(QString nom, agesPre) {
    nomNom(nom);
  }
  game.ages = data.left(data.indexOf(agesPost));
}

void AbstractScraper::getTags(GameEntry &game)
{
  if(tagsPre.isEmpty()) {
    return;
  }
  foreach(QString nom, tagsPre) {
    if(!checkNom(nom)) {
      return;
    }
  }
  foreach(QString nom, tagsPre) {
    nomNom(nom);
  }
  game.tags = data.left(data.indexOf(tagsPost));
}

void AbstractScraper::getRating(GameEntry &game)
{
  if(ratingPre.isEmpty()) {
    return;
  }
  foreach(QString nom, ratingPre) {
    if(!checkNom(nom)) {
      return;
    }
  }
  foreach(QString nom, ratingPre) {
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
  foreach(QString nom, releaseDatePre) {
    if(!checkNom(nom)) {
      return;
    }
  }
  foreach(QString nom, releaseDatePre) {
    nomNom(nom);
  }
  game.releaseDate = data.left(data.indexOf(releaseDatePost)).simplified();
}

void AbstractScraper::getCover(GameEntry &game)
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
  QString coverUrl = data.left(data.indexOf(coverPost)).replace("&amp;", "&");
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

void AbstractScraper::getScreenshot(GameEntry &game)
{
  if(screenshotPre.isEmpty()) {
    return;
  }
  // Check that we have enough screenshots
  int screens = data.count(screenshotCounter.toUtf8());
  if(screens >= 1) {
    for(int a = 0; a < screens - (screens / 2); a++) {
      foreach(QString nom, screenshotPre) {
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
    if(image.loadFromData(manager.getData())) {
      game.screenshotData = image;
    }
  }
}

void AbstractScraper::getWheel(GameEntry &game)
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
  QString wheelUrl = data.left(data.indexOf(wheelPost)).replace("&amp;", "&");
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

void AbstractScraper::getMarquee(GameEntry &game)
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
  QString marqueeUrl = data.left(data.indexOf(marqueePost)).replace("&amp;", "&");
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

void AbstractScraper::getVideo(GameEntry &game)
{
  if(videoPre.isEmpty()) {
    return;
  }
  foreach(QString nom, videoPre) {
    if(!checkNom(nom)) {
      return;
    }
  }
  foreach(QString nom, videoPre) {
    nomNom(nom);
  }
  QString videoUrl = data.left(data.indexOf(videoPost)).replace("&amp;", "&");
  if(videoUrl.left(4) != "http") {
    videoUrl.prepend(baseUrl + (videoUrl.left(1) == "/"?"":"/"));
  }
  manager.request(videoUrl);
  q.exec();
  game.videoData = manager.getData();
  game.videoFormat = videoUrl.right(3);
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
    if(info.suffix() == "lha") {
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
       config->platform == "mame-advmame" ||
       config->platform == "mame-libretro" ||
       config->platform == "mame-mame4all" ||
       config->platform == "fba") {
      baseName = NameTools::getMameName(baseName, mameMap);
    }
  }

  QList<QString> searchNames;

  if(baseName.isEmpty())
    return searchNames;

  searchNames.append(NameTools::getUrlQueryName(baseName));

  if(baseName.contains(":") || baseName.contains(" - ")) {
    QString noSubtitle = baseName.left(baseName.indexOf(":")).simplified();
    noSubtitle = noSubtitle.left(noSubtitle.indexOf("-")).simplified();
    // Only add if longer than 3. We don't want to search for "the" for instance
    if(noSubtitle.length() > 3)
      searchNames.append(NameTools::getUrlQueryName(noSubtitle));
  }

  if(NameTools::hasRomanNumeral(baseName))
    searchNames.append(NameTools::getUrlQueryName(NameTools::convertToIntegerNumeral(baseName)));

  if(NameTools::hasIntegerNumeral(baseName))
    searchNames.append(NameTools::getUrlQueryName(NameTools::convertToRomanNumeral(baseName)));

  return searchNames;
}

QString AbstractScraper::getCompareTitle(QFileInfo info)
{
  QString baseName = info.completeBaseName();

  if(config->scraper != "import") {
    if(info.suffix() == "lha") {
      baseName = NameTools::getNameWithSpaces(baseName);
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

  // Now create actual compareTitle
  baseName = baseName.replace("_", " ").left(baseName.indexOf("(")).left(baseName.indexOf("[")).simplified();

  // Always move The to the beginning of the name if found at the end
  if(baseName.right(5) == ", The") {
    baseName = baseName.left(baseName.indexOf(",")).prepend("The ");
  }
  
  return baseName;
}

void AbstractScraper::runPasses(QList<GameEntry> &gameEntries, const QFileInfo &info, QString &output, QString &debug)
{
  // Reset region priorities to original list
  regionPrios = regionPriosOrig;
  // Autodetect region and append to region priorities
  if(info.fileName().indexOf("(") != -1 && config->region.isEmpty()) {
    QString regionString = info.fileName().toLower().mid(info.fileName().indexOf("("), info.fileName().length());
    if(regionString.indexOf("europe") != -1) {
      regionPrios.prepend("eu");
    }
    if(regionString.indexOf("usa") != -1) {
      regionPrios.prepend("us");
    }
    if(regionString.indexOf("world") != -1) {
      regionPrios.prepend("wor");
    }
    if(regionString.indexOf("japan") != -1) {
      regionPrios.prepend("jp");
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

void AbstractScraper::setRegionPrios()
{
  if(!config->region.isEmpty() && config->region != "eu") {
    regionPrios.append(config->region);
  }
  regionPrios.append("eu");
  regionPrios.append("us");
  regionPrios.append("ss");
  regionPrios.append("uk");
  regionPrios.append("wor");
  regionPrios.append("jp");

  // Set original list so we can reset every time we do runPasses
  regionPriosOrig = regionPrios;
}

void AbstractScraper::setLangPrios()
{
  if(!config->lang.isEmpty() && config->lang != "en") {
    langPrios.append(config->lang);
  }
  langPrios.append("en");
}

bool AbstractScraper::platformMatch(QString found, QString platform) {
  foreach(QString p, Platform::getAliases(platform)) {
    if(found.toLower() == p) {
      return true;
    }
  }
  return false;
}

void AbstractScraper::loadMameMap()
{
  if(config->scraper != "import" &&
     (config->platform == "neogeo" ||
      config->platform == "arcade" ||
      config->platform == "mame-advmame" ||
      config->platform == "mame-libretro" ||
      config->platform == "mame-mame4all" ||
      config->platform == "fba")) {
    QFile mameMapFile("mameMap.csv");
    if(mameMapFile.open(QIODevice::ReadOnly)) {
      while(!mameMapFile.atEnd()) {
	QList<QByteArray> pair = mameMapFile.readLine().split(';');
	QString mameName = pair.at(0);
	QString realName = pair.at(1);
	mameName = mameName.replace("\"", "").simplified();
	realName = realName.replace("\"", "").simplified();
	mameMap[mameName] = realName;
      }
      mameMapFile.close();
    }
  }
}

void AbstractScraper::loadWhdLoadMap()
{
  if(config->platform == "amiga") {
    QDomDocument doc;

    QFile whdLoadFile;
    if(QFileInfo::exists("whdload_db.xml"))
      whdLoadFile.setFileName("whdload_db.xml");
    else if(QFileInfo::exists("/opt/retropie/emulators/amiberry/whdboot/game-data/whdload_db.xml"))
      whdLoadFile.setFileName("/opt/retropie/emulators/amiberry/whdboot/game-data/whdload_db.xml");
    else
      return;

    if(whdLoadFile.open(QIODevice::ReadOnly)) {
      QByteArray rawXml = whdLoadFile.readAll();
      whdLoadFile.close();
      if(doc.setContent(rawXml)) {
	QDomNodeList gameNodes = doc.elementsByTagName("game");
	for(int a = 0; a < gameNodes.length(); ++a) {
	  QDomNode gameNode = gameNodes.at(a);
	  QPair<QString, QString> gamePair;
	  gamePair.first = gameNode.firstChildElement("name").text();
	  gamePair.second = gameNode.firstChildElement("variant_uuid").text();
	  whdLoadMap[gameNode.toElement().attribute("filename")] = gamePair;
	}
      }
    }
  }
}
