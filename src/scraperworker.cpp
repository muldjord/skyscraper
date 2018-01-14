/***************************************************************************
 *            scraperworker.cpp
 *
 *  Wed Jun 7 12:00:00 CEST 2017
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

#include <QTimer>

#include "scraperworker.h"
#include "strtools.h"
#include "settings.h"
#include "compositor.h"

#include "openretro.h"
#include "thegamesdb.h"
#include "worldofspectrum.h"
#include "screenscraper.h"
#include "localscraper.h"
#include "importscraper.h"
#include "arcadedb.h"

ScraperWorker::ScraperWorker(QList<QFileInfo> inputFiles, int filesPerThread, int beginIdx,
			     Settings config, QSharedPointer<LocalDb> localDb)
{
  this->config = config;
  this->localDb = localDb;
  this->inputFiles = inputFiles;
  this->filesPerThread = filesPerThread;
  this->beginIdx = beginIdx;
}

ScraperWorker::~ScraperWorker()
{
}

void ScraperWorker::run()
{
  AbstractScraper *scraper;

  if(config.scraper == "openretro") {
    scraper = new OpenRetro();
  } else if(config.scraper == "thegamesdb") {
    scraper = new TheGamesDb();
  } else if(config.scraper == "arcadedb") {
    scraper = new ArcadeDB();
  } else if(config.scraper == "screenscraper") {
    scraper = new ScreenScraper();
  } else if(config.scraper == "worldofspectrum") {
    scraper = new WorldOfSpectrum();
  } else if(config.scraper == "localdb") {
    scraper = new LocalScraper();
  } else if(config.scraper == "import") {
    scraper = new ImportScraper();
  } else {
    scraper = new AbstractScraper();
  }

  scraper->setConfig(&config);
  scraper->loadMameMap();
  
  platformOrig = config.platform;

  Compositor artHandler(config);
  artHandler.processXml("artwork.xml");
  
  for(int a = beginIdx; a < filesPerThread + beginIdx; ++a) {
    output = "";
    config.platform = platformOrig;
    QFileInfo info = inputFiles.at(a);
    QString parNotes = "";
    QString sqrNotes = "";
    QString sha1 = getSha1(info);

    QString compareName = scraper->getCompareName(info.completeBaseName(), sqrNotes, parNotes);

    // Special markings for the platform, for instance 'AGA'
    QString marking = ""; // No marking is default
    // Special for Amiga platform where there are subplatforms in filename
    if(config.platform == "amiga") {
      if(info.completeBaseName().toLower().indexOf("cd32") != -1) {
	config.platform = "cd32";
      } else if(info.completeBaseName().toLower().indexOf("cdtv") != -1) {
	config.platform = "cdtv";
      } else if(info.completeBaseName().toLower().indexOf("aga") != -1) {
	marking = "+aga";
	config.platform = "aga";
      }
    }
    
    QList<GameEntry> gameEntries;
    
    bool prefilledFromCache = false;
    if(config.localDb && config.scraper == "localdb" && localDb->hasEntries(sha1)) {
      prefilledFromCache = true;
      GameEntry localGame;
      localGame.sha1 = sha1;
      localDb->fillBlanks(localGame);
      if(localGame.title.isEmpty()) {
	localGame.title = compareName;
      }
      if(localGame.platform.isEmpty()) {
	localGame.platform = config.platform;
      }
      gameEntries.append(localGame);
    } else {
      if(config.localDb && config.scraper != "localdb" &&
	 localDb->hasEntries(sha1, config.scraper) && !config.updateDb) {
	prefilledFromCache = true;
	GameEntry localGame;
	localGame.sha1 = sha1;
	localDb->fillBlanks(localGame, config.scraper);
	if(localGame.title.isEmpty()) {
	  localGame.title = compareName;
	}
	if(localGame.platform.isEmpty()) {
	  localGame.platform = config.platform;
	}
	gameEntries.append(localGame);
      } else {
	scraper->runPasses(gameEntries, info, output, marking);
      }
    }

    unsigned int lowestDistance = 666;
    GameEntry game = getBestEntry(gameEntries, compareName, lowestDistance);
    game.path = info.absoluteFilePath();
    game.baseName = info.completeBaseName();
    game.sha1 = sha1;
    game.parNotes = parNotes;
    game.sqrNotes = sqrNotes;

    if(!game.found) {
      output.append("\033[1;33m---- Game '" + info.completeBaseName() + "' not found :( ----\033[0m\n\n");
      emit outputToTerminal(output);
      emit entryReady(game);
      continue;
    }
    int searchMatch = getSearchMatch(game.title, compareName, lowestDistance);
    game.searchMatch = searchMatch;
    if(searchMatch < config.minMatch) {
      output.append("\033[1;33m---- Game '" + info.completeBaseName() + "' match too low :| ----\033[0m\n\n");
      game.found = false;
      emit outputToTerminal(output);
      emit entryReady(game);
      continue;
    }
    output.append("\033[1;34m---- Game '" + info.completeBaseName() + "' found! :) ----\033[0m\n");
    
    if(!prefilledFromCache) {
      scraper->getGameData(game);
    }

    if(!config.pretend) {
      artHandler.saveAll(game, info.completeBaseName());
      if(config.videos && game.videoFormat != "") {
	QFile videoFile(config.videosFolder + "/" + info.completeBaseName() + "." + game.videoFormat);
	if(videoFile.open(QIODevice::WriteOnly)) {
	  videoFile.write(game.videoData);
	  videoFile.close();
	}
      }
    }

    // Always cache resources, even if pretend is set
    if(config.localDb && config.scraper != "localdb" && game.found) {
      game.source = config.scraper;
      localDb->addResources(game, config);
    }

    // We're done saving the raw data at this point, so feel free to manipulate game resources to better suit game list creation from here on out.

    if(game.title.toLower().left(4) == "the ") {
      game.title = game.title.remove(0, 4).simplified().append(", The");
    }

    game.title = StrTools::xmlUnescape(game.title);
    if(config.forceFilename) {
      game.title = StrTools::xmlUnescape(info.completeBaseName().left(info.completeBaseName().indexOf("(")).left(info.completeBaseName().indexOf("[")).replace("_", " ").simplified());
    }
    game.videoFile = StrTools::xmlUnescape(config.videosFolder + "/" + info.completeBaseName() + "." + game.videoFormat);
    game.description = StrTools::xmlUnescape(game.description);
    game.releaseDate = StrTools::xmlUnescape(game.releaseDate);
    game.developer = StrTools::xmlUnescape(game.developer);
    game.publisher = StrTools::xmlUnescape(game.publisher);
    game.tags = StrTools::xmlUnescape(game.tags);
    game.rating = StrTools::xmlUnescape(game.rating);
    game.players = StrTools::xmlUnescape(game.players);

    if(config.verbose) {
      output.append("Scraper:\t'" + config.scraper + "'\n");
    }
    output.append("Search match:\t" + QString::number(searchMatch) + " %\n");
    output.append("Game title:\t'" + game.title + "'\n");
    output.append("Compare title:\t'" + compareName + "'\n");
    output.append("Platform:\t'" + game.platform + "'\n");
    output.append("Release Date:\t'");
    if(game.releaseDate.isEmpty()) {
      output.append("'\n");
    } else {
      if(game.releaseDate.toInt() == 0) {
	output.append("Unknown format (" + game.releaseDate + ")'\n");
      } else {
	output.append(QDate::fromString(game.releaseDate, "yyyyMMdd").toString("yyyy-MM-dd") + "'\n");
      }
    }
    output.append("Developer:\t'" + game.developer + "'\n");
    output.append("Publisher:\t'" + game.publisher + "'\n");
    output.append("Players:\t'" + game.players + "'\n");
    output.append("Tags:\t\t'" + game.tags + "'\n");
    output.append("Rating (0-1):\t'" + game.rating + "'\n");
    output.append("Cover:\t\t" + QString((game.coverData.isNull()?"\033[1;31mNO":"\033[1;32mYES")) + "\033[0m" + QString((config.cacheCovers?"":" (uncached)")) + "\n");
    output.append("Screenshot:\t" + QString((game.screenshotData.isNull()?"\033[1;31mNO":"\033[1;32mYES")) + "\033[0m" + QString((config.cacheScreenshots?"":" (uncached)")) + "\n");
    output.append("Wheel:\t\t" + QString((game.wheelData.isNull()?"\033[1;31mNO":"\033[1;32mYES")) + "\033[0m" + QString((config.cacheWheels?"":" (uncached)")) + "\n");
    output.append("Marquee:\t" + QString((game.marqueeData.isNull()?"\033[1;31mNO":"\033[1;32mYES")) + "\033[0m" + QString((config.cacheMarquees?"":" (uncached)")) + "\n");
    if(config.videos) {
      output.append("Video:\t\t" + QString((game.videoFormat.isEmpty()?"\033[1;31mNO":"\033[1;32mYES")) + "\033[0m\n");
    }
    output.append("\nDescription:\n" + game.description.left(config.maxLength) + "\n");

    emit outputToTerminal(output);
    emit entryReady(game);
  }

  delete scraper;
  emit allDone();
}

int ScraperWorker::getSearchMatch(const QString &title, const QString &compareName,
				  const int &lowestDistance)
{
  int searchMatch = 0;
  if(title.length() > compareName.length()) {
    searchMatch = (int)(100.0 / (double)title.length() *
			((double)title.length() - (double)lowestDistance));
  } else {
    searchMatch = (int)(100.0 / (double)compareName.length() *
			((double)compareName.length() - (double)lowestDistance));
  }
  // Special case where result is actually correct, but gets low match because of the prepending of, for instance, "Disney's [title]" where the fileName is just "[title]". Accept these if searchMatch is 'similar enough' (above 50)
  if(searchMatch < config.minMatch) {
    if(title.toLower().indexOf(compareName.toLower()) != -1 && searchMatch >= 50) {
      searchMatch = 100;
    }
  }
  return searchMatch;
}

unsigned int ScraperWorker::editDistance(const std::string& s1, const std::string& s2) 
{
  const std::size_t len1 = s1.size(), len2 = s2.size();
  std::vector<unsigned int> col(len2+1), prevCol(len2+1);
	
  for (unsigned int i = 0; i < prevCol.size(); i++)
    prevCol[i] = i;
  for (unsigned int i = 0; i < len1; i++) {
    col[0] = i+1;
    for (unsigned int j = 0; j < len2; j++)
      // note that std::min({arg1, arg2, arg3}) works only in C++11,
      // for C++98 use std::min(std::min(arg1, arg2), arg3)
      col[j+1] = std::min({ prevCol[1 + j] + 1, col[j] + 1, prevCol[j] + (s1[i]==s2[j] ? 0 : 1) });
    col.swap(prevCol);
  }
  return prevCol[len2];
}

QString ScraperWorker::getSha1(const QFileInfo &info)
{
  QCryptographicHash sha1(QCryptographicHash::Sha1);

  // If file is some sort of script or a zip file, use filename for sha1
  bool sha1FromData = true;
  if(info.suffix() == "uae" || info.suffix() == "cue" ||
     info.suffix() == "sh" || info.suffix() == "svm" ||
     info.suffix() == "mds" || info.suffix() == "svm" ||
     info.suffix() == "zip") {
    sha1FromData = false;
  }
  // If file is larger than 50 MBs, use filename for sha1
  if(info.size() > 52428800) {
    sha1FromData = false;
  }

  if(sha1FromData) {
    QFile romFile(info.absoluteFilePath());
    if(romFile.open(QIODevice::ReadOnly)) {
      while(!romFile.atEnd()) {
	sha1.addData(romFile.read(1024));
      }
      romFile.close();
    } else {
      printf("Couldn't calculate sha1 hash sum of rom file '%s', please check permissions and try again, now exiting...\n", info.fileName().toStdString().c_str());
      exit(1);
    }
  } else {
    sha1.addData(info.fileName().toUtf8());
  }

  return sha1.result().toHex();
}

GameEntry ScraperWorker::getBestEntry(const QList<GameEntry> &gameEntries,
				      const QString &compareName, unsigned int &lowestDistance)
{
  GameEntry game;
  if(gameEntries.isEmpty()) {
    game.found = false;
    game.title = compareName;
    return game;
  }

  int mostSimilar = 0;
  for(int a = 0; a < gameEntries.length(); ++a) {
    QString currentTitle = gameEntries.at(a).title;
    // If we have a perfect hit, always use this result
    if(compareName == currentTitle) {
      lowestDistance = 0;
      mostSimilar = a;
      break;
    }
    // Check if game is a sequel
    if(compareName.right(1) == "I" ||
       compareName.right(1) == "V" ||
       compareName.right(1) == "X" ||
       compareName.right(1).toInt() != 0) {
      // Remove everything after a ':' in web result title, as it often isn't part of the filename, giving bad compare results. But only do so if length of strings vary more than 4, otherwise filename might have subname included, but perhaps with ' - ' or similar instead of ':'
      if(currentTitle.indexOf(":") != -1 && abs(currentTitle.length() - compareName.length()) > 4) {
	currentTitle = currentTitle.left(currentTitle.indexOf(":"));
      }
      // Check if web title is similar to compareName without ' I'
      // eg 'Tomb Raider' is equal to 'Tomb Raider I'
      if(compareName.right(2) == " I" &&
	 currentTitle == compareName.left(compareName.length() - 2)) {
	lowestDistance = 0;
	mostSimilar = a;
	break;
      }
    }
    // The reason for using .right in the second parameter is to better hit results such as 'maniac mansion II: day of the tentacle' where the filename might be 'day of the tentacle'.
    unsigned int currentDistance =
      editDistance(StrTools::xmlUnescape(compareName).toLower().toStdString(),
		   StrTools::xmlUnescape(currentTitle).right(StrTools::xmlUnescape(compareName).length()).toLower().toStdString());
    if(currentDistance < lowestDistance) {
      lowestDistance = currentDistance;
      mostSimilar = a;
    }
  }
  game = gameEntries.at(mostSimilar);
  return game;
}

// --- Console colors ---
// Black        0;30     Dark Gray     1;30
// Red          0;31     Light Red     1;31
// Green        0;32     Light Green   1;32
// Brown/Orange 0;33     Yellow        1;33
// Blue         0;34     Light Blue    1;34
// Purple       0;35     Light Purple  1;35
// Cyan         0;36     Light Cyan    1;36
// Light Gray   0;37     White         1;37
