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
  if(!config.emulator.isEmpty()) {
    printf("Emulator:           '\033[1;32m%s\033[0m'\n", config.emulator.toStdString().c_str());
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
  printf("Cache folder:       '\033[1;32m%s\033[0m'\n", config.cacheFolder.toStdString().c_str());
  if(config.scraper == "import") {
    printf("Import folder:      '\033[1;32m%s\033[0m'\n", config.importFolder.toStdString().c_str());
  }

  printf("\n");
  
  if(!config.nohints) {
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
	printf("Couldn't find '7z' command. 7z is required by the '--unpack' option. On Debian derivatives such as RetroPie you can install it with 'sudo apt-get install p7zip-full'.\n\nNow quitting...\n");
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
  if(!config.cacheOptions.isEmpty() &&
     (config.cacheOptions.contains("purge:") ||
      config.cacheOptions.contains("vacuum"))) {
    if(config.cacheOptions == "purge:all") {
      cache->purgeAll(config.unattend || config.unattendSkip);
    } else if(config.cacheOptions == "vacuum") {
      cache->vacuumResources(config.inputFolder, Platform::getFormats(config.platform, config.extensions, config.addExtensions), config.unattend || config.unattendSkip);
    } else if(config.cacheOptions.contains("purge:m=") ||
	      config.cacheOptions.contains("purge:t=")) {
      cache->purgeResources(config.cacheOptions);
    }
    cache->write();
    exit(0);
  }
  if(config.cacheOptions == "validate") {
    cache->validate();
    cache->write();
    exit(0);
  }
  if(!config.cacheOptions.isEmpty() &&
     config.cacheOptions.contains("merge:")) {
    QFileInfo mergeCacheInfo(config.cacheOptions.replace("merge:", ""));
    if(mergeCacheInfo.exists()) {
      Cache mergeCache(mergeCacheInfo.absoluteFilePath());
      mergeCache.read();
      cache->merge(mergeCache, config.refresh, mergeCacheInfo.absoluteFilePath());
      cache->write();
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
  checkForFolder(gameListDir);
  config.gameListFolder = gameListDir.absolutePath();
  
  QDir coversDir(config.coversFolder);
  checkForFolder(coversDir);
  config.coversFolder = coversDir.absolutePath();

  QDir screenshotsDir(config.screenshotsFolder);
  checkForFolder(screenshotsDir);
  config.screenshotsFolder = screenshotsDir.absolutePath();

  QDir wheelsDir(config.wheelsFolder);
  checkForFolder(wheelsDir);
  config.wheelsFolder = wheelsDir.absolutePath();

  QDir marqueesDir(config.marqueesFolder);
  checkForFolder(marqueesDir);
  config.marqueesFolder = marqueesDir.absolutePath();

  if(config.videos) {
    QDir videosDir(config.videosFolder);
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
		       QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks,
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

  if(!cliFiles.isEmpty()) {
    queue->clear();
    foreach(QString cliFile, cliFiles) {
      queue->append(QFileInfo(cliFile));
    }
  } 

  if(config.cacheOptions == "edit") {
    cache->editResources(queue);
    printf("Done editing resources!\n");
    cache->write();
    exit(0);
  }

  if(!config.pretend && config.scraper == "cache" &&
     !config.unattend && !config.unattendSkip &&
     gameListFile.exists()) {
    std::string userInput = "";
    printf("\033[1;34m'\033[1;32m%s\033[0m\033[1;34m' already exists, do you want to overwrite it\033[0m (y/N)? ", frontend->getGameListFileName().toStdString().c_str());
    getline(std::cin, userInput);
    if(userInput == "y" || userInput == "Y") {
    } else {
      printf("User chose not to overwrite, now exiting...\n");
      exit(0);
    }

    printf("Checking if '\033[1;32m%s\033[0m' is writable?... ", frontend->getGameListFileName().toStdString().c_str());

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

  if(totalFiles > 0) {
    printf("\nStarting scraping run on \033[1;32m%d\033[0m files using \033[1;32m%d\033[0m threads.\nSit back, relax and let me do the work! :)\n\n", totalFiles, config.threads);
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
  // Ready, set, GO!!! Start all threads
  foreach(QThread *thread, threadList) {
    thread->start();
    threadsRunning = true;
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
  
  printf("\033[1;32m%d\033[0m/\033[1;33m%d\033[0m/\033[1;34m%d\033[0m\n", found, notFound, currentFile);
  int elapsed = timer.elapsed();
  int estTime = (elapsed / currentFile * totalFiles) - elapsed;
  if(estTime < 0)
    estTime = 0;
  printf("Elapsed time   : \033[1;33m%s\033[0m\n", secsToString(elapsed).toStdString().c_str());
  printf("Est. time left : \033[1;33m%s\033[0m\n\n", secsToString(estTime).toStdString().c_str());

  if(currentFile == config.maxFails && notFound == config.maxFails &&
     config.scraper != "import" && config.scraper != "cache") {
    printf("\033[1;31mThis is NOT going well! I guit! *slams the door*\nNo, seriously, out of %d files we had %d misses. So either the scraping source is down or you are using a scraping source that doesn't support this platform. Please try another scraping module (check '--help').\n\nNow exiting...\033[0m\n", config.maxFails, config.maxFails);
    exit(1);
  }
  currentFile++;

#if QT_VERSION >= 0x050400
  qint64 spaceLimit = 209715200;
  if(config.spaceCheck && (QStorageInfo(QDir(config.mediaFolder)).bytesFree() < spaceLimit ||
			   QStorageInfo(QDir::current()).bytesFree() < spaceLimit)) {
    printf("\033[1;31mYou have very little disk space left either on the Skyscraper resource cache drive or on the game list and media export drive, please free up some space and try again. Now aborting...\033[0m\n\nNote! You can disable this check by setting 'spaceCheck=\"false\"' in the '[main]' section of config.ini.\n\n");
    // Clean up and exit
    if(config.scraper == "cache") {
      config.pretend = true;
    }
    // By clearing the queue here we basically tell Skyscraper to stop and quit nicely
    queue->clearAll();
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
    QString finalOutput;
    frontend->sortEntries(gameEntries);
    printf("Assembling game list...");
    frontend->assembleList(finalOutput, gameEntries);
    printf(" \033[1;32mDone!!!\033[0m\n");
    QFile gameListFile(gameListFileString);
    printf("Now writing '%s'... ", gameListFileString.toStdString().c_str());
    fflush(stdout);
    if(gameListFile.open(QIODevice::WriteOnly)) {
      gameListFile.write(finalOutput.toUtf8());
      gameListFile.close();
      printf("\033[1;32mSuccess!!!\033[0m\n\n");
    } else {
      printf("\033[1;31mCouldn't open file for writing!!!\nAll that work for nothing... :(\033[0m\n");
    }
  } else {
    printf("\033[1;34m---- Resource gathering run completed! YAY! ----\033[0m\n");
    if(!config.cacheFolder.isEmpty()) {
      cache->write();
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
  printf("\033[1;33mSkipped games: %d (Filenames saved to '~/.skyscraper/%s')\033[0m\n\n", notFound, skippedFileString.toStdString().c_str());

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

  current = "hints.txt";
  distro = "/usr/local/etc/skyscraper/hints.txt";
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

  migrate(parser.isSet("c")?parser.value("c"):"config.ini");

  QSettings settings(parser.isSet("c")?parser.value("c"):"config.ini", QSettings::IniFormat);

  // Start by setting frontend, since we need it to set default for gamelist and so on
  settings.beginGroup("main");
  if(settings.contains("frontend")) {
    config.frontend = settings.value("frontend").toString();
  }
  settings.endGroup();
  if(parser.isSet("f") && (parser.value("f") == "emulationstation" ||
			   parser.value("f") == "attractmode")) {
    config.frontend = parser.value("f");
  }
  if(config.frontend == "emulationstation") {
    frontend = new EmulationStation;
  } else if(config.frontend == "attractmode") {
    frontend = new AttractMode;
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
    config.nohints = !settings.value("hints").toBool();
  }
  if(settings.contains("subdirs")) {
    config.subdirs = settings.value("subdirs").toBool();
  }
  if(settings.contains("maxLength")) {
    config.maxLength = settings.value("maxLength").toInt();
  }
  if(settings.contains("threads")) {
    config.threads = settings.value("threads").toInt();
  }
  if(settings.contains("emulator")) {
    config.emulator = settings.value("emulator").toString();
  }
  if(settings.contains("videos")) {
    config.videos = settings.value("videos").toBool();
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
  if(settings.contains("cacheResize")) {
    config.noResize = !settings.value("cacheResize").toBool();
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
  // Check for command line platform here, since we need it for 'platform' config.ini entries
  if(parser.isSet("p") && Platform::getPlatforms().contains(parser.value("p"))) {
    config.platform = parser.value("p");
  } else {
    printf("Please set a valid platform with '-p [platform]'\nCheck '--help' for a list of supported platforms, now exiting...\n");
    exit(1);
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
  if(settings.contains("gamelistFolder")) {
    QString gamelistFolder = settings.value("gamelistFolder").toString();
    config.gameListFolder = gamelistFolder + (gamelistFolder.right(1) == "/"?"":"/") + config.platform;
    gameListFolderSet = true;
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
  settings.endGroup();

  // Platform specific configs, overrides main and defaults
  settings.beginGroup(config.platform);
  if(settings.contains("emulator")) {
    config.emulator = settings.value("emulator").toString();
  }
  if(settings.contains("inputFolder")) {
    config.inputFolder = settings.value("inputFolder").toString();
    inputFolderSet = true;
  }
  if(settings.contains("gamelistFolder")) {
    config.gameListFolder = settings.value("gamelistFolder").toString();
    gameListFolderSet = true;
  }
  if(settings.contains("mediaFolder")) {
    config.mediaFolder = settings.value("mediaFolder").toString();
    mediaFolderSet = true;
  }
  if(settings.contains("cacheFolder")) {
    config.cacheFolder = settings.value("cacheFolder").toString();
  }
  if(settings.contains("cacheResize")) {
    config.noResize = !settings.value("cacheResize").toBool();
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

  // Scraping module specific configs, overrides main, platform and defaults
  settings.beginGroup(config.scraper);
  if(settings.contains("userCreds")) {
    config.userCreds = settings.value("userCreds").toString();
  }
  if(settings.contains("threads")) {
    config.threads = settings.value("threads").toInt();
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
  settings.endGroup();

  // Command line configs, overrides main, platform, module and defaults
  if(parser.isSet("l") && parser.value("l").toInt() >= 0 && parser.value("l").toInt() <= 10000) {
    config.maxLength = parser.value("l").toInt();
  }
  if(parser.isSet("t") && parser.value("t").toInt() <= 8) {
    config.threads = parser.value("t").toInt();
  }
  if(parser.isSet("e")) {
    config.emulator = parser.value("e");
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
  if(parser.isSet("videos")) {
    config.videos = true;
  }
  if(parser.isSet("symlink")) {
    config.symlink = true;
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
    if(config.cacheOptions == "refresh")
      config.refresh = true;
  }
  if(parser.isSet("noresize")) {
    config.noResize = true;
  }
  if(parser.isSet("nosubdirs")) {
    config.subdirs = false;
  }
  if(parser.isSet("unpack")) {
    config.unpack = true;
  }
  if(parser.isSet("startat")) {
    config.startAt = parser.value("startat");
  }
  if(parser.isSet("endat")) {
    config.endAt = parser.value("endat");
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
    config.nohints = true;
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

  skippedFileString = "skipped-" + config.scraper + ".txt";

  // If user has set specific files to scrape on command line set them internally
  foreach(QString cliArgument, parser.positionalArguments()) {
    QFileInfo cliFile(cliArgument);
    if(!cliFile.exists()) {
      cliFile.setFile(config.currentDir + "/" + cliArgument);
    }
    if(!cliFile.exists()) {
      cliFile.setFile(config.inputFolder + "/" + cliArgument);
    }
    if(cliFile.exists()) {
      cliFiles.append(cliFile.absoluteFilePath());
      // Always set refresh and unattend true if user has supplied filenames on
      // command line. That way they are cached, but game list is not changed and user isn't
      // asked about skipping and overwriting.
      config.refresh = true;
      config.unattend = true;
    } else {
      printf("Filename: '%s' not found!\n\nNow quitting...\n", cliArgument.toStdString().c_str());
      exit(1);
    }
  }

  // Add query only if a single filename was passed on command line
  if(parser.isSet("query")) {
    if(cliFiles.length() == 1) {
      config.searchName = parser.value("query");
    } else {
      printf("'--query' was set but no filename was provided on command line. Please provide a single rom file name you wish to scrape using the query, now quitting...\n");
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

  if(!config.userCreds.isEmpty() && config.userCreds.indexOf(":") != -1) {
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
			QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks,
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
  QFile hintsFile("hints.txt");
  QList<QString> hints;
  if(hintsFile.open(QIODevice::ReadOnly)) {
    while(!hintsFile.atEnd()) {
      hints.append(QString(hintsFile.readLine()));
    }
    hintsFile.close();
    if(!hints.isEmpty()) {
      printf("\033[1;33mDID YOU KNOW:\033[0m %s\n", hints.at(qrand() % hints.size()).toStdString().c_str());
    }
  }
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
  } else if(config.scraper == "openretro" && config.threads != 1) {
    printf("\033[1;33mForcing 1 thread to accomodate limits in OpenRetro scraping module\033[0m\n\n");
    config.threads = 1;
  } else if(config.scraper == "igdb") {
    bool exitNow = false;
    printf("\033[1;33mForcing 1 thread when using the IGDB scraping module\033[0m\n\n");
    printf("\033[1;32mTHIS MODULE IS POWERED BY IGDB.COM\033[0m\n");
    config.threads = 1;
    config.romLimit = 35;
    printf("Fetching key status, just a sec...\n");
    manager.request("https://api-v3.igdb.com/api_status", "", "user-key", StrTools::unMagic("136;213;169;133;171;147;206;117;211;152;214;221;209;213;157;197;136;158;212;220;171;211;160;215;202;172;216;125;172;174;151;171"));
    q.exec();
    QByteArray data = manager.getData();
    QJsonObject jsonObj = QJsonDocument::fromJson(data).array().first().toObject();
    if(jsonObj.isEmpty()) {
      printf("Recieved invalid IGDB server response, maybe their server is having issues, please try again later...\n");
      exitNow = true;
    }
    if(jsonObj.value("authorized").toBool()) {
      QString plan = jsonObj.value("plan").toString();
      jsonObj = jsonObj.value("usage_reports").toObject().value("usage_report").toObject();
      QString limit = QString::number(jsonObj.value("max_value").toInt());
      QString requests = QString::number(jsonObj.value("current_value").toInt());
      QString resetDate = jsonObj.value("period_end").toString();
      printf("Plan       : %s\n", plan.toStdString().c_str());
      printf("Requests   : %s / %s\n", requests.toStdString().c_str(), limit.toStdString().c_str());
      printf("Period ends: %s\n", resetDate.toStdString().c_str());
    } else {
      if(jsonObj.value("status").toInt() == 403) {
	printf("IGDB monthly request limit has been reached, can't continue with this module...\n");
      } else {
	printf("IGDB says key is unauthorized, can't continue with this module...\n");
      }
      exitNow = true;
    }
    if(config.verbosity >= 1)
      printf("Answer was:\n%s\n", data.data());
    if(exitNow)
      exit(1);
    printf("\n");
  } else if(config.scraper == "mobygames" && config.threads != 1) {
    printf("\033[1;33mForcing 1 thread to accomodate limits in MobyGames scraping module. Also be aware that MobyGames has a request limit of 360 requests per hour for the entire Skyscraper user base. So if someone else is currently using it, it will quit.\033[0m\n\n");
    config.threads = 1;
    config.romLimit = 35;
  } else if(config.scraper == "screenscraper") {
    if(config.user.isEmpty() || config.password.isEmpty()) {
      if(config.threads > 1) {
	printf("\033[1;33mForcing 1 threads as this is the anonymous limit in the ScreenScraper scraping module. Sign up for an account at https://www.screenscraper.fr and support them to gain more threads. Then use the credentials with Skyscraper using the '-u [user:password]' command line option or by setting 'userCreds=[user:password]' in '~/.skyscraper/config.ini'.\033[0m\n\n");
	config.threads = 1;
      }
    } else {
      printf("Fetching limits for user '%s', just a sec...\n", config.user.toStdString().c_str());
      manager.request("https://www.screenscraper.fr/api2/ssuserInfos.php?devid=muldjord&devpassword=" + StrTools::unMagic("204;198;236;130;203;181;203;126;191;167;200;198;192;228;169;156") + "&softname=skyscraper" VERSION "&output=xml&ssid=" + config.user + "&sspassword=" + config.password);
      q.exec();
      QByteArray data = manager.getData();
      QByteArray nodeBegin = "<maxthreads>";
      QByteArray nodeEnd = "</maxthreads>";
      int allowedThreads = QString(data.mid(data.indexOf(nodeBegin) + nodeBegin.length(), data.indexOf(nodeEnd) - (data.indexOf(nodeBegin) + nodeBegin.length()))).toInt();
      if(allowedThreads != 0) {
	config.threads = (allowedThreads <= 8?allowedThreads:8);
	printf("Setting threads to %d as allowed for the supplied user credentials.\n\n", config.threads);
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
    foreach(QString region, config.regionPriosStr.split(",")) {
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
    foreach(QString lang, config.langPriosStr.split(",")) {
      config.langPrios.append(lang);
    }
  } else {
    config.langPrios.append("en");
  }
}

void Skyscraper::migrate(QString filename)
{
  if(QFileInfo::exists(filename + ".old"))
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

// --- Console colors ---
// Black        0;30     Dark Gray     1;30
// Red          0;31     Light Red     1;31
// Green        0;32     Light Green   1;32
// Brown/Orange 0;33     Yellow        1;33
// Blue         0;34     Light Blue    1;34
// Purple       0;35     Light Purple  1;35
// Cyan         0;36     Light Cyan    1;36
// Light Gray   0;37     White         1;37
