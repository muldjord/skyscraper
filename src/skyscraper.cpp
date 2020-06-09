/***************************************************************************
 *            skyscraper.cpp
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

#include <QThread>
#include <QSettings>
#include <QDirIterator>
#include <QTimer>
#include <QMutexLocker>
#include <QProcess>
#include <QDomDocument>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#if QT_VERSION >= 0x050400
#include <QStorageInfo>
#endif

#include "skyscraper.h"
#include "strtools.h"

#include "emulationstation.h"
#include "attractmode.h"
#include "pegasus.h"

Skyscraper::Skyscraper(const QCommandLineParser &parser, const QString &currentDir)
{
  qRegisterMetaType<GameEntry>("GameEntry");

  // Randomize timer
  qsrand(QTime::currentTime().msec());
  
  printf("%s", StrTools::getVersionHeader().toStdString().c_str());

  config.currentDir = currentDir;
  loadConfig(parser);
}

Skyscraper::~Skyscraper()
{
  delete frontend;
}

void Skyscraper::run()
{
  printf("Platform:           '\033[1;32m%s\033[0m'\n", config.platform.toStdString().c_str());
  printf("Scraping module:    '\033[1;32m%s\033[0m'\n", config.scraper.toStdString().c_str());
  if(config.scraper == "cache") {
    printf("Frontend:           '\033[1;32m%s\033[0m'\n", config.frontend.toStdString().c_str());
    if(!config.frontendExtra.isEmpty()) {
      printf("Extra:              '\033[1;32m%s\033[0m'\n", config.frontendExtra.toStdString().c_str());
    }
  }
  printf("Input folder:       '\033[1;32m%s\033[0m'\n", config.inputFolder.toStdString().c_str());
  printf("Game list folder:   '\033[1;32m%s\033[0m'\n", config.gameListFolder.toStdString().c_str());
  printf("Covers folder:      '\033[1;32m%s\033[0m'\n", config.coversFolder.toStdString().c_str());
  printf("Screenshots folder: '\033[1;32m%s\033[0m'\n", config.screenshotsFolder.toStdString().c_str());
  printf("Wheels folder:      '\033[1;32m%s\033[0m'\n", config.wheelsFolder.toStdString().c_str());
  printf("Marquees folder:    '\033[1;32m%s\033[0m'\n", config.marqueesFolder.toStdString().c_str());
  if(config.videos) {
    printf("Videos folder:      '\033[1;32m%s\033[0m'\n", config.videosFolder.toStdString().c_str());
  }
  printf("Cache folder:       '\033[1;32m%s\033[0m'\n", QDir(config.cacheFolder).absolutePath().toStdString().c_str());
  if(config.scraper == "import") {
    printf("Import folder:      '\033[1;32m%s\033[0m'\n", config.importFolder.toStdString().c_str());
  }

  printf("\n");
  
  if(config.hints) {
    showHint();
  }

  doPrescrapeJobs();
  
  doneThreads = 0;
  notFound = 0;
  found = 0;
  avgCompleteness = 0;
  avgSearchMatch = 0;
  
  {
    if(config.unpack) {
      QProcess decProc;
      decProc.setReadChannel(QProcess::StandardOutput);
      decProc.start("which 7z");
      decProc.waitForFinished(10000);
      if(!decProc.readAllStandardOutput().contains("7z")) {
	printf("Couldn't find '7z' command. 7z is required by the '--flags unpack' flag. On Debian derivatives such as RetroPie you can install it with 'sudo apt-get install p7zip-full'.\n\nNow quitting...\n");
	exit(1);
      }
    }
  }
  
  if(!config.cacheFolder.isEmpty()) {
    cache = QSharedPointer<Cache>(new Cache(config.cacheFolder));
    if(cache->createFolders(config.scraper)) {
      if(!cache->read() && config.scraper == "cache") {
	printf("No resources for this platform found in the resource cache. Please specify a scraping module with '-s' to gather some resources before trying to generate a game list. Check all available modules with '--help'. You can also run Skyscraper in simple mode by typing 'Skyscraper' and follow the instructions on screen.\n\n");
	exit(1);
      }
    } else {
      printf("Couldn't create cache folders, please check folder permissions and try again...\n");
      exit(1);
    }
  }
  if(config.verbosity || config.cacheOptions == "show") {
    cache->showStats(config.cacheOptions == "show"?2:config.verbosity);
    if(config.cacheOptions == "show")
      exit(0);
  }
  if(config.cacheOptions.contains("purge:") ||
     config.cacheOptions.contains("vacuum")) {
    bool success = true;
    if(config.cacheOptions == "purge:all") {
      success = cache->purgeAll(config.unattend || config.unattendSkip);
    } else if(config.cacheOptions == "vacuum") {
      success = cache->vacuumResources(config.inputFolder, Platform::getFormats(config.platform, config.extensions, config.addExtensions), config.verbosity, config.unattend || config.unattendSkip);
    } else if(config.cacheOptions.contains("purge:m=") ||
	      config.cacheOptions.contains("purge:t=")) {
      success = cache->purgeResources(config.cacheOptions);
    }
    if(success) {
      state = 1; // Ignore ctrl+c
      cache->write();
      state = 0;
    }
    exit(0);
  }
  if(config.cacheOptions.contains("report:")) {
    cache->assembleReport(config.inputFolder, Platform::getFormats(config.platform,
								   config.extensions,
								   config.addExtensions),
			  config.platform, config.subdirs, config.cacheOptions);
    exit(0);
  }
  if(config.cacheOptions == "validate") {
    cache->validate();
    state = 1; // Ignore ctrl+c
    cache->write();
    state = 0;
    exit(0);
  }
  if(config.cacheOptions.contains("merge:")) {
    QFileInfo mergeCacheInfo(config.cacheOptions.replace("merge:", ""));
    if(mergeCacheInfo.exists()) {
      Cache mergeCache(mergeCacheInfo.absoluteFilePath());
      mergeCache.read();
      cache->merge(mergeCache, config.refresh, mergeCacheInfo.absoluteFilePath());
      state = 1; // Ignore ctrl+c
      cache->write();
      state = 0;
    } else {
      printf("Folder to merge from doesn't seem to exist, can't continue...\n");
    }
    exit(0);
  }
  cache->readPriorities();

  QDir inputDir(config.inputFolder, Platform::getFormats(config.platform, config.extensions, config.addExtensions), QDir::Name, QDir::Files);
  if(!inputDir.exists()) {
    printf("Input folder '\033[1;32m%s\033[0m' doesn't exist or can't be seen by current user. Please check path and permissions.\n", inputDir.absolutePath().toStdString().c_str());
    exit(1);
  }
  config.inputFolder = inputDir.absolutePath();

  QDir gameListDir(config.gameListFolder);
  if(config.scraper == "cache" && !config.pretend)
    checkForFolder(gameListDir);
  config.gameListFolder = gameListDir.absolutePath();
  
  QDir coversDir(config.coversFolder);
  if(config.scraper == "cache" && !config.pretend)
    checkForFolder(coversDir);
  config.coversFolder = coversDir.absolutePath();

  QDir screenshotsDir(config.screenshotsFolder);
  if(config.scraper == "cache" && !config.pretend)
    checkForFolder(screenshotsDir);
  config.screenshotsFolder = screenshotsDir.absolutePath();

  QDir wheelsDir(config.wheelsFolder);
  if(config.scraper == "cache" && !config.pretend)
    checkForFolder(wheelsDir);
  config.wheelsFolder = wheelsDir.absolutePath();

  QDir marqueesDir(config.marqueesFolder);
  if(config.scraper == "cache" && !config.pretend)
    checkForFolder(marqueesDir);
  config.marqueesFolder = marqueesDir.absolutePath();

  if(config.videos) {
    QDir videosDir(config.videosFolder);
    if(config.scraper == "cache" && !config.pretend)
      checkForFolder(videosDir);
    config.videosFolder = videosDir.absolutePath();
  }

  QDir importDir(config.importFolder);
  checkForFolder(importDir, false);
  config.importFolder = importDir.absolutePath();
  
  gameListFileString = gameListDir.absolutePath() + "/" + frontend->getGameListFileName();

  QFile gameListFile(gameListFileString);

  // Create shared queue with files to process
  queue = QSharedPointer<Queue>(new Queue());
  QList<QFileInfo> infoList = inputDir.entryInfoList();
  if(!config.startAt.isEmpty() && !infoList.isEmpty()) {
    QFileInfo startAt(config.startAt);
    if(!startAt.exists()) {
      startAt.setFile(config.currentDir + "/" + config.startAt);
    }
    if(!startAt.exists()) {
      startAt.setFile(inputDir.absolutePath() + "/" + config.startAt);
    }
    if(startAt.exists()) {
      while(infoList.first().fileName() != startAt.fileName() && !infoList.isEmpty()) {
	infoList.removeFirst();
      }
    }
  }
  if(!config.endAt.isEmpty() && !infoList.isEmpty()) {
    QFileInfo endAt(config.endAt);
    if(!endAt.exists()) {
      endAt.setFile(config.currentDir + "/" + config.endAt);
    }
    if(!endAt.exists()) {
      endAt.setFile(inputDir.absolutePath() + "/" + config.endAt);
    }
    if(endAt.exists()) {
      while(infoList.last().fileName() != endAt.fileName() && !infoList.isEmpty()) {
	infoList.removeLast();
      }
    }
  }
  queue->append(infoList);
  if(config.subdirs) {
    QDirIterator dirIt(config.inputFolder,
		       QDir::Dirs | QDir::NoDotAndDotDot,
		       QDirIterator::Subdirectories);
    while(dirIt.hasNext()) {
      QString subdir = dirIt.next();
      inputDir.setPath(subdir);
      queue->append(inputDir.entryInfoList());
      if(config.verbosity > 0) {
	printf("Adding files from subdir: '%s'\n", subdir.toStdString().c_str());
      }
    }
    if(config.verbosity > 0)
      printf("\n");
  }
  if(!config.excludeFiles.isEmpty()) {
    queue->filterFiles(config.excludeFiles);
  }
  if(!config.includeFiles.isEmpty()) {
    queue->filterFiles(config.includeFiles, true);
  }

  if(!cliFiles.isEmpty()) {
    queue->clear();
    for(const auto &cliFile: cliFiles) {
      queue->append(QFileInfo(cliFile));
    }
  } 

  state = 2; // Clear queue on ctrl+c
  if(config.cacheOptions.left(4) == "edit") {
    QString editCommand = "";
    QString editType = "";
    if(config.cacheOptions.contains(":") && config.cacheOptions.contains("=")) {
      config.cacheOptions.remove(0, config.cacheOptions.indexOf(":") + 1);
      if(config.cacheOptions.split("=").size() == 2) {
	editCommand = config.cacheOptions.split("=").at(0);
	editType = config.cacheOptions.split("=").at(1);
      }
    }
    cache->editResources(queue, editCommand, editType);
    printf("Done editing resources!\n");
    state = 1; // Ignore ctrl+c
    cache->write();
    state = 0;
    exit(0);
  }
  state = 0;

  if(!config.pretend && config.scraper == "cache" && config.gameListBackup) {
    QString gameListBackup = gameListFile.fileName() + "-" +
      QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
    printf("Game list backup saved to '\033[1;33m%s\033[0m'\n", gameListBackup.toStdString().c_str());
    gameListFile.copy(gameListBackup);
  }

  if(!config.pretend && config.scraper == "cache" &&
     !config.unattend && !config.unattendSkip &&
     gameListFile.exists()) {
    std::string userInput = "";
    printf("\033[1;34m'\033[0m\033[1;33m%s\033[0m\033[1;34m' already exists, do you want to overwrite it\033[0m (y/N)? ", frontend->getGameListFileName().toStdString().c_str());
    getline(std::cin, userInput);
    if(userInput == "y" || userInput == "Y") {
    } else {
      printf("User chose not to overwrite, now exiting...\n");
      exit(0);
    }
    printf("Checking if '\033[1;33m%s\033[0m' is writable?... ", frontend->getGameListFileName().toStdString().c_str());

    if(gameListFile.open(QIODevice::Append)) {
      printf("\033[1;32mIt is! :)\033[0m\n");
      gameListFile.close();
    } else {
      printf("\033[1;31mIt isn't! :(\nPlease check path and permissions and try again.\033[0m\n");
      exit(1);
    }
    printf("\n");
  }
  if(config.pretend && config.scraper == "cache") {
    printf("Pretend set! Not changing any files, just showing output.\n\n");
  }

  QFile skippedFile(skippedFileString);
  skippedFile.open(QIODevice::WriteOnly);
  skippedFile.write("--- The following is a list of skipped games ---\n");
  skippedFile.close();

  if(gameListFile.exists()) {
    printf("Trying to parse and load existing game list metadata... ");
    fflush(stdout);
    if(frontend->loadOldGameList(gameListFileString)) {
      printf("\033[1;32mSuccess!\033[0m\n");
      if(!config.unattend && cliFiles.isEmpty()) {
	std::string userInput = "";
	if(gameListFile.exists() && frontend->canSkip()) {
	  if(config.unattendSkip) {
	    userInput = "y";
	  } else {
	    printf("\033[1;34mDo you want to skip already existing game list entries\033[0m (y/N)? ");
	    getline(std::cin, userInput);
	  }
	  if((userInput == "y" || userInput == "Y") && frontend->canSkip()) {
	    frontend->skipExisting(gameEntries, queue);
	  }
	}
      }
    } else {
      printf("\033[1;33mNot found or unsupported!\033[0m\n");
    }
  }

  totalFiles = queue->length();
  
  if(config.romLimit != -1 && totalFiles > config.romLimit) {
    printf("\n\033[1;33mRestriction overrun!\033[0m This scraping module only allows for scraping up to %d roms at a time. You can either supply a few rom filenames on command line, or make use of the '--startat' and / or '--endat' command line options to adhere to this. Please check '--help' for more info.\n\nNow quitting...\n", config.romLimit);
    exit(0);
  }
  printf("\n");
  if(totalFiles > 0) {
    printf("Starting scraping run on \033[1;32m%d\033[0m files using \033[1;32m%d\033[0m threads.\nSit back, relax and let me do the work! :)\n\n", totalFiles, config.threads);
  } else {
    printf("\nNo entries to scrape...\n\n");
  }

  timer.start();
  currentFile = 1;

  QList<QThread*> threadList;
  for(int curThread = 1; curThread <= config.threads; ++curThread) {
    QThread *thread = new QThread;
    ScraperWorker *worker = new ScraperWorker(queue, cache, config, QString::number(curThread));
    worker->moveToThread(thread);
    connect(thread, &QThread::started, worker, &ScraperWorker::run);
    connect(worker, &ScraperWorker::entryReady, this, &Skyscraper::entryReady);
    connect(worker, &ScraperWorker::allDone, this, &Skyscraper::checkThreads);
    connect(thread, &QThread::finished, worker, &ScraperWorker::deleteLater);
    threadList.append(thread);
    // Do not start more threads if we have less files than allowed threads
    if(curThread == totalFiles) {
      config.threads = curThread;
      break;
    }
  }
  // Ready, set, GO! Start all threads
  for(const auto thread: threadList) {
    thread->start();
    state = 3;
  }
}

void Skyscraper::checkForFolder(QDir &folder, bool create)
{
  if(!folder.exists()) {
    printf("Folder '%s' doesn't exist", folder.absolutePath().toStdString().c_str());
    if(create) {
      printf(", trying to create it... ");
      fflush(stdout);
      if(folder.mkpath(folder.absolutePath())) {
	printf("\033[1;32mSuccess!\033[0m\n");
      } else {
	printf("\033[1;32mFailed!\033[0m Please check path and permissions, now exiting...\n");
	exit(1);
      }
    } else {
      printf(", can't continue...\n");
      exit(1);
    }
  }
}

QString Skyscraper::secsToString(const int &secs)
{
  QString hours = QString::number(secs / 3600000 % 24);
  QString minutes = QString::number(secs / 60000 % 60);
  QString seconds = QString::number(secs / 1000 % 60);
  if(hours.length() == 1) {
    hours.prepend("0");
  }
  if(minutes.length() == 1) {
    minutes.prepend("0");
  }
  if(seconds.length() == 1) {
    seconds.prepend("0");
  }

  return hours + ":" + minutes + ":" + seconds;
}

void Skyscraper::entryReady(GameEntry entry, QString output, QString debug)
{
  QMutexLocker locker(&entryMutex);

  printf("\033[0;32m#%d/%d\033[0m %s\n", currentFile, totalFiles, output.toStdString().c_str());

  if(config.verbosity >= 3) {
    printf("\033[1;33mDebug output:\033[0m\n%s\n", debug.toStdString().c_str());
  }

  if(entry.found) {
    found++;
    avgCompleteness += entry.completeness(config.videos);
    avgSearchMatch += entry.searchMatch;
    // Remove unnecessary media data to save memory before adding it to final entrylist
    // At this point data has been saved to disc, so we don't need it anymore.
    entry.resetMedia();
    gameEntries.append(entry);
  } else {
    notFound++;
    QFile skippedFile(skippedFileString);
    skippedFile.open(QIODevice::Append);
    skippedFile.write("'" + entry.baseName.toUtf8() + "'");
    if(entry.searchMatch == 0) {
      skippedFile.write(", No returned matches\n");
    } else {
      skippedFile.write(", Closest match was '" + entry.title.toUtf8() + "' at " + QByteArray::number(entry.searchMatch) + "%\n");
    }
    skippedFile.close();
    if(config.skipped) {
      // Remove unnecessary media data to save memory before adding it to final entrylist
      // At this point data has been saved to disc, so we don't need it anymore.
      entry.resetMedia();
      gameEntries.append(entry);
    }
  }
  
  printf("\033[1;34m#%d/%d\033[0m, (\033[1;32m%d\033[0m/\033[1;33m%d\033[0m)\n", currentFile, totalFiles, found, notFound);
  int elapsed = timer.elapsed();
  int estTime = (elapsed / currentFile * totalFiles) - elapsed;
  if(estTime < 0)
    estTime = 0;
  printf("Elapsed time   : \033[1;33m%s\033[0m\n", secsToString(elapsed).toStdString().c_str());
  printf("Est. time left : \033[1;33m%s\033[0m\n\n", secsToString(estTime).toStdString().c_str());

  if(!config.onlyMissing &&
     currentFile == config.maxFails &&
     notFound == config.maxFails &&
     config.scraper != "import" && config.scraper != "cache") {
    printf("\033[1;31mThis is NOT going well! I guit! *slams the door*\nNo, seriously, out of %d files we had %d misses. So either the scraping source is down or you are using a scraping source that doesn't support this platform. Please try another scraping module (check '--help').\n\nNow exiting...\033[0m\n", config.maxFails, config.maxFails);
    exit(1);
  }
  currentFile++;

#if QT_VERSION >= 0x050400
  qint64 spaceLimit = 209715200;
  if(config.spaceCheck) {
    if(config.scraper == "cache" && !config.pretend &&
       QStorageInfo(QDir(config.screenshotsFolder)).bytesFree() < spaceLimit) {
      printf("\033[1;31mYou have very little disk space left on the Skyscraper media export drive, please free up some space and try again. Now aborting...\033[0m\n\n");
      printf("Note! You can disable this check by setting 'spaceCheck=\"false\"' in the '[main]' section of config.ini.\n\n");
      // By clearing the queue here we basically tell Skyscraper to stop and quit nicely
      config.pretend = true;
      queue->clearAll();
    } else if(QStorageInfo(QDir(config.cacheFolder)).bytesFree() < spaceLimit) {
      printf("\033[1;31mYou have very little disk space left on the Skyscraper resource cache drive, please free up some space and try again. Now aborting...\033[0m\n\n");
      printf("Note! You can disable this check by setting 'spaceCheck=\"false\"' in the '[main]' section of config.ini.\n\n");
      // By clearing the queue here we basically tell Skyscraper to stop and quit nicely
      config.pretend = true;
      queue->clearAll();
    }
  }
#endif
}

void Skyscraper::checkThreads()
{
  QMutexLocker locker(&checkThreadMutex);

  doneThreads++;
  if(doneThreads != config.threads)
    return;

  if(!config.pretend && config.scraper == "cache") {
    printf("\033[1;34m---- Game list generation run completed! YAY! ----\033[0m\n");
    if(!config.cacheFolder.isEmpty()) {
      state = 1; // Ignore ctrl+c
      cache->write(true);
      state = 0;
    }
    QString finalOutput;
    frontend->sortEntries(gameEntries);
    printf("Assembling game list...");
    frontend->assembleList(finalOutput, gameEntries);
    printf(" \033[1;32mDone!\033[0m\n");
    QFile gameListFile(gameListFileString);
    printf("Now writing '\033[1;33m%s\033[0m'... ", gameListFileString.toStdString().c_str());
    fflush(stdout);
    if(gameListFile.open(QIODevice::WriteOnly)) {
      state = 1; // Ignore ctrl+c
      gameListFile.write(finalOutput.toUtf8());
      state = 0;
      gameListFile.close();
      printf("\033[1;32mSuccess!\033[0m\n\n");
    } else {
      printf("\033[1;31mCouldn't open file for writing!\nAll that work for nothing... :(\033[0m\n");
    }
  } else {
    printf("\033[1;34m---- Resource gathering run completed! YAY! ----\033[0m\n");
    if(!config.cacheFolder.isEmpty()) {
      state = 1; // Ignore ctrl+c
      cache->write();
      state = 0;
    }
  }
  
  printf("\033[1;34m---- And here are some neat stats :) ----\033[0m\n");
  printf("Total completion time: \033[1;33m%s\033[0m\n\n", secsToString(timer.elapsed()).toStdString().c_str());
  if(found > 0) {
    printf("Average search match: \033[1;33m%d%%\033[0m\n",
	   (int)((double)avgSearchMatch / (double)found));
    printf("Average entry completeness: \033[1;33m%d%%\033[0m\n\n",
	   (int)((double)avgCompleteness / (double)found));
  }
  printf("\033[1;34mTotal number of games: %d\033[0m\n", totalFiles);
  printf("\033[1;32mSuccessfully processed games: %d\033[0m\n", found);
  printf("\033[1;33mSkipped games: %d\033[0m (Filenames saved to '\033[1;33m/home/USER/.skyscraper/%s\033[0m')\n\n", notFound, skippedFileString.toStdString().c_str());

  // All done, now clean up and exit to terminal
  emit finished();
}

void Skyscraper::loadConfig(const QCommandLineParser &parser)
{
  QString current;
  QString distro;

  /* -----
     Files that should ALWAYS be updated from distributed default files
     ----- */

  current = "config.ini.example";
  distro = "/usr/local/etc/skyscraper/config.ini.example";
  copyFile(distro, current);

  current = "README.md";
  distro = "/usr/local/etc/skyscraper/README.md";
  copyFile(distro, current);

  current = "hints.xml";
  distro = "/usr/local/etc/skyscraper/hints.xml";
  copyFile(distro, current);

  current = "ARTWORK.md";
  distro = "/usr/local/etc/skyscraper/ARTWORK.md";
  copyFile(distro, current);

  current = "artwork.xml.example1";
  distro = "/usr/local/etc/skyscraper/artwork.xml.example1";
  copyFile(distro, current);

  current = "artwork.xml.example2";
  distro = "/usr/local/etc/skyscraper/artwork.xml.example2";
  copyFile(distro, current);

  current = "artwork.xml.example3";
  distro = "/usr/local/etc/skyscraper/artwork.xml.example3";
  copyFile(distro, current);

  current = "artwork.xml.example4";
  distro = "/usr/local/etc/skyscraper/artwork.xml.example4";
  copyFile(distro, current);

  current = "mameMap.csv";
  distro = "/usr/local/etc/skyscraper/mameMap.csv";
  copyFile(distro, current);
  
  current = "tgdb_developers.json";
  distro = "/usr/local/etc/skyscraper/tgdb_developers.json";
  copyFile(distro, current);

  current = "tgdb_publishers.json";
  distro = "/usr/local/etc/skyscraper/tgdb_publishers.json";
  copyFile(distro, current);

  current = "resources/boxfront.png";
  distro = "/usr/local/etc/skyscraper/resources/boxfront.png";
  copyFile(distro, current);

  current = "resources/boxside.png";
  distro = "/usr/local/etc/skyscraper/resources/boxside.png";
  copyFile(distro, current);

  current = "cache/README.md";
  distro = "/usr/local/etc/skyscraper/docs/CACHE.md";
  copyFile(distro, current);

  current = "cache/priorities.xml.example";
  distro = "/usr/local/etc/skyscraper/cache/priorities.xml.example";
  copyFile(distro, current);

  current = "import/README.md";
  distro = "/usr/local/etc/skyscraper/import/IMPORT.md";
  copyFile(distro, current);

  current = "import/definitions.dat.example1";
  distro = "/usr/local/etc/skyscraper/import/definitions.dat.example1";
  copyFile(distro, current);

  current = "import/definitions.dat.example2";
  distro = "/usr/local/etc/skyscraper/import/definitions.dat.example2";
  copyFile(distro, current);

  /* -----
     Files that will only be overwritten if they don't already exist
     ----- */

  current = "artwork.xml";
  distro = "/usr/local/etc/skyscraper/artwork.xml";
  copyFile(distro, current, false); // False means it won't overwrite if it exists

  current = "aliasMap.csv";
  distro = "/usr/local/etc/skyscraper/aliasMap.csv";
  copyFile(distro, current, false);

  current = "resources/maskexample.png";
  distro = "/usr/local/etc/skyscraper/resources/maskexample.png";
  copyFile(distro, current, false);

  current = "resources/frameexample.png";
  distro = "/usr/local/etc/skyscraper/resources/frameexample.png";
  copyFile(distro, current, false);

  current = "resources/scanlines1.png";
  distro = "/usr/local/etc/skyscraper/resources/scanlines1.png";
  copyFile(distro, current, false);

  current = "resources/scanlines2.png";
  distro = "/usr/local/etc/skyscraper/resources/scanlines2.png";
  copyFile(distro, current, false);

  // Copy one of the example definitions.dat files if none exists
  current = "import/definitions.dat";
  distro = "/usr/local/etc/skyscraper/import/definitions.dat.example2";
  copyFile(distro, current, false);

  /* -----
     END updating files from distribution files
     ----- */

  //migrate(parser.isSet("c")?parser.value("c"):"config.ini");

  QSettings settings(parser.isSet("c")?parser.value("c"):"config.ini", QSettings::IniFormat);

  // Start by setting frontend, since we need it to set default for game list and so on
  settings.beginGroup("main");
  if(settings.contains("frontend")) {
    config.frontend = settings.value("frontend").toString();
  }
  settings.endGroup();
  if(parser.isSet("f") && (parser.value("f") == "emulationstation" ||
			   parser.value("f") == "attractmode" ||
			   parser.value("f") == "pegasus")) {
    config.frontend = parser.value("f");
  }
  if(config.frontend == "emulationstation") {
    frontend = new EmulationStation;
  } else if(config.frontend == "attractmode") {
    frontend = new AttractMode;
  } else if(config.frontend == "pegasus") {
    frontend = new Pegasus;
  }

  frontend->setConfig(&config);
  
  bool inputFolderSet = false;
  bool gameListFolderSet = false;
  bool mediaFolderSet = false;
  
  // Main config, overrides defaults
  settings.beginGroup("main");
  if(settings.contains("platform")) {
    config.platform = settings.value("platform").toString();
  }
  if(settings.contains("lang")) {
    config.lang = settings.value("lang").toString();
  }
  if(settings.contains("region")) {
    config.region = settings.value("region").toString();
  }
  if(settings.contains("langPrios")) {
    config.langPriosStr = settings.value("langPrios").toString();
  }
  if(settings.contains("regionPrios")) {
    config.regionPriosStr = settings.value("regionPrios").toString();
  }
  if(settings.contains("pretend")) {
    config.pretend = settings.value("pretend").toBool();
  }
  if(settings.contains("unpack")) {
    config.unpack = settings.value("unpack").toBool();
  }
  if(settings.contains("interactive")) {
    config.interactive = settings.value("interactive").toBool();
  }
  if(settings.contains("unattend")) {
    config.unattend = settings.value("unattend").toBool();
  }
  if(settings.contains("unattendSkip")) {
    config.unattendSkip = settings.value("unattendSkip").toBool();
  }
  if(settings.contains("forceFilename")) {
    config.forceFilename = settings.value("forceFilename").toBool();
  }
  if(settings.contains("verbosity")) {
    config.verbosity = settings.value("verbosity").toInt();
  }
  if(settings.contains("hints")) {
    config.hints = settings.value("hints").toBool();
  }
  if(settings.contains("subdirs")) {
    config.subdirs = settings.value("subdirs").toBool();
  }
  if(settings.contains("maxLength")) {
    config.maxLength = settings.value("maxLength").toInt();
  }
  if(settings.contains("threads")) {
    config.threads = settings.value("threads").toInt();
    config.threadsSet = true;
  }
  if(settings.contains("emulator")) {
    config.frontendExtra = settings.value("emulator").toString();
  }
  if(settings.contains("launch")) {
    config.frontendExtra = settings.value("launch").toString();
  }
  if(settings.contains("videos")) {
    config.videos = settings.value("videos").toBool();
  }
  if(settings.contains("videoSizeLimit")) {
    config.videoSizeLimit = settings.value("videoSizeLimit").toInt() * 1024 * 1024;
  }
  if(settings.contains("videoConvertCommand")) {
    config.videoConvertCommand = settings.value("videoConvertCommand").toString();
  }
  if(settings.contains("videoConvertExtension")) {
    config.videoConvertExtension = settings.value("videoConvertExtension").toString();
  }
  if(settings.contains("symlink")) {
    config.symlink = settings.value("symlink").toBool();
  }
  if(settings.contains("skipped")) {
    config.skipped = settings.value("skipped").toBool();
  }
  if(settings.contains("maxFails") &&
     settings.value("maxFails").toInt() >= 1 &&
     settings.value("maxFails").toInt() <= 200) {
    config.maxFails = settings.value("maxFails").toInt();
  }
  if(settings.contains("brackets")) {
    config.brackets = settings.value("brackets").toBool();
  }
  if(settings.contains("relativePaths")) {
    config.relativePaths = settings.value("relativePaths").toBool();
  }
  if(settings.contains("addExtensions")) {
    config.addExtensions = settings.value("addExtensions").toString();
  }
  if(settings.contains("minMatch")) {
    config.minMatch = settings.value("minMatch").toInt();
    config.minMatchSet = true;
  }
  if(settings.contains("artworkXml")) {
    config.artworkConfig = settings.value("artworkXml").toString();
  }
  if(settings.contains("excludeFiles")) {
    config.excludeFiles = settings.value("excludeFiles").toString();
  }
  if(settings.contains("includeFiles")) {
    config.includeFiles = settings.value("includeFiles").toString();
  }
  if(settings.contains("jpgQuality")) {
    config.jpgQuality = settings.value("jpgQuality").toInt();
  }
  if(settings.contains("cacheResize")) {
    config.cacheResize = settings.value("cacheResize").toBool();
  }
  if(settings.contains("cacheCovers")) {
    config.cacheCovers = settings.value("cacheCovers").toBool();
  }
  if(settings.contains("cacheScreenshots")) {
    config.cacheScreenshots = settings.value("cacheScreenshots").toBool();
  }
  if(settings.contains("cacheWheels")) {
    config.cacheWheels = settings.value("cacheWheels").toBool();
  }
  if(settings.contains("cacheMarquees")) {
    config.cacheMarquees = settings.value("cacheMarquees").toBool();
  }
  if(settings.contains("scummIni")) {
    config.scummIni = settings.value("scummIni").toString();
  }
  // Check for command line platform here, since we need it for 'platform' config.ini entries
  if(parser.isSet("p") && Platform::getPlatforms().contains(parser.value("p"))) {
    config.platform = parser.value("p");
  } else {
    if((!parser.isSet("flags") && parser.value("flags") != "help") &&
       (!parser.isSet("cache") && parser.value("cache") != "help")) {
      printf("Please set a valid platform with '-p [platform]'\nCheck '--help' for a list of supported platforms.\n");
      exit(1);
    }
  }
  if(settings.contains("cacheFolder")) {
    QString cacheFolder = settings.value("cacheFolder").toString();
    config.cacheFolder = cacheFolder + (cacheFolder.right(1) == "/"?"":"/") + config.platform;
  }
  if(settings.contains("inputFolder")) {
    QString inputFolder = settings.value("inputFolder").toString();
    config.inputFolder = inputFolder + (inputFolder.right(1) == "/"?"":"/") + config.platform;
    inputFolderSet = true;
  }
  // gamelistFolder kept in addition to gameListFolder for backwards compatibility
  if(settings.contains("gamelistFolder")) {
    QString gameListFolder = settings.value("gamelistFolder").toString();
    config.gameListFolder = gameListFolder + (gameListFolder.right(1) == "/"?"":"/") + config.platform;
    gameListFolderSet = true;
  }
  if(settings.contains("gameListFolder")) {
    QString gameListFolder = settings.value("gameListFolder").toString();
    config.gameListFolder = gameListFolder + (gameListFolder.right(1) == "/"?"":"/") + config.platform;
    gameListFolderSet = true;
  }
  if(settings.contains("gameListBackup")) {
    config.gameListBackup = settings.value("gameListBackup").toBool();
  }
  if(settings.contains("mediaFolder")) {
    QString mediaFolder = settings.value("mediaFolder").toString();
    config.mediaFolder = mediaFolder + (mediaFolder.right(1) == "/"?"":"/") + config.platform;
    mediaFolderSet = true;
  }
  if(settings.contains("importFolder")) {
    config.importFolder = settings.value("importFolder").toString();
  }
  if(settings.contains("spaceCheck")) {
    config.spaceCheck = settings.value("spaceCheck").toBool();
  }
  if(settings.contains("nameTemplate")) {
    config.nameTemplate = settings.value("nameTemplate").toString();
  }
  settings.endGroup();

  // Platform specific configs, overrides main and defaults
  settings.beginGroup(config.platform);
  if(settings.contains("emulator")) {
    config.frontendExtra = settings.value("emulator").toString();
  }
  if(settings.contains("launch")) {
    config.frontendExtra = settings.value("launch").toString();
  }
  if(settings.contains("inputFolder")) {
    config.inputFolder = settings.value("inputFolder").toString();
    inputFolderSet = true;
  }
  // gamelistFolder kept in addition to gameListFolder for backwards compatibility
  if(settings.contains("gamelistFolder")) {
    config.gameListFolder = settings.value("gamelistFolder").toString();
    gameListFolderSet = true;
  }
  if(settings.contains("gameListFolder")) {
    config.gameListFolder = settings.value("gameListFolder").toString();
    gameListFolderSet = true;
  }
  if(settings.contains("mediaFolder")) {
    config.mediaFolder = settings.value("mediaFolder").toString();
    mediaFolderSet = true;
  }
  if(settings.contains("cacheFolder")) {
    config.cacheFolder = settings.value("cacheFolder").toString();
  }
  if(settings.contains("jpgQuality")) {
    config.jpgQuality = settings.value("jpgQuality").toInt();
  }
  if(settings.contains("cacheResize")) {
    config.cacheResize = settings.value("cacheResize").toBool();
  }
  if(settings.contains("cacheCovers")) {
    config.cacheCovers = settings.value("cacheCovers").toBool();
  }
  if(settings.contains("cacheScreenshots")) {
    config.cacheScreenshots = settings.value("cacheScreenshots").toBool();
  }
  if(settings.contains("cacheWheels")) {
    config.cacheWheels = settings.value("cacheWheels").toBool();
  }
  if(settings.contains("cacheMarquees")) {
    config.cacheMarquees = settings.value("cacheMarquees").toBool();
  }
  if(settings.contains("importFolder")) {
    config.importFolder = settings.value("importFolder").toString();
  }
  if(settings.contains("skipped")) {
    config.skipped = settings.value("skipped").toBool();
  }
  if(settings.contains("brackets")) {
    config.brackets = settings.value("brackets").toBool();
  }
  if(settings.contains("subdirs")) {
    config.subdirs = settings.value("subdirs").toBool();
  }
  if(settings.contains("relativePaths")) {
    config.relativePaths = settings.value("relativePaths").toBool();
  }
  if(settings.contains("extensions")) {
    config.extensions = settings.value("extensions").toString();
  }
  if(settings.contains("addExtensions")) {
    config.addExtensions = settings.value("addExtensions").toString();
  }
  if(settings.contains("minMatch")) {
    config.minMatch = settings.value("minMatch").toInt();
    config.minMatchSet = true;
  }
  if(settings.contains("lang")) {
    config.lang = settings.value("lang").toString();
  }
  if(settings.contains("region")) {
    config.region = settings.value("region").toString();
  }
  if(settings.contains("langPrios")) {
    config.langPriosStr = settings.value("langPrios").toString();
  }
  if(settings.contains("regionPrios")) {
    config.regionPriosStr = settings.value("regionPrios").toString();
  }
  if(settings.contains("threads")) {
    config.threads = settings.value("threads").toInt();
    config.threadsSet = true;
  }
  if(settings.contains("videos")) {
    config.videos = settings.value("videos").toBool();
  }
  if(settings.contains("videoSizeLimit")) {
    config.videoSizeLimit = settings.value("videoSizeLimit").toInt() * 1000 * 1000;
  }
  if(settings.contains("symlink")) {
    config.symlink = settings.value("symlink").toBool();
  }
  if(settings.contains("startAt")) {
    config.startAt = settings.value("startAt").toString();
  }
  if(settings.contains("endAt")) {
    config.endAt = settings.value("endAt").toString();
  }
  if(settings.contains("pretend")) {
    config.pretend = settings.value("pretend").toBool();
  }
  if(settings.contains("unpack")) {
    config.unpack = settings.value("unpack").toBool();
  }
  if(settings.contains("unattend")) {
    config.unattend = settings.value("unattend").toBool();
  }
  if(settings.contains("unattendSkip")) {
    config.unattendSkip = settings.value("unattendSkip").toBool();
  }
  if(settings.contains("interactive")) {
    config.interactive = settings.value("interactive").toBool();
  }
  if(settings.contains("forceFilename")) {
    config.forceFilename = settings.value("forceFilename").toBool();
  }
  if(settings.contains("verbosity")) {
    config.verbosity = settings.value("verbosity").toInt();
  }
  if(settings.contains("maxLength")) {
    config.maxLength = settings.value("maxLength").toInt();
  }
  if(settings.contains("artworkXml")) {
    config.artworkConfig = settings.value("artworkXml").toString();
  }
  if(settings.contains("excludeFiles")) {
    config.excludeFiles = settings.value("excludeFiles").toString();
  }
  if(settings.contains("includeFiles")) {
    config.includeFiles = settings.value("includeFiles").toString();
  }
  if(settings.contains("nameTemplate")) {
    config.nameTemplate = settings.value("nameTemplate").toString();
  }
  settings.endGroup();

  // Check for command line scraping module here
  if(parser.isSet("s") && (parser.value("s") == "openretro" ||
			   parser.value("s") == "thegamesdb" ||
			   parser.value("s") == "arcadedb" ||
			   parser.value("s") == "worldofspectrum" ||
			   parser.value("s") == "igdb" ||
			   parser.value("s") == "mobygames" ||
			   parser.value("s") == "screenscraper" ||
			   parser.value("s") == "esgamelist" ||
			   parser.value("s") == "cache" ||
			   parser.value("s") == "import")) {
    config.scraper = parser.value("s");
  }

  // Frontend specific configs, overrides main, platform, module and defaults
  settings.beginGroup(config.frontend);
  if(settings.contains("artworkXml")) {
    config.artworkConfig = settings.value("artworkXml").toString();
  }
  if(settings.contains("excludeFiles")) {
    config.excludeFiles = settings.value("excludeFiles").toString();
  }
  if(settings.contains("includeFiles")) {
    config.includeFiles = settings.value("includeFiles").toString();
  }
  if(settings.contains("emulator")) {
    config.frontendExtra = settings.value("emulator").toString();
  }
  if(settings.contains("launch")) {
    config.frontendExtra = settings.value("launch").toString();
  }
  // gamelistFolder kept in addition to gameListFolder for backwards compatibility
  if(settings.contains("gamelistFolder")) {
    config.gameListFolder = settings.value("gamelistFolder").toString();
    gameListFolderSet = true;
  }
  if(settings.contains("gameListFolder")) {
    config.gameListFolder = settings.value("gameListFolder").toString();
    gameListFolderSet = true;
  }
  if(settings.contains("gameListBackup")) {
    config.gameListBackup = settings.value("gameListBackup").toBool();
  }
  if(settings.contains("mediaFolder")) {
    config.mediaFolder = settings.value("mediaFolder").toString();
    mediaFolderSet = true;
  }
  if(settings.contains("skipped")) {
    config.skipped = settings.value("skipped").toBool();
  }
  if(settings.contains("brackets")) {
    config.brackets = settings.value("brackets").toBool();
  }
  if(settings.contains("videos")) {
    config.videos = settings.value("videos").toBool();
  }
  if(settings.contains("symlink")) {
    config.symlink = settings.value("symlink").toBool();
  }
  if(settings.contains("startAt")) {
    config.startAt = settings.value("startAt").toString();
  }
  if(settings.contains("endAt")) {
    config.endAt = settings.value("endAt").toString();
  }
  if(settings.contains("unattend")) {
    config.unattend = settings.value("unattend").toBool();
  }
  if(settings.contains("unattendSkip")) {
    config.unattendSkip = settings.value("unattendSkip").toBool();
  }
  if(settings.contains("forceFilename")) {
    config.forceFilename = settings.value("forceFilename").toBool();
  }
  if(settings.contains("verbosity")) {
    config.verbosity = settings.value("verbosity").toInt();
  }
  if(settings.contains("maxLength")) {
    config.maxLength = settings.value("maxLength").toInt();
  }
  settings.endGroup();

  // Scraping module specific configs, overrides main, platform and defaults
  settings.beginGroup(config.scraper);
  if(settings.contains("userCreds")) {
    config.userCreds = settings.value("userCreds").toString();
  }
  if(settings.contains("threads")) {
    config.threads = settings.value("threads").toInt();
    config.threadsSet = true;
  }
  if(settings.contains("minMatch")) {
    config.minMatch = settings.value("minMatch").toInt();
    config.minMatchSet = true;
  }
  if(settings.contains("maxLength")) {
    config.maxLength = settings.value("maxLength").toInt();
  }
  if(settings.contains("interactive")) {
    config.interactive = settings.value("interactive").toBool();
  }
  if(settings.contains("jpgQuality")) {
    config.jpgQuality = settings.value("jpgQuality").toInt();
  }
  if(settings.contains("cacheResize")) {
    config.cacheResize = settings.value("cacheResize").toBool();
  }
  if(settings.contains("cacheCovers")) {
    config.cacheCovers = settings.value("cacheCovers").toBool();
  }
  if(settings.contains("cacheScreenshots")) {
    config.cacheScreenshots = settings.value("cacheScreenshots").toBool();
  }
  if(settings.contains("cacheWheels")) {
    config.cacheWheels = settings.value("cacheWheels").toBool();
  }
  if(settings.contains("cacheMarquees")) {
    config.cacheMarquees = settings.value("cacheMarquees").toBool();
  }
  if(settings.contains("videos")) {
    config.videos = settings.value("videos").toBool();
  }
  if(settings.contains("videoSizeLimit")) {
    config.videoSizeLimit = settings.value("videoSizeLimit").toInt() * 1000 * 1000;
  }
  if(settings.contains("videoConvertCommand")) {
    config.videoConvertCommand = settings.value("videoConvertCommand").toString();
  }
  if(settings.contains("videoConvertExtension")) {
    config.videoConvertExtension = settings.value("videoConvertExtension").toString();
  }
  if(settings.contains("videoPreferNormalized")) {
    config.videoPreferNormalized = settings.value("videoPreferNormalized").toBool();
  }
  settings.endGroup();

  // Command line configs, overrides main, platform, module and defaults
  if(parser.isSet("l") && parser.value("l").toInt() >= 0 && parser.value("l").toInt() <= 10000) {
    config.maxLength = parser.value("l").toInt();
  }
  if(parser.isSet("t") && parser.value("t").toInt() <= 8) {
    config.threads = parser.value("t").toInt();
    config.threadsSet = true;
  }
  if(parser.isSet("e")) {
    config.frontendExtra = parser.value("e");
  }
  if(parser.isSet("i")) {
    config.inputFolder = parser.value("i");
    inputFolderSet = true;
  }
  if(parser.isSet("g")) {
    config.gameListFolder = parser.value("g");
    gameListFolderSet = true;
  }
  if(parser.isSet("o")) {
    config.mediaFolder = parser.value("o");
    mediaFolderSet = true;
  }
  if(parser.isSet("a")) {
    config.artworkConfig = parser.value("a");
  }
  if(parser.isSet("m") && parser.value("m").toInt() >= 0 && parser.value("m").toInt() <= 100) {
    config.minMatch = parser.value("m").toInt();
    config.minMatchSet = true;
  }
  if(parser.isSet("u")) {
    config.userCreds = parser.value("u");
  }
  if(parser.isSet("d")) {
    config.cacheFolder = parser.value("d");
  } else {
    if(config.cacheFolder.isEmpty())
      config.cacheFolder = "cache/" + config.platform;
  }
  if(parser.isSet("flags")) {
    if(parser.value("flags") == "help") {
      printf("Showing '\033[1;33m--flags\033[0m' help\n");
      printf("Use comma-separated flags (eg. '--flags FLAG1,FLAG2') to enable multiple flags.\nThe following is a list of valid flags and what they do:\n");

      printf("  \033[1;33mforcefilename\033[0m: Use filename as game name instead of the returned game title when generating a game list. Consider using 'nameTemplate' config.ini option instead.\n");
      printf("  \033[1;33minteractive\033[0m: Always ask user to choose best returned result from the scraping modules.\n");
      printf("  \033[1;33mnobrackets\033[0m: Disables any [] and () tags in the frontend game titles. Consider using 'nameTemplate' config.ini option instead.\n");
      printf("  \033[1;33mnocovers\033[0m: Disable covers/boxart from being cached locally. Only do this if you do not plan to use the cover artwork in 'artwork.xml'\n");
      printf("  \033[1;33mnohints\033[0m: Disables the 'DID YOU KNOW:' hints when running Skyscraper.\n");
      printf("  \033[1;33mnomarquees\033[0m: Disable marquees from being cached locally. Only do this if you do not plan to use the marquee artwork in 'artwork.xml'\n");
      printf("  \033[1;33mnoresize\033[0m: Disable resizing of artwork when saving it to the resource cache. Normally they are resized to save space.\n");
      printf("  \033[1;33mnoscreenshots\033[0m: Disable screenshots/snaps from being cached locally. Only do this if you do not plan to use the screenshot artwork in 'artwork.xml'\n");
      printf("  \033[1;33mnosubdirs\033[0m: Do not include input folder subdirectories when scraping.\n");
      printf("  \033[1;33mnowheels\033[0m: Disable wheels from being cached locally. Only do this if you do not plan to use the wheel artwork in 'artwork.xml'\n");
      printf("  \033[1;33monlymissing\033[0m: Tells Skyscraper to skip all files which already have any data from any source in the cache.\n");
      printf("  \033[1;33mpretend\033[0m: Only relevant when generating a game list. It disables the game list generator and artwork compositor and only outputs the results of the potential game list generation to the terminal. Use it to check what and how the data will be combined from cached resources.\n");
      printf("  \033[1;33mrelative\033[0m: Forces all gamelist paths to be relative to rom location.\n");
      printf("  \033[1;33mskipexistingcovers\033[0m: When generating gamelists, skip processing covers that already exist in the media output folder.\n");
      printf("  \033[1;33mskipexistingmarquees\033[0m: When generating gamelists, skip processing marquees that already exist in the media output folder.\n");
      printf("  \033[1;33mskipexistingscreenshots\033[0m: When generating gamelists, skip processing screenshots that already exist in the media output folder.\n");
      printf("  \033[1;33mskipexistingvideos\033[0m: When generating gamelists, skip copying videos that already exist in the media output folder.\n");
      printf("  \033[1;33mskipexistingwheels\033[0m: When generating gamelists, skip processing wheels that already exist in the media output folder.\n");
      printf("  \033[1;33mskipped\033[0m: When generating a gamelist, also include games that do not have any cached data.\n");
      printf("  \033[1;33msymlink\033[0m: Forces cached videos to be symlinked to game list destination to save space. WARNING! Deleting or moving files from your cache can invalidate the links!\n");
      printf("  \033[1;33munattend\033[0m: Skip initial questions when scraping. It will then always overwrite existing gamelist and not skip existing entries.\n");
      printf("  \033[1;33munattendskip\033[0m: Skip initial questions when scraping. It will then always overwrite existing gamelist and always skip existing entries.\n");
      printf("  \033[1;33munpack\033[0m: Unpacks and checksums the file inside 7z or zip files instead of the compressed file itself. Be aware that this option requires '7z' to be installed on the system to work. Only relevant for 'screenscraper' scraping module.\n");
      printf("  \033[1;33mvideos\033[0m: Enables scraping and caching of videos for the scraping modules that support them. Beware, this takes up a lot of disk space!.\n");
      printf("\n");
      exit(0);
    } else {
      QList<QString> flags = parser.value("flags").split(",");
      for(const auto &flag: flags) {
	if(flag == "forcefilename") {
	  config.forceFilename = true;
	} else if(flag == "interactive") {
	  config.interactive = true;
	} else if(flag == "nobrackets") {
	  config.brackets = false;
	} else if(flag == "nocovers") {
	  config.cacheCovers = false;
	} else if(flag == "nohints") {
	  config.hints = false;
	} else if(flag == "nomarquees") {
	  config.cacheMarquees = false;
	} else if(flag == "noresize") {
	  config.cacheResize = false;
	} else if(flag == "noscreenshots") {
	  config.cacheScreenshots = false;
	} else if(flag == "nosubdirs") {
	  config.subdirs = false;
	} else if(flag == "nowheels") {
	  config.cacheWheels = false;
	} else if(flag == "onlymissing") {
	  config.onlyMissing = true;
	} else if(flag == "pretend") {
	  config.pretend = true;
	} else if(flag == "relative") {
	  config.relativePaths = true;
	} else if(flag == "skipexistingcovers") {
	  config.skipExistingCovers = true;
	} else if(flag == "skipexistingmarquees") {
	  config.skipExistingMarquees = true;
	} else if(flag == "skipexistingscreenshots") {
	  config.skipExistingScreenshots = true;
	} else if(flag == "skipexistingvideos") {
	  config.skipExistingVideos = true;
	} else if(flag == "skipexistingwheels") {
	  config.skipExistingWheels = true;
	} else if(flag == "skipped") {
	  config.skipped = true;
	} else if(flag == "symlink") {
	  config.symlink = true;
	} else if(flag == "unattend") {
	  config.unattend = true;
	} else if(flag == "unattendskip") {
	  config.unattendSkip = true;
	} else if(flag == "unpack") {
	  config.unpack = true;
	} else if(flag == "videos") {
	  config.videos = true;
	} else {
	  printf("Unknown flag '%s', please check '--flags help' for a list of valid flags. Exiting...\n", flag.toStdString().c_str());
	  exit(1);
	}
      }
    }
  }
  if(parser.isSet("videos")) {
    config.videos = true;
  }
  if(parser.isSet("symlink")) {
    config.symlink = true;
  }
  if(parser.isSet("skipexistingvideos")) {
    config.skipExistingVideos = true;
  }
  if(parser.isSet("nocovers")) {
    config.cacheCovers = false;
  }
  if(parser.isSet("noscreenshots")) {
    config.cacheScreenshots = false;
  }
  if(parser.isSet("nowheels")) {
    config.cacheWheels = false;
  }
  if(parser.isSet("nomarquees")) {
    config.cacheMarquees = false;
  }
  if(parser.isSet("skipexistingcovers")) {
    config.skipExistingCovers = true;
  }
  if(parser.isSet("skipexistingscreenshots")) {
    config.skipExistingScreenshots = true;
  }
  if(parser.isSet("skipexistingwheels")) {
    config.skipExistingWheels = true;
  }
  if(parser.isSet("skipexistingmarquees")) {
    config.skipExistingMarquees = true;
  }
  if(parser.isSet("skipped")) {
    config.skipped = true;
  }
  if(parser.isSet("nobrackets")) {
    config.brackets = false;
  }
  if(parser.isSet("relative")) {
    config.relativePaths = true;
  }
  if(parser.isSet("addext")) {
    config.addExtensions = parser.value("addext");
  }
  if(parser.isSet("refresh")) {
    config.refresh = true;
  }
  if(parser.isSet("cache")) {
    config.cacheOptions = parser.value("cache");
    if(config.cacheOptions == "refresh") {
      config.refresh = true;
    } else if(config.cacheOptions == "help") {
      printf("Showing '\033[1;33m--cache\033[0m' help\n");
      printf("  \033[1;33m--cache show\033[0m: Prints a status of all cached resources for the selected platform.\n");
      printf("  \033[1;33m--cache validate\033[0m: Checks the consistency of the cache for the selected platform.\n");
      printf("  \033[1;33m--cache edit\033[0m: Let's you edit resources for the selected platform for all files or a range of files. Add a filename on command line to edit cached resources for just that one file, use '--fromfile' to edit files created with the '--cache report' option or use '--startat' and '--endat' to edit a range of roms.\n");
      printf("  \033[1;33m--cache edit:new=<TYPE>\033[0m: Let's you batch add resources of <TYPE> to the selected platform for all files or a range of files. Add a filename on command line to edit cached resources for just that one file, use '--fromfile' to edit files created with the '--cache report' option or use '--startat' and '--endat' to edit a range of roms.\n");
      printf("  \033[1;33m--cache vacuum\033[0m: Compares your romset to any cached resource and removes the resources that you no longer have roms for.\n");
      printf("  \033[1;33m--cache report:missing=<OPTION>\033[0m: Generates reports with all files that are missing the specified resources. Check '--cache report:missing=help' for more info.\n");
      printf("  \033[1;33m--cache merge:<PATH>\033[0m: Merges two resource caches together. It will merge the resource cache specified by <PATH> into the local resource cache by default. To merge into a non-default destination cache folder set it with '-d <PATH>'. Both should point to folders with the 'db.xml' inside.\n");
      printf("  \033[1;33m--cache purge:all\033[0m: Removes ALL cached resources for the selected platform.\n");
      printf("  \033[1;33m--cache purge:m=<MODULE>,t=<TYPE>\033[0m: Removes cached resources related to the selected module(m) and / or type(t). Either one can be left out in which case ALL resources from the selected module or ALL resources from the selected type will be removed.\n");
      printf("  \033[1;33m--cache refresh\033[0m: Forces a refresh of existing cached resources for any scraping module. Requires a scraping module set with '-s'. Similar to '--refresh'.\n");
      printf("\n");
      exit(0);
    }
  }
  if(parser.isSet("noresize")) {
    config.cacheResize = false;
  }
  if(parser.isSet("nosubdirs")) {
    config.subdirs = false;
  }
  if(parser.isSet("unpack")) {
    config.unpack = true;
  }
  if(parser.isSet("onlymissing")) {
    config.onlyMissing = true;
  }
  if(parser.isSet("startat")) {
    config.startAt = parser.value("startat");
  }
  if(parser.isSet("endat")) {
    config.endAt = parser.value("endat");
  }
  if(parser.isSet("excludefiles")) {
    config.excludeFiles = parser.value("excludefiles");
  }
  if(parser.isSet("includefiles")) {
    config.includeFiles = parser.value("includefiles");
  }
  if(parser.isSet("maxfails") &&
     parser.value("maxfails").toInt() >= 1 &&
     parser.value("maxfails").toInt() <= 200) {
    config.maxFails = parser.value("maxfails").toInt();
  }
  if(parser.isSet("pretend")) {
    config.pretend = true;
  }
  if(parser.isSet("interactive")) {
    config.interactive = true;
  }
  if(parser.isSet("unattend")) {
    config.unattend = true;
  }
  if(parser.isSet("unattendskip")) {
    config.unattendSkip = true;
  }
  if(parser.isSet("forcefilename")) {
    config.forceFilename = true;
  }
  if(parser.isSet("region")) {
    config.region = parser.value("region");
  }
  if(parser.isSet("lang")) {
    config.lang = parser.value("lang");
  }
  if(parser.isSet("nohints")) {
    config.hints = false;
  }
  if(parser.isSet("verbosity")) {
    config.verbosity = parser.value("verbosity").toInt();
  }

  frontend->checkReqs();

  // Change config defaults if they aren't already set, find the rest in settings.h
  if(!inputFolderSet) {
    config.inputFolder = frontend->getInputFolder();
  }
  if(!gameListFolderSet) {
    config.gameListFolder = frontend->getGameListFolder();
  }
  if(!mediaFolderSet) {
    config.mediaFolder = config.gameListFolder + "/media";
  }
  config.coversFolder = frontend->getCoversFolder();
  config.screenshotsFolder = frontend->getScreenshotsFolder();
  config.wheelsFolder = frontend->getWheelsFolder();
  config.marqueesFolder = frontend->getMarqueesFolder();
  config.videosFolder = frontend->getVideosFolder();
  
  // Choose default scraper for chosen platform if none has been set yet
  if(config.scraper.isEmpty()) {
    config.scraper = Platform::getDefaultScraper(config.platform);
  }

  // If platform subfolder exists for import path, use it
  QDir importFolder(config.importFolder);
  if(importFolder.exists(config.platform)) {
    config.importFolder.append((config.importFolder.right(1) == "/"?"":"/") + config.platform);
  }

  // Set minMatch to 0 for cache, arcadedb and screenscraper
  // We know these results are always accurate
  if(config.minMatchSet == false && (config.scraper == "cache" ||
				     config.scraper == "screenscraper" ||
				     config.scraper == "arcadedb" ||
				     config.scraper == "esgamelist" ||
				     config.scraper == "import")) {
    config.minMatch = 0;
  }

  skippedFileString = "skipped-" + config.platform + "-" + config.scraper + ".txt";

  // Grab all requested files from cli, if any
  QList<QString> requestedFiles = parser.positionalArguments();
  
  // Add files from '--fromfile', if any
  if(parser.isSet("fromfile") && QFileInfo::exists(parser.value("fromfile"))) {
    QFile fromFile(parser.value("fromfile"));
    if(fromFile.open(QIODevice::ReadOnly)) {
      while(!fromFile.atEnd()) {
	requestedFiles.append(QString(fromFile.readLine().simplified()));
      }
      fromFile.close();
    }
  }

  // Verify requested files and add the ones that exist
  for(const auto &requestedFile: requestedFiles) {
    QFileInfo requestedFileInfo(requestedFile);
    if(!requestedFileInfo.exists()) {
      requestedFileInfo.setFile(config.currentDir + "/" + requestedFile);
    }
    if(!requestedFileInfo.exists()) {
      requestedFileInfo.setFile(config.inputFolder + "/" + requestedFile);
    }
    if(requestedFileInfo.exists()) {
      cliFiles.append(requestedFileInfo.absoluteFilePath());
      // Always set refresh and unattend true if user has supplied filenames on
      // command line. That way they are cached, but game list is not changed and user isn't
      // asked about skipping and overwriting.
      config.refresh = true;
      config.unattend = true;
    } else {
      printf("Filename: '%s' requested either on command line or with '--fromfile' not found!\n\nPlease verify the filename and try again...\n", requestedFile.toStdString().c_str());
      exit(1);
    }
  }

  // Add query only if a single filename was passed on command line
  if(parser.isSet("query")) {
    if(cliFiles.length() == 1) {
      config.searchName = parser.value("query");
    } else {
      printf("'--query' was set but more than one filename was provided on command line or using the '--fromfile' option. Please provide only a single rom file name you wish to scrape using the query, now quitting...\n");
      exit(1);
    }
  }
  
  if(config.startAt != "" || config.endAt != "") {
    config.refresh = true;
    config.unattend = true;
    config.subdirs = false;
  }
  
  // If interactive is set, force 1 thread and always accept the chosen result
  if(config.interactive) {
    if(config.scraper == "cache" ||
       config.scraper == "import" ||
       config.scraper == "arcadedb" ||
       config.scraper == "esgamelist" ||
       config.scraper == "screenscraper") {
      config.interactive = false;
    } else {
      config.threads = 1;
      config.minMatch = 0;
      config.refresh = true;
    }
  }

  if(config.scraper == "import") {
    // Always force the cache to be refreshed when using import scraper
    config.refresh = true;
    config.videos = true;
    // minMatch set to 0 further up
  }

  if(!config.userCreds.isEmpty() && config.userCreds.contains(":")) {
    QList<QString> userCreds = config.userCreds.split(":");
    if(userCreds.length() == 2) {
      config.user = userCreds.at(0);
      config.password = userCreds.at(1);
    }
  }

  QFile artworkFile(config.artworkConfig);
  if(artworkFile.open(QIODevice::ReadOnly)) {
    config.artworkXml = artworkFile.readAll();
    artworkFile.close();
  } else {
    printf("Couldn't read artwork xml file '\033[1;32m%s\033[0m'. Please check file and permissions. Now exiting...\n", config.artworkConfig.toStdString().c_str());
    exit(1);
  }
  
  QDir resDir("./resources");
  QDirIterator resDirIt(resDir.absolutePath(),
			QDir::Files | QDir::NoDotAndDotDot,
			QDirIterator::Subdirectories);
  while(resDirIt.hasNext()) {
    QString resFile = resDirIt.next();
    resFile = resFile.remove(0, resFile.indexOf("resources/") + 10); // Also cut off 'resources/'
    config.resources[resFile] = QImage("resources/" + resFile);
  }
}

void Skyscraper::copyFile(QString &distro, QString &current, bool overwrite)
{
  if(QFileInfo::exists(distro)) {
    if(QFileInfo::exists(current)) {
      if(overwrite) {
	QFile::remove(current);
	QFile::copy(distro, current);
      }
    } else {
      QFile::copy(distro, current);
    }
  }
}

void Skyscraper::showHint()
{
  QFile hintsFile("hints.xml");
  QDomDocument hintsXml;
  if(!hintsFile.open(QIODevice::ReadOnly)) {
    return;
  }
  if(!hintsXml.setContent(&hintsFile)) {
    return;
  }
  hintsFile.close();
  QDomNodeList hintNodes = hintsXml.elementsByTagName("hint");
  printf("\033[1;33mDID YOU KNOW:\033[0m %s\n\n", hintsXml.elementsByTagName("hint").at(qrand() % hintNodes.length()).toElement().text().toStdString().c_str());
}

void Skyscraper::doPrescrapeJobs()
{
  loadAliasMap();
  loadMameMap();
  loadWhdLoadMap();

  setRegionPrios();
  setLangPrios();

  NetComm manager;
  QEventLoop q; // Event loop for use when waiting for data from NetComm.
  connect(&manager, &NetComm::dataReady, &q, &QEventLoop::quit);
  
  if(config.platform == "amiga" &&
     config.scraper != "cache" && config.scraper != "import" && config.scraper != "esgamelist") {
    printf("Fetching 'whdload_db.xml', just a sec...");
    manager.request("https://raw.githubusercontent.com/HoraceAndTheSpider/Amiberry-XML-Builder/master/whdload_db.xml");
    q.exec();
    QByteArray data = manager.getData();
    QDomDocument tempDoc;
    QFile whdLoadFile("whdload_db.xml");
    if(data.size() > 1000000 &&
       tempDoc.setContent(data) &&
       whdLoadFile.open(QIODevice::WriteOnly)) {
      whdLoadFile.write(data);
      whdLoadFile.close();
      printf("\033[1;32m Success!\033[0m\n\n");
    } else {
      printf("\033[1;31m Failed!\033[0m\n\n");
    }
  }

  if(config.scraper == "arcadedb" && config.threads != 1) {
    printf("\033[1;33mForcing 1 thread to accomodate limits in ArcadeDB scraping module\033[0m\n\n");
    config.threads = 1;
  } else if(config.scraper == "igdb") {
    printf("\033[1;33mForcing 1 thread when using the IGDB scraping module\033[0m\n\n");
    printf("\033[1;32mTHIS MODULE IS POWERED BY IGDB.COM\033[0m\n");
    config.threads = 1;
    if(config.userCreds.isEmpty()) {
      printf("\033[1;31mThe IGDB module requires a free personal API key to work. Get one at https://api.igdb.com and set it either with '-u <KEY>' or by adding the following to '/home/USER/.skyscraper/config.ini':\n[igdb]\nuserCreds=\"<KEY>\"\n\nSkyscraper can't continue, now exiting...\n");
      exit(1);
    }
    printf("Fetching key status, just a sec...\n");
    manager.request("https://api-v3.igdb.com/api_status", "", "user-key", config.userCreds);
    q.exec();
    QJsonObject jsonObj = QJsonDocument::fromJson(manager.getData()).array().first().toObject();
    if(jsonObj.isEmpty()) {
      printf("Received invalid IGDB server response, maybe their server is having issues, please try again later...\n");
      exit(1);
    }
    if(jsonObj["authorized"].toBool()) {
      QString plan = jsonObj["plan"].toString();
      jsonObj = jsonObj["usage_reports"].toObject()["usage_report"].toObject();
      QString limit = QString::number(jsonObj["max_value"].toInt());
      QString requests = QString::number(jsonObj["current_value"].toInt());
      QString resetDate = jsonObj["period_end"].toString();
      printf("Plan       : %s\n", plan.toStdString().c_str());
      printf("Requests   : %s / %s\n", requests.toStdString().c_str(), limit.toStdString().c_str());
      printf("Period ends: %s\n", resetDate.toStdString().c_str());
    } else {
      if(jsonObj["status"].toInt() == 403) {
	printf("IGDB monthly request limit has been reached, can't continue with this module...\n");
      } else {
	printf("IGDB says key is unauthorized, can't continue with this module...\n");
      }
      exit(1);
    }
    printf("\n");
  } else if(config.scraper == "mobygames" && config.threads != 1) {
    printf("\033[1;33mForcing 1 thread to accomodate limits in MobyGames scraping module. Also be aware that MobyGames has a request limit of 360 requests per hour for the entire Skyscraper user base. So if someone else is currently using it, it will quit.\033[0m\n\n");
    config.threads = 1;
    config.romLimit = 35;
  } else if(config.scraper == "screenscraper") {
    if(config.user.isEmpty() || config.password.isEmpty()) {
      if(config.threads > 1) {
	printf("\033[1;33mForcing 1 threads as this is the anonymous limit in the ScreenScraper scraping module. Sign up for an account at https://www.screenscraper.fr and support them to gain more threads. Then use the credentials with Skyscraper using the '-u user:password' command line option or by setting 'userCreds=\"user:password\"' in '/home/USER/.skyscraper/config.ini'.\033[0m\n\n");
	config.threads = 1;
      }
    } else {
      printf("Fetching limits for user '\033[1;33m%s\033[0m', just a sec...\n", config.user.toStdString().c_str());
      manager.request("https://www.screenscraper.fr/api2/ssuserInfos.php?devid=muldjord&devpassword=" + StrTools::unMagic("204;198;236;130;203;181;203;126;191;167;200;198;192;228;169;156") + "&softname=skyscraper" VERSION "&output=json&ssid=" + config.user + "&sspassword=" + config.password);
      q.exec();
      QJsonObject jsonObj = QJsonDocument::fromJson(manager.getData()).object();
      if(jsonObj.isEmpty()) {
	if(manager.getData().contains("Erreur de login")) {
	  printf("\033[0;31mScreenScraper login error! Please verify that you've entered your credentials correctly in '/home/USER/.skyscraper/config.ini'. It needs to look EXACTLY like this, but with your USER and PASS:\033[0m\n\033[1;33m[screenscraper]\nuserCreds=\"USER:PASS\"\033[0m\033[0;31m\nContinuing with unregistered user, forcing 1 thread...\033[0m\n\n");
	} else {
	  printf("\033[1;33mReceived invalid / empty ScreenScraper server response, maybe their server is busy / overloaded. Forcing 1 thread...\033[0m\n\n");
	}
	config.threads = 1;
      } else {
	int allowedThreads = jsonObj["response"].toObject()["ssuser"].toObject()["maxthreads"].toString().toInt();
	if(allowedThreads != 0) {
	  if(config.threadsSet && config.threads <= allowedThreads) {
	    printf("User is allowed %d threads, but user has set it manually, so ignoring.\n\n", allowedThreads);
	  } else {
	    config.threads = (allowedThreads <= 8?allowedThreads:8);
	    printf("Setting threads to \033[1;32m%d\033[0m as allowed for the supplied user credentials.\n\n", config.threads);
	  }
	}
      }
    }
  }
}

void Skyscraper::loadAliasMap()
{
  if(!QFileInfo::exists("aliasMap.csv"))
    return;
  QFile aliasMapFile("aliasMap.csv");
  if(aliasMapFile.open(QIODevice::ReadOnly)) {
    while(!aliasMapFile.atEnd()) {
      QByteArray line = aliasMapFile.readLine();
      if(line.left(1) == "#")
	continue;
      QList<QByteArray> pair = line.split(';');
      if(pair.size() != 2)
	continue;
      QString baseName = pair.at(0);
      QString aliasName = pair.at(1);
      baseName = baseName.replace("\"", "").simplified();
      aliasName = aliasName.replace("\"", "").simplified();
      config.aliasMap[baseName] = aliasName;
    }
    aliasMapFile.close();
  }
}

void Skyscraper::loadMameMap()
{
  if(config.scraper != "import" &&
     (config.platform == "neogeo" ||
      config.platform == "arcade" ||
      config.platform == "mame-advmame" ||
      config.platform == "mame-libretro" ||
      config.platform == "mame-mame4all" ||
      config.platform == "fba")) {
    QFile mameMapFile("mameMap.csv");
    if(mameMapFile.open(QIODevice::ReadOnly)) {
      while(!mameMapFile.atEnd()) {
	QList<QByteArray> pair = mameMapFile.readLine().split(';');
	if(pair.size() != 2)
	  continue;
	QString mameName = pair.at(0);
	QString realName = pair.at(1);
	mameName = mameName.replace("\"", "").simplified();
	realName = realName.replace("\"", "").simplified();
	config.mameMap[mameName] = realName;
      }
      mameMapFile.close();
    }
  }
}


void Skyscraper::loadWhdLoadMap()
{
  if(config.platform == "amiga") {
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
	  config.whdLoadMap[gameNode.toElement().attribute("filename")] = gamePair;
	}
      }
    }
  }
}

void Skyscraper::setRegionPrios()
{
  // Load single custom region
  if(!config.region.isEmpty()) {
    config.regionPrios.append(config.region);
  }
  
  // Load custom region prioritizations
  if(!config.regionPriosStr.isEmpty()) {
    for(const auto &region: config.regionPriosStr.split(",")) {
      config.regionPrios.append(region);
    }
  } else {
    config.regionPrios.append("eu");
    config.regionPrios.append("us");
    config.regionPrios.append("ss");
    config.regionPrios.append("uk");
    config.regionPrios.append("wor");
    config.regionPrios.append("jp");
    config.regionPrios.append("au");
    config.regionPrios.append("ame");
    config.regionPrios.append("de");
    config.regionPrios.append("cus");
    config.regionPrios.append("cn");
    config.regionPrios.append("kr");
    config.regionPrios.append("asi");
    config.regionPrios.append("br");
    config.regionPrios.append("sp");
    config.regionPrios.append("fr");
    config.regionPrios.append("gr");
    config.regionPrios.append("it");
    config.regionPrios.append("no");
    config.regionPrios.append("dk");
    config.regionPrios.append("nz");
    config.regionPrios.append("nl");
    config.regionPrios.append("pl");
    config.regionPrios.append("ru");
    config.regionPrios.append("se");
    config.regionPrios.append("tw");
  }
}

void Skyscraper::setLangPrios()
{
  // Load single custom lang
  if(!config.lang.isEmpty()) {
    config.langPrios.append(config.lang);
  }
  
  // Load custom lang prioritizations
  if(!config.langPriosStr.isEmpty()) {
    for(const auto &lang: config.langPriosStr.split(",")) {
      config.langPrios.append(lang);
    }
  } else {
    config.langPrios.append("en");
  }
}

/*
void Skyscraper::migrate(QString filename)
{
  if(!QFileInfo::exists(filename) || QFileInfo::exists(filename + ".old"))
    return;

  QByteArray data;
  QFile settings(filename);
  if(settings.open(QIODevice::ReadOnly)) {
    data = settings.readAll();
    settings.close();
  }
  QFile oldSettings(filename + ".old");
  if(oldSettings.open(QIODevice::WriteOnly)) {
    printf("Migrating old config.ini to new v3.0.0 format. Old config saved to '%s'\n\n", oldSettings.fileName().toStdString().c_str());
    oldSettings.write(data);
    oldSettings.close();
  }
  data.replace("dbFolder=", "cacheFolder="); 
  data.replace("allowExtension=", "addExtensions="); 
  data.replace("startat=", "startAt="); 
  data.replace("endat=", "endAt="); 
  data.replace("noHints=\"true\"", "hints=\"false\"");
  data.replace("noHints=\"false\"", "hints=\"true\"");
  data.replace("noHints=true", "hints=false");
  data.replace("noHints=false", "hints=true");
  data.replace("noBrackets=\"true\"", "brackets=\"false\"");
  data.replace("noBrackets=\"false\"", "brackets=\"true\"");
  data.replace("noBrackets=true", "brackets=false");
  data.replace("noBrackets=false", "brackets=true");
  data.replace("noResize=\"true\"", "resize=\"false\"");
  data.replace("noResize=\"false\"", "resize=\"true\"");
  data.replace("noResize=true", "resize=false");
  data.replace("noResize=false", "resize=true");
  data.replace("\n[cache]\n", ""); 
  data.replace("\n[localDb]\n", ""); 
  data.replace("covers=", "cacheCovers="); 
  data.replace("screenshots=", "cacheScreenshots="); 
  data.replace("wheels=", "cacheWheels="); 
  data.replace("marquees=", "cacheMarquees="); 
  if(settings.open(QIODevice::WriteOnly)) {
    settings.write(data);
    settings.close();
  }
}
*/

// --- Console colors ---
// Black        0;30     Dark Gray     1;30
// Red          0;31     Light Red     1;31
// Green        0;32     Light Green   1;32
// Brown/Orange 0;33     Yellow        1;33
// Blue         0;34     Light Blue    1;34
// Purple       0;35     Light Purple  1;35
// Cyan         0;36     Light Cyan    1;36
// Light Gray   0;37     White         1;37
