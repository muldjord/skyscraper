/***************************************************************************
 *            main.cpp
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

#include <signal.h>

#include <QCoreApplication>
#include <QDir>
#include <QtDebug>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QTimer>
#include <QTextCodec>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include "strtools.h"
#include "skyscraper.h"
#include "scripter.h"
#include "platform.h"

Skyscraper *x;
int sigIntRequests = 0;

void customMessageHandler(QtMsgType type, const QMessageLogContext&, const QString &msg)
{
  QString txt;
  // Decide which type of debug message it is, and add string to signify it
  // Then append the debug message itself to the same string.
  switch (type) {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 5, 0))
  case QtInfoMsg:
    txt += QString("INFO: '%1'").arg(msg);
    break;
#endif
  case QtDebugMsg:
    txt += QString("DEBUG: '%1'").arg(msg);
    break;
  case QtWarningMsg:
    //txt += QString("Warning: %1").arg(msg);
    break;
  case QtCriticalMsg:
    txt += QString("CRITICAL: '%1'").arg(msg);
    break;
  case QtFatalMsg:
    txt += QString("FATAL: '%1'").arg(msg);
    abort();
  }
  printf("%s", txt.toStdString().c_str());
  fflush(stdout);
}

void sigHandler(int signal) {
  if(signal == 2) {
    sigIntRequests++;
    if(sigIntRequests <= 2) {
      if(x->threadsRunning) {
	printf("User wants to quit, trying to exit nicely. This can take a few seconds depending on how many threads you have running...\n");
	x->queue->clearAll();
      } else {
	exit(1);
      }
    } else {
      printf("User REALLY wants to quit NOW, forcing unclean exit...\n");
      exit(1);
    }
  }
}

int main(int argc, char *argv[])
{
  struct sigaction sigIntHandler;
  
  sigIntHandler.sa_handler = sigHandler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  
  sigaction(SIGINT, &sigIntHandler, NULL);
  
  QCoreApplication app(argc, argv);
  app.setApplicationVersion(VERSION);
  
  // Get current dir. If user has specified file(s) on command line we need this.
  QString currentDir = QDir::currentPath();

  // Set the working directory to the applications own path
  QDir skyDir(QDir::homePath() + "/.skyscraper");
  if(!skyDir.exists()) {
    if(!skyDir.mkpath(".")) {
      printf("Couldn't create folder '%s'. Please check permissions, now exiting...\n", skyDir.absolutePath().toStdString().c_str());
      exit(1);
    }
  }

  // Create import paths
  skyDir.mkpath("import");
  skyDir.mkpath("import/textual");
  skyDir.mkpath("import/screenshots");
  skyDir.mkpath("import/covers");
  skyDir.mkpath("import/wheels");
  skyDir.mkpath("import/marquees");
  skyDir.mkpath("import/videos");

  // Create resources folder
  skyDir.mkpath("resources");

  // Rename 'dbs' folder to migrate 2.x users to 3.x
  skyDir.rename(skyDir.absolutePath() + "/dbs", skyDir.absolutePath() + "/cache");

  // Create cache folder
  skyDir.mkpath("cache");
  QDir::setCurrent(skyDir.absolutePath());

  // Install the custom debug message handler used by qDebug()
  qInstallMessageHandler(customMessageHandler);

  QString platforms;
  foreach(QString platform, Platform::getPlatforms()) {
    platforms.append("'" + platform + "', ");
  }
  // Remove the last ', '
  platforms = platforms.left(platforms.length() - 2);

  QCommandLineParser parser;

  QString headerString = "Running Skyscraper v" VERSION " by Lars Muldjord";
  QString dashesString = "";
  for(int a = 0; a < headerString.length(); ++a) {
    dashesString += "-";
  }

  parser.setApplicationDescription(StrTools::getVersionHeader() + "Skyscraper looks for compatible game files in the input directory (set with '-i'). It allows you to fetch and cache media and game information resources for the files using various scraping modules (set with '-s'). It then lets you built game lists for the supported frontends by combining all previously cached resources ('game list generation mode' is initiated by simply leaving out the '-s' option). While doing so it also composites game art for all files by following the recipe at '~/.skyscraper/artwork.xml'.\n\nIn addition to the command line options Skyscraper also provides a lot of customizable options for configuration, artwork, game name aliases, resource priorities and much more. Please check the full documentation at 'https://github.com/muldjord/skyscraper/docs' for a detailed explanation of all features.\n\nRemember that most of the following options can also be set in the '~/.skyscraper/config.ini' file. All cli options and config.ini options are thoroughly documented in the above link.");
  parser.addHelpOption();
  parser.addVersionOption();
  QCommandLineOption pOption("p", "The platform you wish to scrape.\n(Currently supports " + platforms + ".)", "PLATFORM", "");
  QCommandLineOption fOption("f", "Frontend to scrape for.\n(Currently supports 'emulationstation' and 'attractmode'. Default is 'emulationstation')", "FRONTEND", "");
  QCommandLineOption eOption("e", "Set emulator. This is only required by the 'attractmode' frontend.\n(Default is none)", "EMULATOR", "");
  QCommandLineOption iOption("i", "Folder which contains the game/rom files.\n(default is '~/RetroPie/roms/<PLATFORM>')", "PATH", "");
  QCommandLineOption gOption("g", "Game list export folder.\n(default depends on frontend)", "PATH", "");
  QCommandLineOption oOption("o", "Game media export folder.\n(default depends on frontend)", "PATH", "");
  QCommandLineOption sOption("s", "Choose scraping module to gather resources from for selected platform.\n(WEB: 'arcadedb', 'igdb', 'mobygames', 'openretro', 'screenscraper', 'thegamesdb' and 'worldofspectrum', LOCAL: 'esgamelist' and 'import'. Leave '-s' out to generate game list from cached resources)", "MODULE", "");
  QCommandLineOption uOption("u", "userKey or UserID and Password for use with the selected scraping module.\n(Default is none)", "KEY/USER:PASSWORD", "");
  QCommandLineOption mOption("m", "Minimum match percentage when comparing search result titles to filename titles.\n(default is 65)", "0-100", "");
  QCommandLineOption lOption("l", "Maximum game description length. Everything longer than this will be truncated.\n(default is 2500)", "0-10000", "");
  QCommandLineOption tOption("t", "Number of scraper threads to use. This might change depending on the scraping module limits.\n(default is 4)", "1-8", "");
  QCommandLineOption cOption("c", "Use this config file to set up Skyscraper.\n(default is '~/.skyscraper/config.ini')", "FILENAME", "");
  QCommandLineOption aOption("a", "Use this artwork xml file to set up the artwork compositing.\n(default is '~/.skyscraper/artwork.xml')", "FILENAME", "");
  QCommandLineOption dOption("d", "Set custom resource cache folder.\n(default is '~/.skyscraper/cache/<PLATFORM>')", "FOLDER", "");
  QCommandLineOption videosOption("videos", "Enables scraping and caching of videos for the scraping modules that support them. Beware, this takes up a lot of disk space!");
  QCommandLineOption symlinkOption("symlink", "Forces cached videos to be symlinked to game list destination to save space. WARNING! Deleting or moving files from your cache can invalidate the links!");
  QCommandLineOption nocoversOption("nocovers", "Disable covers/boxart from being cached locally. Only do this if you do not plan to use the cover artwork in 'artwork.xml'");
  QCommandLineOption noscreenshotsOption("noscreenshots", "Disable screenshots/snaps from being cached locally. Only do this if you do not plan to use the screenshot artwork in 'artwork.xml'");
  QCommandLineOption nowheelsOption("nowheels", "Disable wheels from being cached locally. Only do this if you do not plan to use the wheel artwork in 'artwork.xml'");
  QCommandLineOption nomarqueesOption("nomarquees", "Disable marquees from being cached locally. Only do this if you do not plan to use the marquee artwork in 'artwork.xml'");
  QCommandLineOption skippedOption("skipped", "Include skipped entries when writing final gamelist.");
  QCommandLineOption nobracketsOption("nobrackets", "Disables any [] and () tags in the frontend game titles.");
  QCommandLineOption relativeOption("relative", "Forces all gamelist paths to be relative to rom location.");
  QCommandLineOption addextOption("addext", "Add this or these file extension(s) to accepted file extensions during a scraping run. (example: '*.zst' or '*.zst *.ext)", "EXTENSION(S)", "");
  QCommandLineOption cacheOption("cache", "This option is the master option for all options related to the resource cache. It must be followed by 'COMMAND[:OPTIONS]'.\n'show' Will print a status of all cached resources.\n'validate' will check the consistency of the cache.\n'vacuum' Will compare your romset to any cached resource and remove the resources that you no longer have roms for.\n'purge:all' Will remove ALL cached resources for the selected platform.\n'merge:<PATH>' will merge two caches together.\n'purge:m=<MODULE>,t=<TYPE>' Will remove cached resources related to the selected module(m) and / or type(t). Either one can be left out in which case ALL resources from the selected module or ALL resources from the selected type will be removed.\n'refresh' Will force a refresh of existing cached resources for any scraping module. Requires a scraping module set with '-s'.", "COMMAND[:OPTIONS]", "");
  QCommandLineOption refreshOption("refresh", "DEPRECATED! Please use '--cache refresh' instead.");
  QCommandLineOption noresizeOption("noresize", "Disable resizing of artwork when saving it to the resource cache. Normally they are resized to save space. Setting this option will save them as is. NOTE! This is NOT related to how Skyscraper renders the artwork when scraping. Check the online 'Artwork' documentation to know more about this.");
  QCommandLineOption nosubdirsOption("nosubdirs", "Do not include input folder subdirectories when scraping.");
  QCommandLineOption unpackOption("unpack", "Unpacks and checksums the file inside 7z or zip files instead of the compressed file itself. Be aware that this option requires '7z' to be installed on the system to work. Only relevant for 'screenscraper' scraping module.");
  QCommandLineOption forcefilenameOption("forcefilename", "Use filename as game name instead of the returned game title when generating a game list.");
  QCommandLineOption startatOption("startat", "Tells Skyscraper which file to start at. Forces '--refresh' and '--nosubdirs' enabled.", "FILENAME", "");
  QCommandLineOption endatOption("endat", "Tells Skyscraper which file to end at. Forces '--refresh' and '--nosubdirs' enabled.", "FILENAME", "");
  QCommandLineOption maxfailsOption("maxfails", "Sets the allowed number of initial 'Not found' results before rage-quitting. (Default is 42)", "1-200", "");
  QCommandLineOption pretendOption("pretend", "Only relevant when generating a game list. It disables the game list generator and artwork compositor and only outputs the results of the potential game list generation to the terminal. Use it to check what and how the data will be combined from cached resources.");
  QCommandLineOption unattendOption("unattend", "Don't ask any initial questions when scraping. It will then always overwrite existing gamelist and not skip existing entries.");
  QCommandLineOption unattendskipOption("unattendskip", "Don't ask any initial questions when scraping. It will then always overwrite existing gamelist and always skip existing entries.");
  QCommandLineOption interactiveOption("interactive", "Always ask user to choose best result.");
  QCommandLineOption queryOption("query", "Allows you to set a custom search query (eg. 'rick+dangerous' for name based modules or 'sha1=<CHECKSUM>', 'md5=<CHECKSUM>' or 'romnom=<FILENAME>' for the 'screenscraper' module). Requires the single rom filename you wish to override for to be passed on command line as well, otherwise it will be ignored.", "QUERY", "");
  QCommandLineOption regionOption("region", "Add preferred game region for scraping modules that support it.\n(Default prioritization is 'eu', 'us', 'wor' and 'jp' + others in that order)", "CODE", "eu");
  QCommandLineOption langOption("lang", "Set preferred result language for scraping modules that support it.\n(Default 'en')", "CODE", "en");
  QCommandLineOption nohintsOption("nohints", "Disables the 'DID YOU KNOW:' hints when running Skyscraper.");
  QCommandLineOption verbosityOption("verbosity", "Print more info while scraping\n(Default is 0.)", "0-3", "0");
  
  parser.addOption(pOption);
  parser.addOption(sOption);
  parser.addOption(uOption);
  parser.addOption(iOption);
  parser.addOption(nosubdirsOption);
  parser.addOption(unpackOption);
  parser.addOption(gOption);
  parser.addOption(oOption);
  parser.addOption(tOption);
  parser.addOption(fOption);
  parser.addOption(eOption);
  parser.addOption(mOption);
  parser.addOption(lOption);
  parser.addOption(cOption);
  parser.addOption(aOption);
  parser.addOption(dOption);
  parser.addOption(cacheOption);
  parser.addOption(refreshOption);
  parser.addOption(videosOption);
  parser.addOption(symlinkOption);
  parser.addOption(nocoversOption);
  parser.addOption(noscreenshotsOption);
  parser.addOption(nowheelsOption);
  parser.addOption(nomarqueesOption);
  parser.addOption(nobracketsOption);
  parser.addOption(skippedOption);
  parser.addOption(noresizeOption);
  parser.addOption(startatOption);
  parser.addOption(endatOption);
  parser.addOption(maxfailsOption);
  parser.addOption(pretendOption);
  parser.addOption(unattendOption);
  parser.addOption(unattendskipOption);
  parser.addOption(interactiveOption);
  parser.addOption(queryOption);
  parser.addOption(forcefilenameOption);
  parser.addOption(relativeOption);
  parser.addOption(addextOption);
  parser.addOption(langOption);
  parser.addOption(regionOption);
  parser.addOption(nohintsOption);
  parser.addOption(verbosityOption);

  parser.process(app);

  if(argc > 1) {
    if(parser.isSet("help") || parser.isSet("h")) {
      parser.showHelp();
    } else {
      x = new Skyscraper(parser, currentDir);
      QObject::connect(x, &Skyscraper::finished, &app, &QCoreApplication::quit);
      QTimer::singleShot(0, x, SLOT(run()));
    }
    return app.exec();
  } else {
    Scripter scripter;
    return 0;
  }
}
