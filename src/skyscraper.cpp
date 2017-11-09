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

#include "skyscraper.h"
#include "xmlreader.h"
#include "strtools.h"

#include "emulationstation.h"
#include "attractmode.h"

Skyscraper::Skyscraper(const QCommandLineParser &parser)
{
  qRegisterMetaType<GameEntry>("GameEntry");
  
  qDebug("\033[1;34m------------------------------------------\033[0m\n\033[1;33mRunning Skyscraper v" VERSION " by Lars Muldjord\033[0m\n\033[1;34m------------------------------------------\033[0m\n");
  loadConfig(parser);
}

Skyscraper::~Skyscraper()
{
  delete frontend;
}

void Skyscraper::run()
{
  qDebug("Platform        : '\033[1;32m%s\033[0m'\n", config.platform.toStdString().c_str());
  qDebug("Scraper module  : '\033[1;32m%s\033[0m'\n", config.scraper.toStdString().c_str());
  if(config.emulator != "") {
    qDebug("Emulator        : '\033[1;32m%s\033[0m'\n", config.emulator.toStdString().c_str());
  }
  qDebug("Input folder    : '\033[1;32m%s\033[0m'\n", config.inputFolder.toStdString().c_str());
  qDebug("Gamelist folder : '\033[1;32m%s\033[0m'\n", config.gameListFolder.toStdString().c_str());
  qDebug("Images folder   : '\033[1;32m%s\033[0m'\n", config.imagesFolder.toStdString().c_str());
  if(config.videos) {
    qDebug("Videos folder   : '\033[1;32m%s\033[0m'\n", config.videosFolder.toStdString().c_str());
  }
  if(config.localDb) {
    qDebug("Local db folder : '\033[1;32m%s\033[0m'\n\n", config.dbFolder.toStdString().c_str());
  }
  
  if(config.scraper == "arcadedb" && config.threads != 1) {
    qDebug("Forcing 1 thread to accomodate limits in ArcadeDB scraping module\n\n");
    config.threads = 1;
  }

  doneThreads = 0;
  notFound = 0;
  found = 0;
  avgCompleteness = 0;
  avgSearchMatch = 0;
  
  QDir inputDir;
  if(config.platform == "amiga") {
    inputDir = QDir(config.inputFolder, "*.uae *.zip *.afd *.dms *.adz *.rp9", QDir::Name, QDir::Files);
  } else if(config.platform == "apple2") {
    inputDir = QDir(config.inputFolder, "*.dsk *.zip", QDir::Name, QDir::Files);
  } else if(config.platform == "arcade") {
    inputDir = QDir(config.inputFolder, "*.zip", QDir::Name, QDir::Files);
  } else if(config.platform == "atari2600") {
    inputDir = QDir(config.inputFolder, "*.bin *.a26 *.rom *.zip *.gz", QDir::Name, QDir::Files);
  } else if(config.platform == "atari5200") {
    inputDir = QDir(config.inputFolder, "*.a52 *.bas *.bin *.car *.xex *.atr *.xfd *.dcm *.atr.gz *.xfd.gz *.zip", QDir::Name, QDir::Files);
  } else if(config.platform == "atari7800") {
    inputDir = QDir(config.inputFolder, "*.a78 *.bin *.zip", QDir::Name, QDir::Files);
  } else if(config.platform == "atarijaguar") {
    inputDir = QDir(config.inputFolder, "*.j64 *.jag *.zip", QDir::Name, QDir::Files);
  } else if(config.platform == "atarist") {
    inputDir = QDir(config.inputFolder, "*.st *.stx *.img *.rom *.raw *.ipf *.ctr *.zip", QDir::Name, QDir::Files);
  } else if(config.platform == "c64") {
    inputDir = QDir(config.inputFolder, "*.crt *.d64 *.prg *.zip *.tap *.t64 *.g64", QDir::Name, QDir::Files);
  } else if(config.platform == "coleco") {
    inputDir = QDir(config.inputFolder, "*.bin *.col *.rom *.zip", QDir::Name, QDir::Files);
  } else if(config.platform == "gb") {
    inputDir = QDir(config.inputFolder, "*.gb *.zip", QDir::Name, QDir::Files);
  } else if(config.platform == "gba") {
    inputDir = QDir(config.inputFolder, "*.gba *.zip", QDir::Name, QDir::Files);
  } else if(config.platform == "gbc") {
    inputDir = QDir(config.inputFolder, "*.gbc *.zip", QDir::Name, QDir::Files);
  } else if(config.platform == "gamegear") {
    inputDir = QDir(config.inputFolder, "*.zip *.gg", QDir::Name, QDir::Files);
  } else if(config.platform == "genesis") {
    inputDir = QDir(config.inputFolder, "*.smd *.bin *.gen *.md *.sg *.zip", QDir::Name, QDir::Files);
  } else if(config.platform == "mastersystem") {
    inputDir = QDir(config.inputFolder, "*.sms *.zip", QDir::Name, QDir::Files);
  } else if(config.platform == "megadrive") {
    inputDir = QDir(config.inputFolder, "*.smd *.bin *.gen *.md *.sg *.zip", QDir::Name, QDir::Files);
  } else if(config.platform == "msx") {
    inputDir = QDir(config.inputFolder, "*.zip *.rom *.mx1 *.mx2 *.col *.dsk", QDir::Name, QDir::Files);
  } else if(config.platform == "n64") {
    inputDir = QDir(config.inputFolder, "*.z64 *.n64 *.v64 *.zip", QDir::Name, QDir::Files);
  } else if(config.platform == "nds") {
    inputDir = QDir(config.inputFolder, "*.zip *.nds", QDir::Name, QDir::Files);
  } else if(config.platform == "neogeo") {
    inputDir = QDir(config.inputFolder, "*.zip", QDir::Name, QDir::Files);
  } else if(config.platform == "nes") {
    inputDir = QDir(config.inputFolder, "*.zip *.nes *.smc *.sfc *.fig *.swc *.mgd", QDir::Name, QDir::Files);
  } else if(config.platform == "pcengine") {
    inputDir = QDir(config.inputFolder, "*.pce *.zip", QDir::Name, QDir::Files);
  } else if(config.platform == "psp") {
    inputDir = QDir(config.inputFolder, "*.cso *.iso *.pbp *.zip", QDir::Name, QDir::Files);
  } else if(config.platform == "psx") {
    inputDir = QDir(config.inputFolder, "*.cue *.cbn *.img *.iso *.m3u *.mdf *.pbp *.toc *.z *.znx *.zip", QDir::Name, QDir::Files);
  } else if(config.platform == "scummvm") {
    inputDir = QDir(config.inputFolder, "*.svm", QDir::Name, QDir::Files);
  } else if(config.platform == "snes") {
    inputDir = QDir(config.inputFolder, "*.zip *.smc *.sfc *.fig *.swc", QDir::Name, QDir::Files);
  } else if(config.platform == "segacd") {
    inputDir = QDir(config.inputFolder, "*.cue *.iso *.zip", QDir::Name, QDir::Files);
  } else if(config.platform == "zxspectrum") {
    inputDir = QDir(config.inputFolder, "*.sna *.szx *.z80 *.tap *.tzx *.gz *.udi *.mgt *.img *.trd *.scl *.dsk *.zip", QDir::Name, QDir::Files);
  }
  if(!inputDir.exists()) {
    qDebug("Input folder '%s' doesn't exist or can't be seen by current user. Please check path and permissions.\n", inputDir.absolutePath().toStdString().c_str());
    exit(1);
  }

  QDir gameListDir(config.gameListFolder);
  checkForFolder(gameListDir);
  config.gameListFolder = gameListDir.absolutePath();
  
  QDir imagesDir(config.imagesFolder);
  checkForFolder(imagesDir);
  config.imagesFolder = imagesDir.absolutePath();

  if(config.videos) {
    QDir videosDir(config.videosFolder);
    checkForFolder(videosDir);
    config.videosFolder = videosDir.absolutePath();
  }

  if(!config.dbFolder.isEmpty() && config.localDb) {
    localDb = QSharedPointer<LocalDb>(new LocalDb(config.dbFolder));
    if(localDb->createFolders(config.scraper)) {
      localDb->readDb();
    } else {
      qDebug("Couldn't create local db folders, disabling localdb...\n");
      config.localDb = false;
    }
  }
  if(config.localDb && config.cleanDb) {
    localDb->cleanDb();
    exit(0);
  }
  if(config.localDb && !config.mergeDb.isEmpty() && QDir(config.mergeDb).exists()) {
    LocalDb srcDb(config.mergeDb);
    srcDb.readDb();
    localDb->mergeDb(srcDb, config.updateDb, config.mergeDb);
    localDb->writeDb();
    exit(0);
  }
  if(config.localDb) {
    localDb->readPriorities();
  }
  
  gameListFileString = gameListDir.absolutePath() + "/" + frontend->getGameListFileName();

  QFile gameListFile(gameListFileString);

  if(!config.pretend && !config.unattend && gameListFile.exists()) {
    std::string userInput = "";
    qDebug("\033[1;34m'%s' already exists, do you want to overwrite it? (y/n):\033[0m ", frontend->getGameListFileName().toStdString().c_str());
    std::cin >> userInput;
    if(userInput != "y") {
      qDebug("User chose not to overwrite, now exiting...\n");
      exit(0);
    }
    
    qDebug("Checking if '%s' is writable?... ", frontend->getGameListFileName().toStdString().c_str());
    
    if(gameListFile.open(QIODevice::Append)) {
      qDebug("\033[1;32mIt is! :)\033[0m\n");
      gameListFile.close();
    } else {
      qDebug("\033[1;31mIt isn't! :(\nPlease check path and permissions and try again.\033[0m\n");
      exit(1);
    }
  }
  if(config.pretend) {
    qDebug("Pretend set! Not changing any files except '%s'.\n\n", skippedFileString.toStdString().c_str());
  }

  QFile skippedFile(skippedFileString);
  skippedFile.open(QIODevice::WriteOnly);
  skippedFile.write("--- The following is a list of skipped games ---\n");
  skippedFile.close();

  QList<QFileInfo> inputFiles = inputDir.entryInfoList();
  if(config.subDirs) {
    QDirIterator dirIt(config.inputFolder,
		       QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks,
		       QDirIterator::Subdirectories);
    while(dirIt.hasNext()) {
      QString subdir = dirIt.next();
      inputDir.setPath(subdir);
      inputFiles.append(inputDir.entryInfoList());
      if(config.verbose) {
	qDebug("Added files from subdir: '%s'\n", subdir.toStdString().c_str());
      }
    }
  }

  if(!config.unattend) {
    std::string userInput = "";
    if(gameListFile.exists() && frontend->canSkip()) {
      qDebug("\033[1;34mDo you wish to skip existing entries? (y/n):\033[0m ");
      std::cin >> userInput;
      if(userInput == "y" && frontend->canSkip()) {
	frontend->skipExisting(gameListFileString, gameEntries, inputFiles);
      }
    }
  }

  totalFiles = inputFiles.length();
  if(totalFiles == 0) {
    // A bit of a hack to let the scraping process take place. We want it to rewrite the gamelist
    qDebug("No entries to scrape...\n\n");
    doneThreads = config.threads - 1;
    checkThreads();
    exit(0);
  }
  
  qDebug("\nStarting scraping run on \033[1;32m%d\033[0m files using \033[1;32m%d\033[0m threads.\nSit back, relax and let me do the work! :)\n\n", totalFiles, config.threads);

  timer.start();
  currentFile = 1;

  int filesPerThread = (totalFiles - totalFiles % config.threads) / config.threads;
  if(totalFiles < config.threads) {
    filesPerThread = 1;
  }
  
  if(config.verbose) {
    qDebug("Files per thread: %d\n", filesPerThread);
  }
  QList<QThread*> threadList;
  for(int curThread = 1; curThread <= config.threads; ++curThread) {
    int beginIdx = (curThread - 1) * filesPerThread;
    if(curThread == config.threads) {
      filesPerThread += totalFiles % config.threads;
    }
    QThread *thread = new QThread;
    ScraperWorker *worker = new ScraperWorker(inputFiles, filesPerThread, beginIdx, config, localDb);
    worker->moveToThread(thread);
    connect(thread, &QThread::started, worker, &ScraperWorker::run);
    connect(worker, &ScraperWorker::outputToTerminal, this, &Skyscraper::outputToTerminal);
    connect(worker, &ScraperWorker::entryReady, this, &Skyscraper::entryReady);
    //connect(worker, &ScraperWorker::addToSkipped, this, &Skyscraper::addToSkipped);
    connect(worker, &ScraperWorker::allDone, this, &Skyscraper::checkThreads);
    connect(thread, &QThread::finished, worker, &ScraperWorker::deleteLater);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
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
  }
}

void Skyscraper::checkForFolder(QDir &folder)
{
  if(!folder.exists()) {
    qDebug("Folder '%s' doesn't exist, trying to create it... ", folder.absolutePath().toStdString().c_str());
    if(folder.mkpath(folder.absolutePath())) {
      qDebug("Success!\n\n");
    } else {
      qDebug("Failed! Please check path and permissions\n");
      exit(1);
    }
  }
}

void Skyscraper::outputToTerminal(const QString &output)
{
  outputMutex.lock();

  qDebug("\033[0;32m#%d/%d\033[0m %s\n", currentFile, totalFiles, output.toStdString().c_str());
  int elapsed = timer.elapsed();
  int estTime = elapsed / currentFile * totalFiles;

  qDebug("Elapsed time: %s\n", secsToString(elapsed).toStdString().c_str());
  qDebug("Estimated time: %s\n\n", secsToString(estTime).toStdString().c_str());

  outputMutex.unlock();
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

void Skyscraper::entryReady(const GameEntry &entry)
{
  entryMutex.lock();

  GameEntry tmpEntry = entry;

  if(tmpEntry.found) {
    found++;
    avgCompleteness += tmpEntry.completeness(config.videos);
    avgSearchMatch += tmpEntry.searchMatch;
    // Remove unnecessary media data to save memory before adding it to final entrylist
    // At this point data has been saved to disc, so we don't need it anymore.
    tmpEntry.coverData = QImage();
    tmpEntry.screenshotData = QImage();
    tmpEntry.videoData = "";
    gameEntries.append(tmpEntry);
  } else {
    notFound++;
    QFile skippedFile(skippedFileString);
    skippedFile.open(QIODevice::Append);
    skippedFile.write("'" + tmpEntry.baseName.toUtf8() + "'");
    if(tmpEntry.searchMatch == 0) {
      skippedFile.write(", No returned matches\n");
    } else {
      skippedFile.write(", Closest match was '" + tmpEntry.title.toUtf8() + "'\n");
    }
    skippedFile.close();
    if(config.skipped) {
      gameEntries.append(tmpEntry);
    }
  }
  
  if(currentFile == 30 && notFound == 30 &&
     config.scraper != "import" && config.scraper != "localdb") {
    qDebug("\033[1;31mThis is NOT going well! I guit! *slams the door*\nNo, seriously, out of 30 files we had 30 misses, which probably means you are using a scraping source that doesn't support this platform. Please be considerate and choose a scraping module that supports the platform you are scraping (check '--help').\n\nNow exiting...\033[0m\n");
    exit(1);
  }

  currentFile++;

  entryMutex.unlock();
}

void Skyscraper::checkThreads()
{
  checkThreadMutex.lock();

  doneThreads++;
  if(doneThreads == config.threads) {
    qDebug("\033[1;34m---- Scraping run completed! YAY! ----\033[0m\n");
    if(!config.pretend && !config.dbFolder.isEmpty() && config.localDb) {
      localDb->writeDb();
    }

    frontend->sortEntries(gameEntries);

    QString finalOutput;
    frontend->assembleList(finalOutput, gameEntries, config.maxLength);
    
    if(!config.pretend) {
      QFile gameListFile(gameListFileString);
      qDebug("Now writing '%s'... ", gameListFileString.toStdString().c_str());
      if(gameListFile.open(QIODevice::WriteOnly)) {
	gameListFile.write(finalOutput.toUtf8());
	gameListFile.close();
	qDebug("\033[1;32mSuccess!!!\033[0m\n\n");
      } else {
	qDebug("\033[1;31mCouldn't open file for writing!!!\nAll that work for nothing... :(\033[0m\n");
      }
    }

    qDebug("\033[1;34m---- And here are some neat stats :) ----\033[0m\n");
    qDebug("Total completion time: \033[1;33m%s\033[0m\n\n", secsToString(timer.elapsed()).toStdString().c_str());
    if(found > 0) {
      qDebug("Average search match: \033[1;33m%d%%\033[0m\n",
	     (int)((double)avgSearchMatch / (double)found));
      qDebug("Average entry completeness: \033[1;33m%d%%\033[0m\n\n",
	     (int)((double)avgCompleteness / (double)found));
    }
    qDebug("\033[1;34mTotal number of games: %d\033[0m\n", totalFiles);
    qDebug("\033[1;32mSuccessfully scraped games: %d\033[0m\n", found);
    qDebug("\033[1;33mSkipped games: %d (Filenames saved to '%s')\033[0m\n\n", notFound, skippedFileString.toStdString().c_str());

    // All done, now clean up and exit to terminal
    emit finished();
  }

  checkThreadMutex.unlock();
}

void Skyscraper::loadConfig(const QCommandLineParser &parser)
{
  if(!QFileInfo::exists("config.ini")) {
    QFile::copy("/usr/local/etc/skyscraper/config.ini.example", "config.ini");
  }

  if(QFileInfo::exists("README.md")) {
    QFile::remove("README.md");
  }
  QFile::copy("/usr/local/etc/skyscraper/README.md", "README.md");

  if(QFileInfo::exists("mameMap.csv")) {
    QFile::remove("mameMap.csv");
  }
  QFile::copy("/usr/local/etc/skyscraper/mameMap.csv", "mameMap.csv");

  if(!QFileInfo::exists("config_artwork01.ini")) {
    QFile::copy("/usr/local/etc/skyscraper/config_artwork01.ini", "config_artwork01.ini");
  }

  if(!QFileInfo::exists("config_artwork02.ini")) {
    QFile::copy("/usr/local/etc/skyscraper/config_artwork02.ini", "config_artwork02.ini");
  }

  if(!QFileInfo::exists("config_artwork03.ini")) {
    QFile::copy("/usr/local/etc/skyscraper/config_artwork03.ini", "config_artwork03.ini");
  }

  if(QFileInfo::exists("dbs/README.md")) {
    QFile::remove("dbs/README.md");
  }
  QFile::copy("/usr/local/etc/skyscraper/dbs/README.md", "dbs/README.md");

  if(QFileInfo::exists("dbs/priorities.xml.example")) {
    QFile::remove("dbs/priorities.xml.example");
  }
  QFile::copy("/usr/local/etc/skyscraper/dbs/priorities.xml.example", "dbs/priorities.xml.example");

  if(QFileInfo::exists("import/README.md")) {
    QFile::remove("import/README.md");
  }
  QFile::copy("/usr/local/etc/skyscraper/import/README.md", "import/README.md");
  if(QFileInfo::exists("import/definitions.dat.example1")) {
    QFile::remove("import/definitions.dat.example1");
  }
  QFile::copy("/usr/local/etc/skyscraper/import/definitions.dat.example1", "import/definitions.dat.example1");
  if(QFileInfo::exists("import/definitions.dat.example2")) {
    QFile::remove("import/definitions.dat.example2");
  }
  QFile::copy("/usr/local/etc/skyscraper/import/definitions.dat.example2", "import/definitions.dat.example2");

  QSettings settings(parser.isSet("c")?parser.value("c"):"config.ini", QSettings::IniFormat);

  // Artwork config
  settings.beginGroup("artwork");
  if(settings.contains("finalImageWidth")) {
    config.finalImageWidth = settings.value("finalImageWidth").toInt();
  }
  if(settings.contains("finalImageHeight")) {
    config.finalImageHeight = settings.value("finalImageHeight").toInt();
  }
  if(settings.contains("coverEnabled")) {
    config.coverEnabled = settings.value("coverEnabled").toBool();
  }
  if(settings.contains("coverWidth")) {
    config.coverWidth = settings.value("coverWidth").toInt();
  }
  if(settings.contains("coverHeight")) {
    config.coverHeight = settings.value("coverHeight").toInt();
  }
  if(settings.contains("coverX")) {
    config.coverX = settings.value("coverX").toInt();
  }
  if(settings.contains("coverY")) {
    config.coverY = settings.value("coverY").toInt();
    config.coverYSet = true;
  }
  if(settings.contains("coverShadowEnabled")) {
    config.coverShadowEnabled = settings.value("coverShadowEnabled").toBool();
  }
  if(settings.contains("coverShadowDistance")) {
    config.coverShadowDistance = settings.value("coverShadowDistance").toInt();
  }
  if(settings.contains("coverShadowSoftness")) {
    config.coverShadowSoftness = settings.value("coverShadowSoftness").toInt();
  }
  if(settings.contains("coverShadowOpacity")) {
    config.coverShadowOpacity = settings.value("coverShadowOpacity").toInt();
  }
  if(settings.contains("screenshotEnabled")) {
    config.screenshotEnabled = settings.value("screenshotEnabled").toBool();
  }
  if(settings.contains("screenshotWidth")) {
    config.screenshotWidth = settings.value("screenshotWidth").toInt();
  }
  if(settings.contains("screenshotHeight")) {
    config.screenshotHeight = settings.value("screenshotHeight").toInt();
  }
  if(settings.contains("screenshotX")) {
    config.screenshotX = settings.value("screenshotX").toInt();
    config.screenshotXSet = true;
  }
  if(settings.contains("screenshotY")) {
    config.screenshotY = settings.value("screenshotY").toInt();
  }
  if(settings.contains("screenshotShadowEnabled")) {
    config.screenshotShadowEnabled = settings.value("screenshotShadowEnabled").toBool();
  }
  if(settings.contains("screenshotShadowDistance")) {
    config.screenshotShadowDistance = settings.value("screenshotShadowDistance").toInt();
  }
  if(settings.contains("screenshotShadowSoftness")) {
    config.screenshotShadowSoftness = settings.value("screenshotShadowSoftness").toInt();
  }
  if(settings.contains("screenshotShadowOpacity")) {
    config.screenshotShadowOpacity = settings.value("screenshotShadowOpacity").toInt();
  }
  settings.endGroup();

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
  bool imagesFolderSet = false;
  bool videosFolderSet = false;
  
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
  if(settings.contains("pretend")) {
    config.pretend = settings.value("pretend").toBool();
  }
  if(settings.contains("unattend")) {
    config.unattend = settings.value("unattend").toBool();
  }
  if(settings.contains("verbose")) {
    config.verbose = settings.value("verbose").toBool();
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
  if(settings.contains("skipped")) {
    config.skipped = settings.value("skipped").toBool();
  }
  if(settings.contains("brackets")) {
    config.brackets = !settings.value("brackets").toBool();
  }
  settings.endGroup();

  // Check for command line platform here, since we need it for 'platform' config.ini entries
  if(parser.isSet("p") && (parser.value("p") == "amiga" ||
			   parser.value("p") == "arcade" ||
			   parser.value("p") == "apple2" ||
			   parser.value("p") == "atari2600" ||
			   parser.value("p") == "atari5200" ||
			   parser.value("p") == "atari7800" ||
			   parser.value("p") == "atarijaguar" ||
			   parser.value("p") == "atarist" ||
			   parser.value("p") == "c64" ||
			   parser.value("p") == "coleco" ||
			   parser.value("p") == "gamegear" ||
			   parser.value("p") == "gb" ||
			   parser.value("p") == "gba" ||
			   parser.value("p") == "gbc" ||
			   parser.value("p") == "genesis" ||
			   parser.value("p") == "mastersystem" ||
			   parser.value("p") == "megadrive" ||
			   parser.value("p") == "msx" ||
			   parser.value("p") == "n64" ||
			   parser.value("p") == "nds" ||
			   parser.value("p") == "neogeo" ||
			   parser.value("p") == "nes" ||
			   parser.value("p") == "pcengine" ||
			   parser.value("p") == "psp" ||
			   parser.value("p") == "psx" ||
			   parser.value("p") == "scummvm" ||
			   parser.value("p") == "segacd" ||
			   parser.value("p") == "snes" ||
			   parser.value("p") == "zxspectrum")) {
    config.platform = parser.value("p");
  } else {
    qDebug("Please set a valid platform with '-p [platform]'\nCheck '--help' for a list of supported platforms, now exiting...\n");
    exit(1);
  }

  // Platform specific config, overrides main and defaults
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
  if(settings.contains("imagesFolder")) {
    config.imagesFolder = settings.value("imagesFolder").toString();;
    imagesFolderSet = true;
  }
  if(settings.contains("videos")) {
    config.videos = settings.value("videos").toBool();
  }
  if(settings.contains("skipped")) {
    config.skipped = settings.value("skipped").toBool();
  }
  if(settings.contains("videosFolder")) {
    config.videosFolder = settings.value("videosFolder").toString();;
    videosFolderSet = true;
  }
  if(settings.contains("videosFolder")) {
    config.videosFolder = settings.value("videosFolder").toString();;
    videosFolderSet = true;
  }
  if(settings.contains("brackets")) {
    config.brackets = !settings.value("brackets").toBool();
  }
  if(settings.contains("minMatch")) {
    config.minMatch = settings.value("minMatch").toInt();
  }
  if(settings.contains("scraper")) {
    config.scraper = settings.value("scraper").toString();
  }
  if(settings.contains("userCreds")) {
    config.userCreds = settings.value("userCreds").toString();
  }
  if(settings.contains("threads")) {
    config.threads = settings.value("threads").toInt();
  }
  if(settings.contains("pretend")) {
    config.pretend = settings.value("pretend").toBool();
  }
  if(settings.contains("unattend")) {
    config.unattend = settings.value("unattend").toBool();
  }
  if(settings.contains("verbose")) {
    config.verbose = settings.value("verbose").toBool();
  }
  if(settings.contains("maxLength")) {
    config.maxLength = settings.value("maxLength").toInt();
  }
  settings.endGroup();
  
  // Command line configs, overrides platform, main and defaults
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
    config.imagesFolder = parser.value("o");
    imagesFolderSet = true;
  }
  if(parser.isSet("v")) {
    config.videosFolder = parser.value("v");
    videosFolderSet = true;
  }
  if(parser.isSet("m") && parser.value("m").toInt() >= 0 && parser.value("m").toInt() <= 100) {
    config.minMatch = parser.value("m").toInt();
  }
  if(parser.isSet("s") && (parser.value("s") == "openretro" ||
			   parser.value("s") == "thegamesdb" ||
			   parser.value("s") == "arcadedb" ||
			   parser.value("s") == "worldofspectrum" ||
			   parser.value("s") == "screenscraper" ||
			   parser.value("s") == "localdb" ||
			   parser.value("s") == "import")) {
    config.scraper = parser.value("s");
  }
  if(parser.isSet("u") && parser.value("u").indexOf(":") != -1) {
    config.userCreds = parser.value("u");
  }
  if(parser.isSet("d")) {
    config.dbFolder = parser.value("d");
  } else {
    config.dbFolder = "dbs/" + config.platform;
  }
  if(parser.isSet("videos")) {
    config.videos = true;
  }
  if(parser.isSet("skipped")) {
    config.skipped = true;
  }
  if(parser.isSet("nobrackets")) {
    config.brackets = false;
  }
  if(parser.isSet("nolocaldb")) {
    if(config.scraper == "localdb") {
      qDebug("Selected scraper 'localdb' can't be used with '--nolocaldb' flag, exiting.\n");
      exit(1);
    }
    config.localDb = false;
  }
  if(parser.isSet("cleandb")) {
    config.cleanDb = true;
  }
  if(parser.isSet("mergedb") && QDir(config.mergeDb).exists()) {
    config.mergeDb = parser.value("mergedb");
  }
  if(parser.isSet("updatedb")) {
    config.updateDb = true;
  }
  if(parser.isSet("nosubdirs")) {
    config.subDirs = false;
  }
  if(parser.isSet("pretend")) {
    config.pretend = true;
  }
  if(parser.isSet("unattend")) {
    config.unattend = true;
  }
  if(parser.isSet("region")) {
    config.region = parser.value("region");
  }
  if(parser.isSet("lang")) {
    config.lang = parser.value("lang");
  }
  if(parser.isSet("verbose")) {
    config.verbose = true;
  }
  if(parser.isSet("l") && parser.value("l").toInt() >= 0 && parser.value("l").toInt() <= 10000) {
    config.maxLength = parser.value("l").toInt();
  }

  frontend->checkReqs();

  // Change config defaults if they aren't already set, find the rest in settings.h
  if(!inputFolderSet) {
    config.inputFolder = frontend->getInputFolder();
  }
  if(!gameListFolderSet) {
    config.gameListFolder = frontend->getGameListFolder();
  }
  if(!imagesFolderSet) {
    config.imagesFolder = frontend->getImagesFolder();
  }
  if(!videosFolderSet) {
    config.videosFolder = frontend->getVideosFolder();
  }

  // Choose default scraper for chosen platform if none has been set yet
  if(config.scraper.isEmpty()) {
    if(config.platform == "amiga") {
      config.scraper = "openretro";
    } else if(config.platform == "arcade") {
      config.scraper = "arcadedb";
    } else if(config.platform == "apple2") {
      config.scraper = "thegamesdb";
    } else if(config.platform == "atari2600") {
      config.scraper = "thegamesdb";
    } else if(config.platform == "atari5200") {
      config.scraper = "thegamesdb";
    } else if(config.platform == "atari7800") {
      config.scraper = "thegamesdb";
    } else if(config.platform == "atarijaguar") {
      config.scraper = "thegamesdb";
    } else if(config.platform == "atarist") {
      config.scraper = "thegamesdb";
    } else if(config.platform == "c64") {
      config.scraper = "thegamesdb";
    } else if(config.platform == "coleco") {
      config.scraper = "thegamesdb";
    } else if(config.platform == "gamegear") {
      config.scraper = "thegamesdb";
    } else if(config.platform == "gb") {
      config.scraper = "thegamesdb";
    } else if(config.platform == "gba") {
      config.scraper = "thegamesdb";
    } else if(config.platform == "gbc") {
      config.scraper = "thegamesdb";
    } else if(config.platform == "genesis") {
      config.scraper = "thegamesdb";
    } else if(config.platform == "megadrive") {
      config.scraper = "thegamesdb";
    } else if(config.platform == "mastersystem") {
      config.scraper = "thegamesdb";
    } else if(config.platform == "msx") {
      config.scraper = "thegamesdb";
    } else if(config.platform == "n64") {
      config.scraper = "thegamesdb";
    } else if(config.platform == "nds") {
      config.scraper = "thegamesdb";
    } else if(config.platform == "neogeo") {
      config.scraper = "arcadedb";
    } else if(config.platform == "nes") {
      config.scraper = "thegamesdb";
    } else if(config.platform == "pcengine") {
      config.scraper = "thegamesdb";
    } else if(config.platform == "psp") {
      config.scraper = "thegamesdb";
    } else if(config.platform == "psx") {
      config.scraper = "thegamesdb";
    } else if(config.platform == "scummvm") {
      config.scraper = "openretro";
    } else if(config.platform == "segacd") {
      config.scraper = "thegamesdb";
    } else if(config.platform == "snes") {
      config.scraper = "thegamesdb";
    } else if(config.platform == "zxspectrum") {
      config.scraper = "worldofspectrum";
    }
  }

  skippedFileString = "skipped-" + config.scraper + ".txt";
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

  /*
  QDir dir("/mnt/data/roms/c64_d64", "*.d64", QDir::Name | QDir::IgnoreCase, QDir::Files | QDir::NoDotAndDotDot);
  QFileInfo infoPrev;
  foreach(QFileInfo info, dir.entryInfoList()) {
    QString baseName = info.completeBaseName().toLower();
    //QString baseNamePrev = infoPrev.completeBaseName().toLower();
    if(baseName.left(baseNamePrev.length()) == baseNamePrev &&
       baseName.indexOf("[") != -1) {
      qDebug("X: %s\n", baseNamePrev.toStdString().c_str());
      QFile::remove(infoPrev.absoluteFilePath());
    } else {
      qDebug(" : %s\n", baseNamePrev.toStdString().c_str());
    }
    infoPrev = info;
    if(baseName.indexOf("side a") == -1 &&
       baseName.indexOf("side b") == -1 &&
       baseName.indexOf("(disk") == -1) {
      QFile::rename(info.absoluteFilePath(), info.absolutePath() + "/scrape/" + info.fileName());
      continue;
    }

    if(baseName.indexOf("(side a)") != -1 ||
       (baseName.indexOf("(disk 1") != -1 && baseName.indexOf("side a") != -1)) {
      QFile::rename(info.absoluteFilePath(), info.absolutePath() + "/scrape/" + info.fileName());
    }
  }
  exit(0);
  */
