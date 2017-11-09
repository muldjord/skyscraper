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

AbstractScraper::AbstractScraper()
{
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
  //qDebug("URL IS: '%s'\n", game.url.toStdString().c_str());
  //qDebug("DATA IS:\n'%s'\n", data.data());

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

  game.description = data.left(data.indexOf(descriptionPost));

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
  QString releaseDate = data.left(data.indexOf(releaseDatePost)).simplified();
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
  if(coverUrl.indexOf("http") != -1) {
    manager.request(coverUrl);
  } else {
    manager.request(baseUrl + (coverUrl.left(1) == "/"?"":"/") + coverUrl);
  }
  q.exec();
  QImage image(QImage::fromData(manager.getData()));
  if(!image.isNull()) {
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
    if(screenshotUrl.indexOf("http") != -1) {
      manager.request(screenshotUrl);
    } else {
      manager.request(baseUrl + (screenshotUrl.left(1) == "/"?"":"/") + screenshotUrl);
    }
    q.exec();
    QImage image(QImage::fromData(manager.getData()));
    if(!image.isNull()) {
      game.screenshotData = image;
    }
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
  if(videoUrl.indexOf("http") != -1) {
    manager.request(videoUrl);
  } else {
    manager.request(baseUrl + (videoUrl.left(1) == "/"?"":"/") + videoUrl);
  }
  q.exec();
  game.videoData = manager.getData();
  game.videoFormat = videoUrl.right(3);
}

QString AbstractScraper::actualPlatform(QString platform)
{
  return platform;
}

void AbstractScraper::nomNom(const QString nom, bool including)
{
  data.remove(0, data.indexOf(nom) + (including?nom.length():0));
  //qDebug("Parser at: '%s'\n", data.left(20).data());
}

bool AbstractScraper::checkNom(const QString nom)
{
  if(data.indexOf(nom) != -1) {
    return true;
  }
  return false;
}

QString AbstractScraper::getSearchName(QString baseName)
{
  if(config->scraper != "mamedb" &&
     config->scraper != "import" &&
     (config->platform == "neogeo" || config->platform == "arcade")) {
    for(int a = 0; a < mameMap.length(); ++a) {
      if(baseName == mameMap.at(a).first) {
	baseName = mameMap.at(a).second;
	break;
      }
    }
  }

  // If we have the first game in a series, remove the ' I' for more search results
  if(baseName.right(2) == " I") {
    baseName = baseName.left(baseName.length() - 2);
  }
  baseName = baseName.
    toLower().
    replace(" - ", "-");
  baseName = baseName.left(baseName.indexOf("("));
  baseName = baseName.left(baseName.indexOf("["));
  // Always remove 'the' from beginning or end
  if(baseName.simplified().left(4) == "the ") {
    baseName = baseName.simplified().remove(0, 4);
  }
  if(baseName.simplified().right(5) == ", the") {
    baseName = baseName.simplified().left(baseName.indexOf(", the"));
  }
  baseName = baseName.replace(",", " ").replace("&", "%26").replace("+", "").
    replace("s's", "s'").replace("'", "%27");
  ;
  baseName = baseName.simplified().replace("_", "+").replace(" ", "+");
  
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


  return baseName;
}

QString AbstractScraper::getCompareName(QString baseName, QString &sqrNotes, QString &parNotes)
{
  if(config->scraper != "mamedb" &&
     config->scraper != "import" &&
     (config->platform == "neogeo" || config->platform == "arcade")) {
    for(int a = 0; a < mameMap.length(); ++a) {
      if(baseName == mameMap.at(a).first) {
	baseName = mameMap.at(a).second;
	break;
      }
    }
  }

  QString baseNameOrig = baseName;
  // Get square notes
  while(baseName.indexOf("[") != -1 && baseName.indexOf("]") != -1) {
    if(baseName.indexOf("[") != -1 && baseName.indexOf("]") != -1) {
      sqrNotes.append(baseName.mid(baseName.indexOf("["),
				  baseName.indexOf("]") - baseName.indexOf("[") + 1));
    }
    baseName.remove(baseName.indexOf("["),
		    baseName.indexOf("]") - baseName.indexOf("[") + 1);
  }
  sqrNotes = sqrNotes.simplified();

  // Reset
  baseName = baseNameOrig;

  // Get parenthesis notes  
  while(baseName.indexOf("(") != -1 && baseName.indexOf(")") != -1) {
    if(baseName.indexOf("(") != -1 && baseName.indexOf(")") != -1) {
      parNotes.append(baseName.mid(baseName.indexOf("("),
				  baseName.indexOf(")") - baseName.indexOf("(") + 1) + " ");
    }
    baseName.remove(baseName.indexOf("("),
		    baseName.indexOf(")") - baseName.indexOf("(") + 1);
  }
  parNotes = parNotes.simplified();

  // Reset
  baseName = baseNameOrig;
  
  // Now create actual compareName
  baseName = baseName.replace("_", " ").left(baseName.indexOf("(")).left(baseName.indexOf("[")).simplified();

  if(baseName.right(5) == ", The") {
    baseName = baseName.left(baseName.indexOf(",")).prepend("The ");
  }
  
  return baseName;
}

void AbstractScraper::runPasses(QList<GameEntry> &gameEntries, const QFileInfo &info, QString &output, QString &marking)
{
  QString searchName = getSearchName(info.completeBaseName());
  QString searchNameOrig = searchName;
  
  for(int pass = 1; pass <= 6; ++pass) {
    // Reset searchName for each pass
    searchName = searchNameOrig;
    output.append("\033[1;35mPass " + QString::number(pass) + "\033[0m ");
    switch(pass) {
    case 1:
      searchName += marking;
      getSearchResults(gameEntries, searchName, config->platform);
      break;
    case 2:
      getSearchResults(gameEntries, searchName, config->platform);
      break;
    case 3:
      if(config->platform == "cd32" || config->platform == "cdtv") {
	// 
	config->platform = "amiga";
	getSearchResults(gameEntries, searchName, config->platform);
      }
      break;
    case 4:
      // Try removing dots
      searchName = searchName.replace(".", "");
      getSearchResults(gameEntries, searchName, config->platform);
      break;
    case 5:
      // Try replacing apostrophes with spaces
      searchName = searchName.replace("%27", " ");
      getSearchResults(gameEntries, searchName, config->platform);
      break;
    case 6:
      // Remove everything after a dash
      searchName = searchName.left(searchName.indexOf("-"));
      getSearchResults(gameEntries, searchName, config->platform);
      break;
    default:
      ;
    }
    if(config->verbose) {
      output.append("searchName: '" + searchName + "'\n");
      output.append("platform: '" + config->platform + "'\n");
    }
    if(!gameEntries.isEmpty()) {
      break;
    }
  }
}

void AbstractScraper::setConfig(Settings *config)
{
  this->config = config;
}

bool AbstractScraper::platformMatch(QString found, QString platform) {
  QStringList platforms = actualPlatform(platform).split(";");
  foreach(QString p, platforms) {
    if(found.toLower() == p) {
      return true;
    }
  }
  return false;
}

void AbstractScraper::loadMameMap()
{
  if(config->scraper != "mamedb" &&
     config->scraper != "import" &&
     (config->platform == "neogeo" || config->platform == "arcade")) {
    QFile mameMapFile("mameMap.csv");
    if(mameMapFile.open(QIODevice::ReadOnly)) {
      while(!mameMapFile.atEnd()) {
	QList<QByteArray> pairs = mameMapFile.readLine().split(';');
	mameMap.append(QPair<QString, QString> (QString::fromUtf8(pairs.at(0)).replace("\"", "").simplified(), QString::fromUtf8(pairs.at(1)).replace("\"", "").simplified()));
      }
      mameMapFile.close();
    }
  }
}
