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

#include <iostream>
#include <QTimer>

#include "scraperworker.h"
#include "strtools.h"
#include "nametools.h"
#include "settings.h"
#include "compositor.h"

#include "openretro.h"
#include "thegamesdb.h"
#include "worldofspectrum.h"
#include "screenscraper.h"
#include "igdb.h"
#include "mobygames.h"
#include "localscraper.h"
#include "importscraper.h"
#include "arcadedb.h"
#include "esgamelist.h"

ScraperWorker::ScraperWorker(QSharedPointer<Queue> queue, QSharedPointer<Cache> cache,
			     Settings config, QString threadId)
{
  this->config = config;
  this->cache = cache;
  this->queue = queue;
  this->threadId = threadId;
}

ScraperWorker::~ScraperWorker()
{
}

void ScraperWorker::run()
{
  if(config.scraper == "openretro") {
    scraper = new OpenRetro(&config);
  } else if(config.scraper == "thegamesdb") {
    scraper = new TheGamesDb(&config);
  } else if(config.scraper == "arcadedb") {
    scraper = new ArcadeDB(&config);
  } else if(config.scraper == "screenscraper") {
    scraper = new ScreenScraper(&config);
  } else if(config.scraper == "igdb") {
    scraper = new Igdb(&config);
  } else if(config.scraper == "mobygames") {
    scraper = new MobyGames(&config);
  } else if(config.scraper == "worldofspectrum") {
    scraper = new WorldOfSpectrum(&config);
  } else if(config.scraper == "esgamelist") {
    scraper = new ESGameList(&config);
  } else if(config.scraper == "cache") {
    scraper = new LocalScraper(&config);
  } else if(config.scraper == "import") {
    scraper = new ImportScraper(&config);
  } else {
    scraper = new AbstractScraper(&config);
  }

  platformOrig = config.platform;

  Compositor compositor(&config);
  if(!compositor.processXml()) {
    printf("Something went wrong when parsing artwork xml from '%s', please check the file for errors. Now exiting...\n", config.artworkConfig.toStdString().c_str());
    exit(1);
  }
  
  while(queue->hasEntry()) {
    // takeEntry() also unlocks the mutex that was locked in hasEntry()
    QFileInfo info = queue->takeEntry();
    // Reset platform in case we have manipulated it (such as changing 'amiga' to 'cd32')
    config.platform = platformOrig;
    QString output = "\033[1;33m(T" + threadId + ")\033[0m ";
    QString debug = "";
    QString sha1 = NameTools::getSha1(info);

    QString compareTitle = scraper->getCompareTitle(info);

    // For Amiga platform, change subplatforms from filename and add brackets if needed
    if(config.platform == "amiga") {
      if(info.completeBaseName().toLower().contains("cd32") ||
	 info.suffix() == "cue" || info.suffix() == "iso" || info.suffix() == "img") {
	debug.append("Platform change: 'amiga'->'cd32'\n");
	config.platform = "cd32";
      } else if(info.completeBaseName().toLower().contains("cdtv")) {
	debug.append("Platform change: 'amiga'->'cdtv', filename contains 'cdtv'\n");
	config.platform = "cdtv";
      }
    }

    QList<GameEntry> gameEntries;

    bool fromCache = false;
    if(config.scraper == "cache" && cache->hasEntries(sha1)) {
      fromCache = true;
      GameEntry localGame;
      localGame.sha1 = sha1;
      cache->fillBlanks(localGame);
      if(localGame.title.isEmpty()) {
	localGame.title = compareTitle;
      }
      if(localGame.platform.isEmpty()) {
	localGame.platform = config.platform;
      }
      gameEntries.append(localGame);
    } else {
      if(config.scraper != "cache" &&
	 cache->hasEntries(sha1, config.scraper) && !config.refresh) {
	fromCache = true;
	GameEntry localGame;
	localGame.sha1 = sha1;
	cache->fillBlanks(localGame, config.scraper);
	if(localGame.title.isEmpty()) {
	  localGame.title = compareTitle;
	}
	if(localGame.platform.isEmpty()) {
	  localGame.platform = config.platform;
	}
	gameEntries.append(localGame);
      } else {
	scraper->runPasses(gameEntries, info, output, debug);
      }
    }
    
    // Sort the returned entries, in case the source did a poor job of doing so itself
    // I disabled this as it messed up results containing dual-game results ("game 1 + game 2")
    /*
    qSort(gameEntries.begin(), gameEntries.end(), 
	  [](const GameEntry a, const GameEntry b) -> bool { return a.title.toLower() < b.title.toLower(); });
    */

    int lowestDistance = 666;
    // Create the game entry we use for the rest of the process
    GameEntry game;
    if(gameEntries.isEmpty()) {
      game.title = compareTitle;
      game.found = false;
    } else {
      game = getBestEntry(gameEntries, compareTitle, lowestDistance);
      if(config.interactive && !fromCache) {
	game = getEntryFromUser(gameEntries, game, compareTitle, lowestDistance);
      }
    }
    // Fill it with additional needed data
    game.path = info.absoluteFilePath();
    game.baseName = info.completeBaseName();
    game.sha1 = sha1;

    if(game.found == false) {
      output.append("\033[1;33m---- Game '" + info.completeBaseName() + "' not found :( ----\033[0m\n\n");
      game.resetMedia();
      if(!forceEnd)
	forceEnd = limitReached(output);
      emit entryReady(game, output, debug);
      if(forceEnd) {
	break;
      } else {
	continue;
      }
    }

    int searchMatch = getSearchMatch(game.title, compareTitle, lowestDistance);
    game.searchMatch = searchMatch;
    if(searchMatch < config.minMatch) {
      output.append("\033[1;33m---- Game '" + info.completeBaseName() + "' match too low :| ----\033[0m\n\n");
      game.found = false;
      game.resetMedia();
      if(!forceEnd)
	forceEnd = limitReached(output);
      emit entryReady(game, output, debug);
      if(forceEnd) {
	break;
      } else {
	continue;
      }
    }

    output.append("\033[1;34m---- Game '" + info.completeBaseName() + "' found! :) ----\033[0m\n");
    
    if(!fromCache) {
      scraper->getGameData(game);
    }

    if(!config.pretend && config.scraper == "cache") {
      // Process all artwork
      compositor.saveAll(game, info.completeBaseName());
      if(config.videos && game.videoFormat != "") {
	QString videoDst = config.videosFolder + "/" + info.completeBaseName() + "." + game.videoFormat;
	QFile videoFileDst(videoDst);
	if(videoFileDst.exists()) {
	  // Try to remove existing video destination file first
	  videoFileDst.remove();
	}
	if(config.symlink && config.scraper == "cache" && !game.videoFile.isEmpty()) {
	  QFile videoFile(game.videoFile);
	  if(videoFile.exists())
	    videoFile.link(videoDst);
	} else {
	  QFile videoFile(videoDst);
	  if(videoFile.open(QIODevice::WriteOnly)) {
	    videoFile.write(game.videoData);
	    videoFile.close();
	  }
	}
      }
    }

    // Add all resources to the cache
    if(config.scraper != "cache" && game.found && !fromCache) {
      game.source = config.scraper;
      cache->addResources(game, config);
    }

    // We're done saving the raw data at this point, so feel free to manipulate game resources to better suit game list creation from here on out.

    // Sort out brackets here, be sure to add brackets for both returned title and filename
    game.sqrNotes = NameTools::getSqrNotes(game.title);
    game.sqrNotes.append(NameTools::getSqrNotes(info.completeBaseName()));
    game.parNotes = NameTools::getParNotes(game.title);
    game.parNotes.append(NameTools::getParNotes(info.completeBaseName()));

    // Strip any brackets from the title as they will be readded when assembling gamelist
    game.title = StrTools::stripBrackets(game.title);

    // Move 'The' to end of title for better sorting
    // This looks like crap for some titles such as 'Simpsons: Bart vs. Blah, The' but I'll leave it in for the time being
    if(game.title.toLower().left(4) == "the ") {
      game.title = game.title.remove(0, 4).simplified().append(", The");
    }

    // Don't unescape title since we already did that in getBestEntry()
    game.videoFile = StrTools::xmlUnescape(config.videosFolder + "/" + info.completeBaseName() + "." + game.videoFormat);
    game.description = StrTools::xmlUnescape(game.description);
    game.releaseDate = StrTools::xmlUnescape(game.releaseDate);
    // Make sure we have the correct 'yyyymmdd' format of 'releaseDate'
    game.releaseDate = StrTools::conformReleaseDate(game.releaseDate);
    game.developer = StrTools::xmlUnescape(game.developer);
    game.publisher = StrTools::xmlUnescape(game.publisher);
    game.tags = StrTools::xmlUnescape(game.tags);
    game.tags = StrTools::conformTags(game.tags);
    game.rating = StrTools::xmlUnescape(game.rating);
    game.players = StrTools::xmlUnescape(game.players);
    // Make sure we have the correct single digit format of 'players'
    game.players = StrTools::conformPlayers(game.players);
    game.ages = StrTools::xmlUnescape(game.ages);
    // Make sure we have the correct format of 'ages'
    game.ages = StrTools::conformAges(game.ages);

    output.append("Scraper:        " + config.scraper + "\n");
    if(config.scraper != "cache" && config.scraper != "import") {
      output.append("From cache:     " + QString((fromCache?"YES (refresh from source with '--cache refresh')":"NO")) + "\n");
    }
    output.append("Search match:   " + QString::number(searchMatch) + " %\n");
    output.append("Compare title:  '\033[1;32m" + compareTitle + "\033[0m'\n");
    output.append("Result title:   '\033[1;32m" + game.title + "\033[0m' (" + game.titleSrc + ")\n");
    if(config.forceFilename) {
      game.title = StrTools::xmlUnescape(StrTools::stripBrackets(info.completeBaseName()));
    }
    output.append("Platform:       '\033[1;32m" + game.platform + "\033[0m' (" + game.platformSrc + ")\n");
    output.append("Release Date:   '\033[1;32m");
    if(game.releaseDate.isEmpty()) {
      output.append("\033[0m' ()\n");
    } else {
      output.append(QDate::fromString(game.releaseDate, "yyyyMMdd").toString("yyyy-MM-dd") + "\033[0m' (" + game.releaseDateSrc + ")\n");
    }
    output.append("Developer:      '\033[1;32m" + game.developer + "\033[0m' (" + game.developerSrc + ")\n");
    output.append("Publisher:      '\033[1;32m" + game.publisher + "\033[0m' (" + game.publisherSrc + ")\n");
    output.append("Players:        '\033[1;32m" + game.players + "\033[0m' (" + game.playersSrc + ")\n");
    output.append("Ages:           '\033[1;32m" + game.ages + (game.ages.toInt() != 0?"+":"") + "\033[0m' (" + game.agesSrc + ")\n");
    output.append("Tags:           '\033[1;32m" + game.tags + "\033[0m' (" + game.tagsSrc + ")\n");
    output.append("Rating (0-1):   '\033[1;32m" + game.rating + "\033[0m' (" + game.ratingSrc + ")\n");
    output.append("Cover:          " + QString((game.coverData.isNull()?"\033[1;31mNO":"\033[1;32mYES")) + "\033[0m" + QString((config.cacheCovers?"":" (uncached)")) + " (" + game.coverSrc + ")\n");
    output.append("Screenshot:     " + QString((game.screenshotData.isNull()?"\033[1;31mNO":"\033[1;32mYES")) + "\033[0m" + QString((config.cacheScreenshots?"":" (uncached)")) + " (" + game.screenshotSrc + ")\n");
    output.append("Wheel:          " + QString((game.wheelData.isNull()?"\033[1;31mNO":"\033[1;32mYES")) + "\033[0m" + QString((config.cacheWheels?"":" (uncached)")) + " (" + game.wheelSrc + ")\n");
    output.append("Marquee:        " + QString((game.marqueeData.isNull()?"\033[1;31mNO":"\033[1;32mYES")) + "\033[0m" + QString((config.cacheMarquees?"":" (uncached)")) + " (" + game.marqueeSrc + ")\n");
    if(config.videos) {
      output.append("Video:          " + QString((game.videoFormat.isEmpty()?"\033[1;31mNO":"\033[1;32mYES")) + "\033[0m (" + game.videoSrc + ")\n");
    }
    output.append("\nDescription: (" + game.descriptionSrc + ")\n" + game.description.left(config.maxLength) + "\n");

    if(!forceEnd)
      forceEnd = limitReached(output);
    emit entryReady(game, output, debug);
    if(forceEnd) {
      break;
    }
  }

  delete scraper;
  emit allDone();
}

bool ScraperWorker::limitReached(QString &output)
{
  if(scraper->reqRemaining != -1) { // -1 means there is no limit
    if(scraper->reqRemaining > 0) {
    output.append("\n\033[1;33m'" + config.scraper + "' requests remaining: " + QString::number(scraper->reqRemaining) + "\033[0m\n");
    } else {
      output.append("\033[1;31mForcing thread " + threadId + " to stop...\033[0m\n");
      return true;
    }
  }
  return false;
}

int ScraperWorker::getSearchMatch(const QString &title, const QString &compareTitle,
				  const int &lowestDistance)
{
  int searchMatch = 0;
  if(title.length() > compareTitle.length()) {
    searchMatch = (int)(100.0 / (double)title.length() *
			((double)title.length() - (double)lowestDistance));
  } else {
    searchMatch = (int)(100.0 / (double)compareTitle.length() *
			((double)compareTitle.length() - (double)lowestDistance));
  }
  // Special case where result is actually correct, but gets low match because of the prepending of, for instance, "Disney's [title]" where the fileName is just "[title]". Accept these if searchMatch is 'similar enough' (above 50)
  if(searchMatch < config.minMatch) {
    if(title.toLower().indexOf(compareTitle.toLower()) != -1 && searchMatch >= 50) {
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

GameEntry ScraperWorker::getBestEntry(const QList<GameEntry> &gameEntries,
				      QString compareTitle,
				      int &lowestDistance)
{
  GameEntry game;

  // If scraper isn't filename search based, always return first entry
  if(config.scraper == "cache" || config.scraper == "import" ||
     config.scraper == "arcadedb" || config.scraper == "screenscraper" ||
     config.scraper == "esgamelist" ||
     (config.scraper == "openretro" && gameEntries.first().url.isEmpty())) {
    lowestDistance = 0;
    game = gameEntries.first();
    game.title = StrTools::xmlUnescape(game.title);
    return game;
  }

  QList<GameEntry> potentials;
  
  int compareNumeral = NameTools::getNumeral(compareTitle);
  // Start by applying rules we are certain are needed. Add the ones that pass to potentials
  foreach(GameEntry entry, gameEntries) {
    entry.title = StrTools::xmlUnescape(entry.title);
    
    int entryNumeral = NameTools::getNumeral(entry.title);
    // If numerals don't match, skip.
    // Numeral defaults to 1, even for games without a numeral.
    if(compareNumeral != entryNumeral) {
      // Special cases
      if(compareTitle.toLower().indexOf("day of the tentacle") == -1 &&
	 compareTitle.toLower().indexOf("curse of monkey island") == -1) {
	continue;
      }
    }
    // Remove all brackets from name, since we pretty much NEVER want these and they fuck up
    // our listings when using mamenames for 'arcade' (everything becomes double)
    if(config.scraper != "openretro") {
      entry.title = entry.title.left(entry.title.indexOf("(")).simplified();
      entry.title = entry.title.left(entry.title.indexOf("[")).simplified();
    }
    
    potentials.append(entry);
  }
  
  // If we have no potentials at all, return false
  if(potentials.isEmpty()) {
    game.found = false;
    return game;
  }
  
  int mostSimilar = 0;
  // Run through the potentials and find the best match
  for(int a = 0; a < potentials.length(); ++a) {
    QString entryTitle = potentials.at(a).title;

    // If we have a perfect hit, always use this result
    if(compareTitle == entryTitle) {
      lowestDistance = 0;
      game = potentials.at(a);
      return game;
    }

    // Check if game is exact match if "The" at either end is manipulated
    bool match = false;
    if(compareTitle.toLower().right(5) == ", the" && entryTitle.toLower().left(4) == "the ") {
      if(compareTitle.toLower().left(compareTitle.length() - 5) ==
	 entryTitle.toLower().remove(0, 4)) {
	match = true;
      }
    }
    if(entryTitle.toLower().right(5) == ", the" && compareTitle.toLower().left(4) == "the ") {
      if(entryTitle.toLower().left(entryTitle.length() - 5) ==
	 compareTitle.toLower().remove(0, 4)) {
	match = true;
      }
    }
    if(match) {
      lowestDistance = 0;
      game = potentials.at(a);
      return game;
    }

    
    // Compare all words of compareTitle and entryTitle. If all words with a length of more than 3 letters are found in the entry words, return match
    QList<QString> compareWords = compareTitle.toLower().simplified().split(" ");
    for(int b = 0; b < compareWords.size(); ++b) {
      if(compareWords.at(b).length() < 3)
	compareWords.removeAt(b);
    }
    QList<QString> entryWords = entryTitle.toLower().simplified().split(" ");
    for(int b = 0; b < entryWords.size(); ++b) {
      if(entryWords.at(b).length() < 3)
	entryWords.removeAt(b);
    }
    // Only perform check if there's 3 or more words in compareTitle
    if(compareWords.size() >= 3) {
      int wordsFound = 0;
      foreach(QString compareWord, compareWords) {
	foreach(QString entryWord, entryWords) {
	  if(entryWord == compareWord) {
	    wordsFound++;
	    break;
	  }
	}
      }
      if(wordsFound == compareWords.size()) {
	lowestDistance = 0;
	game = potentials.at(a);
	return game;
      }
    }
    // Only perform check if there's 3 or more words in entryTitle
    if(entryWords.size() >= 3) {
      int wordsFound = 0;
      foreach(QString entryWord, entryWords) {
	foreach(QString compareWord, compareWords) {
	  if(compareWord == entryWord) {
	    wordsFound++;
	    break;
	  }
	}
      }
      if(wordsFound == entryWords.size()) {
	lowestDistance = 0;
	game = potentials.at(a);
	return game;
      }
    }

    // If only one title has a subtitle (eg. has ":" or similar in name), remove subtitle from
    // the other if length differs more than 4 in order to have a better chance of a match.
    bool compareHasSub = (compareTitle.indexOf(":") != -1 ||
			  compareTitle.indexOf(" - ") != -1  ||
			  compareTitle.indexOf("~") != -1 );
    bool entryHasSub = (entryTitle.indexOf(":") != -1  ||
			entryTitle.indexOf(" - ") != -1  ||
			entryTitle.indexOf("~") != -1 );
    int lengthDiff = abs(entryTitle.length() - compareTitle.length());
    if(lengthDiff > 4) {
      QString comLower = compareTitle.toLower().simplified();
      QString entLower = entryTitle.toLower().simplified();
      if(entryHasSub && !compareHasSub) {
	// Before cutting subtitle, check if subtitle is actually game title
	if(entLower.right(comLower.length()) == comLower && comLower.length() >= 10) {
	  lowestDistance = 0;
	  game = potentials.at(a);
	  return game;
	}
	entryTitle = entryTitle.left(entryTitle.indexOf(":")).simplified();
	entryTitle = entryTitle.left(entryTitle.indexOf(" - ")).simplified();
	entryTitle = entryTitle.left(entryTitle.indexOf("~")).simplified();
      } else if(compareHasSub && !entryHasSub) {
	// Before cutting subtitle, check if subtitle is actually game title
	if(comLower.right(entLower.length()) == entLower && entLower.length() >= 10) {
	  lowestDistance = 0;
	  game = potentials.at(a);
	  return game;
	}
	compareTitle = compareTitle.left(compareTitle.indexOf(":")).simplified();
	compareTitle = compareTitle.left(compareTitle.indexOf(" - ")).simplified();
	compareTitle = compareTitle.left(compareTitle.indexOf("~")).simplified();
      }
    }

    int currentDistance =
      editDistance(StrTools::xmlUnescape(compareTitle).toLower().toStdString(),
		   StrTools::xmlUnescape(entryTitle).toLower().toStdString());
    if(currentDistance < lowestDistance) {
      lowestDistance = currentDistance;
      mostSimilar = a;
    }
  }
  game = potentials.at(mostSimilar);
  return game;
}

GameEntry ScraperWorker::getEntryFromUser(const QList<GameEntry> &gameEntries,
					  const GameEntry &suggestedGame,
					  const QString &compareTitle,
					  int &lowestDistance)
{
  GameEntry game;

  std::string entryStr = "";
  printf("Potential entries for '\033[1;32m%s\033[0m':\n", compareTitle.toStdString().c_str());
  bool suggestedShown = false;
  for(int a = 1; a <= gameEntries.length(); ++a) {
    QString suggested = "";
    if(gameEntries.at(a - 1).title == suggestedGame.title && !suggestedShown) {
      suggested = " <-- Skyscraper's choice";
      suggestedShown = true;
    }
    printf("\033[1;32m%d%s\033[0m: Title:    '\033[1;32m%s\033[0m'%s\n    platform: '\033[1;33m%s\033[0m'\n", a, QString((a <= 9?" ":"")).toStdString().c_str(), gameEntries.at(a - 1).title.toStdString().c_str(), suggested.toStdString().c_str(), gameEntries.at(a - 1).platform.toStdString().c_str());
  }
  printf("\033[1;32m-1\033[0m: \033[1;33mNONE OF THE ABOVE!\033[0m\n");
  printf("\033[1;34mPlease choose the preferred entry\033[0m (Or press enter to let Skyscraper choose):\033[0m ");
  getline(std::cin, entryStr);
  printf("\n");
  // Becomes 0 if input is not a number
  int chosenEntry = QString(entryStr.c_str()).toInt();

  if(entryStr != "") {
    if(chosenEntry == -1) {
      game.title = "Entries discarded by user";
      game.found = false;
      return game;
    } else if(chosenEntry != 0) {
      lowestDistance = 0;
      game = gameEntries.at(chosenEntry - 1);
      return game;
    }
  }

  return suggestedGame;
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
