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

#include <QtGlobal>

// Includes for Linux and MacOS
#if defined(Q_OS_LINUX) || defined(Q_OS_MACOS)
#include <signal.h>
#endif

// Includes for Windows
#if defined(Q_OS_WIN)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

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

Skyscraper *x = nullptr;
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

#if defined(Q_OS_LINUX) || defined(Q_OS_MACOS)
void sigHandler(int signal)
#endif
#if defined(Q_OS_WIN)
BOOL WINAPI ConsoleHandler(DWORD dwType)
#endif
{
#if defined(Q_OS_LINUX) || defined(Q_OS_MACOS)
  if(signal == 2) {
    sigIntRequests++;
#endif
#if defined(Q_OS_WIN)
  if(dwType == CTRL_C_EVENT) {
    sigIntRequests++;
#endif
    if(sigIntRequests <= 2) {
      if(x != nullptr) {
	if(x->state == 0) {
	  // Nothing important going on, just exit
	  exit(1);
	} else if(x->state == 1) {
	  // Ignore signal, something important is going on that needs to finish!
	} else if(x->state == 2) {
	  // Cache being edited, clear the queue to quit nicely
	  x->queue->clearAll();
	} else if(x->state == 3) {
	  // Threads are running, clear queue for a nice exit
	  printf("\033[1;33mUser wants to quit, trying to exit nicely. This can take a few seconds depending on how many threads are running...\033[0m\n");
	  x->queue->clearAll();
	}
      } else {
	exit(1);
      }
    } else {
      printf("\033[1;31mUser REALLY wants to quit NOW, forcing unclean exit...\033[0m\n");
      exit(1);
    }
  }
#if defined(Q_OS_WIN)
  return TRUE;
#endif
}

int main(int argc, char *argv[])
{
#if defined(Q_OS_LINUX) || defined(Q_OS_MACOS)
  struct sigaction sigIntHandler;

  sigIntHandler.sa_handler = sigHandler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;

  sigaction(SIGINT, &sigIntHandler, NULL);
#endif

#if defined(Q_OS_WIN)
  SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler, TRUE);
#endif

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
  for(const auto &platform: Platform::getPlatforms()) {
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

  parser.setApplicationDescription(StrTools::getVersionHeader() + "Skyscraper looks for compatible game files for the chosen platform (set with '-p'). It allows you to gather and cache media and game information for the files using various scraping modules (set with '-s'). It then lets you generate game lists for the supported frontends by combining all previously cached resources ('game list generation mode' is initiated by simply leaving out the '-s' option). While doing so it also composites game art for all files by following the recipe at '/home/USER/.skyscraper/artwork.xml'.\n\nIn addition to the command line options Skyscraper also provides a lot of customizable options for configuration, artwork, game name aliases, resource priorities and much more. Please check the full documentation at 'https://github.com/muldjord/skyscraper/docs' for a detailed explanation of all features.\n\nRemember that most of the following options can also be set in the '/home/USER/.skyscraper/config.ini' file. All cli options and config.ini options are thoroughly documented in the above link.");
  parser.addHelpOption();
  parser.addVersionOption();
  QCommandLineOption pOption("p", "The platform you wish to scrape.\n(Currently supports " + platforms + ".)", "PLATFORM", "");
  QCommandLineOption fOption("f", "The frontend you wish to generate a gamelist for. Remember to leave out the '-s' option when using this in order to enable Skyscraper's gamelist generation mode.\n(Currently supports 'emulationstation', 'attractmode' and 'pegasus'. Default is 'emulationstation')", "FRONTEND", "");
  QCommandLineOption eOption("e", "Set extra frontend option. This is required by the 'attractmode' frontend to set the emulator and optionally for the 'pegasus' frontend to set the launch command.\n(Default is none)", "STRING", "");
  QCommandLineOption iOption("i", "Folder which contains the game/rom files.\n(default is '/home/USER/RetroPie/roms/PLATFORM')", "PATH", "");
  QCommandLineOption gOption("g", "Game list export folder.\n(default depends on frontend)", "PATH", "");
  QCommandLineOption oOption("o", "Game media export folder.\n(default depends on frontend)", "PATH", "");
  QCommandLineOption sOption("s", "The scraping module you wish to gather resources from for the platform set with '-p'.\nLeave the '-s' option out to enable Skyscraper's gamelist generation mode.\n(WEB: 'arcadedb', 'igdb', 'mobygames', 'openretro', 'screenscraper', 'thegamesdb' and 'worldofspectrum', LOCAL: 'esgamelist' and 'import')", "MODULE", "");
  QCommandLineOption uOption("u", "userKey or UserID and Password for use with the selected scraping module.\n(Default is none)", "KEY/USER:PASSWORD", "");
  QCommandLineOption mOption("m", "Minimum match percentage when comparing search result titles to filename titles.\n(default is 65)", "0-100", "");
  QCommandLineOption lOption("l", "Maximum game description length. Everything longer than this will be truncated.\n(default is 2500)", "0-10000", "");
  QCommandLineOption tOption("t", "Number of scraper threads to use. This might change depending on the scraping module limits.\n(default is 4)", "1-8", "");
  QCommandLineOption cOption("c", "Use this config file to set up Skyscraper.\n(default is '/home/USER/.skyscraper/config.ini')", "FILENAME", "");
  QCommandLineOption aOption("a", "Specify a non-default artwork.xml file to use when setting up the artwork compositing when in gamelist generation mode.\n(default is '/home/USER/.skyscraper/artwork.xml')", "FILENAME", "");
  QCommandLineOption dOption("d", "Set custom resource cache folder.\n(default is '/home/USER/.skyscraper/cache/PLATFORM')", "FOLDER", "");
  QCommandLineOption addextOption("addext", "Add this or these file extension(s) to accepted file extensions during a scraping run. (example: '*.zst' or '*.zst *.ext')", "EXTENSION(S)", "");
  QCommandLineOption flagsOption("flags", "Allows setting flags that will impact the run in various ways. See '--flags help' for a list of all available flags and what they do.", "FLAG1,FLAG2,...", "");
  QCommandLineOption cacheOption("cache", "This option is the master option for all options related to the resource cache. It must be followed by 'COMMAND[:OPTIONS]'.\nSee '--cache help' for a full description of all functions.", "COMMAND[:OPTIONS]", "");
  QCommandLineOption refreshOption("refresh", "Same as '--cache refresh'.");
  QCommandLineOption startatOption("startat", "Tells Skyscraper which file to start at. Forces '--refresh' and '--flags nosubdirs' enabled.", "FILENAME", "");
  QCommandLineOption endatOption("endat", "Tells Skyscraper which file to end at. Forces '--refresh' and '--flags nosubdirs' enabled.", "FILENAME", "");
  QCommandLineOption excludefilesOption("excludefiles", "Tells Skyscraper to always exclude the files matching the provided asterisk pattern(s). Remember to double-quote the pattern to avoid weird behaviour. You can add several patterns by separating them with ','. In cases where you need to match for a comma you need to escape it as '\\,'. (Pattern example: '\"*[BIOS]*,*proto*\"')", "PATTERN", "");
  QCommandLineOption includefilesOption("includefiles", "Tells Skyscraper to only include the files matching the provided asterisk pattern(s). Remember to double-quote the pattern to avoid weird behaviour. You can add several patterns by separating them with ','. In cases where you need to match for a comma you need to escape it as '\\,'. (Pattern example: '\"Super*,*Fighter*\"')", "PATTERN", "");
  QCommandLineOption fromfileOption("fromfile", "Tells Skyscraper to load the list of filenames to work on from a file. This file can be generated with the '--cache report:missing' option or made manually.", "FILENAME", "");
  QCommandLineOption maxfailsOption("maxfails", "Sets the allowed number of initial 'Not found' results before rage-quitting. (Default is 42)", "1-200", "");
  QCommandLineOption queryOption("query", "Allows you to set a custom search query (eg. 'rick+dangerous' for name based modules or 'sha1=CHECKSUM', 'md5=CHECKSUM' or 'romnom=FILENAME' for the 'screenscraper' module). Requires the single rom filename you wish to override for to be passed on command line as well, otherwise it will be ignored.", "QUERY", "");
  QCommandLineOption regionOption("region", "Add preferred game region for scraping modules that support it.\n(Default prioritization is 'eu', 'us', 'wor' and 'jp' + others in that order)", "CODE", "eu");
  QCommandLineOption langOption("lang", "Set preferred result language for scraping modules that support it.\n(Default 'en')", "CODE", "en");
  QCommandLineOption verbosityOption("verbosity", "Print more info while scraping\n(Default is 0.)", "0-3", "0");
  QCommandLineOption skippedOption("skipped", "\033[1;33mDeprecated!\033[0m Please see '--flags'.");
  QCommandLineOption nocoversOption("nocovers", "\033[1;33mDeprecated!\033[0m Please see '--flags'.");
  QCommandLineOption noscreenshotsOption("noscreenshots", "\033[1;33mDeprecated!\033[0m Please see '--flags'.");
  QCommandLineOption nowheelsOption("nowheels", "\033[1;33mDeprecated!\033[0m Please see '--flags'.");
  QCommandLineOption nomarqueesOption("nomarquees", "\033[1;33mDeprecated!\033[0m Please see '--flags'.");
  QCommandLineOption skipexistingcoversOption("skipexistingcovers", "\033[1;33mDeprecated!\033[0m Please see '--flags'.");
  QCommandLineOption skipexistingscreenshotsOption("skipexistingscreenshots", "\033[1;33mDeprecated!\033[0m Please see '--flags'.");
  QCommandLineOption skipexistingwheelsOption("skipexistingwheels", "\033[1;33mDeprecated!\033[0m Please see '--flags'.");
  QCommandLineOption skipexistingmarqueesOption("skipexistingmarquees", "\033[1;33mDeprecated!\033[0m Please see '--flags'.");
  QCommandLineOption skipexistingvideosOption("skipexistingvideos", "\033[1;33mDeprecated!\033[0m Please see '--flags'.");
  QCommandLineOption symlinkOption("symlink", "\033[1;33mDeprecated!\033[0m Please see '--flags'.");
  QCommandLineOption nobracketsOption("nobrackets", "\033[1;33mDeprecated!\033[0m Please see '--flags'.");
  QCommandLineOption relativeOption("relative", "\033[1;33mDeprecated!\033[0m Please see '--flags'.");
  QCommandLineOption noresizeOption("noresize", "\033[1;33mDeprecated!\033[0m Please see '--flags'.");
  QCommandLineOption nosubdirsOption("nosubdirs", "\033[1;33mDeprecated!\033[0m Please see '--flags'.");
  QCommandLineOption unattendOption("unattend", "\033[1;33mDeprecated!\033[0m Please see '--flags'.");
  QCommandLineOption unattendskipOption("unattendskip", "\033[1;33mDeprecated!\033[0m Please see '--flags'.");
  QCommandLineOption unpackOption("unpack", "\033[1;33mDeprecated!\033[0m Please see '--flags'.");
  QCommandLineOption forcefilenameOption("forcefilename", "\033[1;33mDeprecated!\033[0m Please see '--flags'.");
  QCommandLineOption onlymissingOption("onlymissing", "\033[1;33mDeprecated!\033[0m Please see '--flags'.");
  QCommandLineOption nohintsOption("nohints", "\033[1;33mDeprecated!\033[0m Please see '--flags'.");
  QCommandLineOption videosOption("videos", "\033[1;33mDeprecated!\033[0m Please see '--flags'."); 
  QCommandLineOption pretendOption("pretend", "\033[1;33mDeprecated!\033[0m Please see '--flags'.");
  QCommandLineOption interactiveOption("interactive", "\033[1;33mDeprecated!\033[0m Please see '--flags'.");

#if QT_VERSION >= 0x050800
  skippedOption.setFlags(QCommandLineOption::HiddenFromHelp);
  nocoversOption.setFlags(QCommandLineOption::HiddenFromHelp);
  noscreenshotsOption.setFlags(QCommandLineOption::HiddenFromHelp);
  nowheelsOption.setFlags(QCommandLineOption::HiddenFromHelp);
  nomarqueesOption.setFlags(QCommandLineOption::HiddenFromHelp);
  skipexistingcoversOption.setFlags(QCommandLineOption::HiddenFromHelp);
  skipexistingscreenshotsOption.setFlags(QCommandLineOption::HiddenFromHelp);
  skipexistingwheelsOption.setFlags(QCommandLineOption::HiddenFromHelp);
  skipexistingmarqueesOption.setFlags(QCommandLineOption::HiddenFromHelp);
  skipexistingvideosOption.setFlags(QCommandLineOption::HiddenFromHelp);
  symlinkOption.setFlags(QCommandLineOption::HiddenFromHelp);
  nobracketsOption.setFlags(QCommandLineOption::HiddenFromHelp);
  relativeOption.setFlags(QCommandLineOption::HiddenFromHelp);
  noresizeOption.setFlags(QCommandLineOption::HiddenFromHelp);
  nosubdirsOption.setFlags(QCommandLineOption::HiddenFromHelp);
  unattendOption.setFlags(QCommandLineOption::HiddenFromHelp);
  unattendskipOption.setFlags(QCommandLineOption::HiddenFromHelp);
  unpackOption.setFlags(QCommandLineOption::HiddenFromHelp);
  forcefilenameOption.setFlags(QCommandLineOption::HiddenFromHelp);
  onlymissingOption.setFlags(QCommandLineOption::HiddenFromHelp);
  nohintsOption.setFlags(QCommandLineOption::HiddenFromHelp);
  videosOption.setFlags(QCommandLineOption::HiddenFromHelp);
  pretendOption.setFlags(QCommandLineOption::HiddenFromHelp);
  interactiveOption.setFlags(QCommandLineOption::HiddenFromHelp);
#endif

  parser.addOption(pOption);
  parser.addOption(sOption);
  parser.addOption(uOption);
  parser.addOption(iOption);
  parser.addOption(gOption);
  parser.addOption(oOption);
  parser.addOption(fOption);
  parser.addOption(eOption);
  parser.addOption(tOption);
  parser.addOption(aOption);
  parser.addOption(mOption);
  parser.addOption(lOption);
  parser.addOption(cOption);
  parser.addOption(dOption);
  parser.addOption(flagsOption);
  parser.addOption(verbosityOption);
  parser.addOption(cacheOption);
  parser.addOption(refreshOption);
  parser.addOption(langOption);
  parser.addOption(regionOption);
  parser.addOption(queryOption);
  parser.addOption(startatOption);
  parser.addOption(endatOption);
  parser.addOption(includefilesOption);
  parser.addOption(excludefilesOption);
  parser.addOption(fromfileOption);
  parser.addOption(maxfailsOption);
  parser.addOption(addextOption);
  parser.addOption(forcefilenameOption);
  parser.addOption(interactiveOption);
  parser.addOption(nobracketsOption);
  parser.addOption(nocoversOption);
  parser.addOption(nohintsOption);
  parser.addOption(nomarqueesOption);
  parser.addOption(noresizeOption);
  parser.addOption(noscreenshotsOption);
  parser.addOption(nosubdirsOption);
  parser.addOption(nowheelsOption);
  parser.addOption(onlymissingOption);
  parser.addOption(pretendOption);
  parser.addOption(relativeOption);
  parser.addOption(skipexistingcoversOption);
  parser.addOption(skipexistingmarqueesOption);
  parser.addOption(skipexistingscreenshotsOption);
  parser.addOption(skipexistingvideosOption);
  parser.addOption(skipexistingwheelsOption);
  parser.addOption(skippedOption);
  parser.addOption(symlinkOption);
  parser.addOption(unattendOption);
  parser.addOption(unattendskipOption);
  parser.addOption(unpackOption);
  parser.addOption(videosOption);

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
