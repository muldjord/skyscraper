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

#include "skyscraper.h"
#include "scripter.h"
#include "platform.h"

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

int main(int argc, char *argv[])
{
  QCoreApplication app(argc, argv);

  // Set the working directory to the applications own path
  QDir skyDir(QDir::homePath() + "/.skyscraper");
  if(!skyDir.exists()) {
    if(!skyDir.mkpath(".")) {
      printf("Couldn't create folder '%s'. Please check permissions, now exiting...\n", skyDir.absolutePath().toStdString().c_str());
      exit(1);
    }
  }
  skyDir.mkpath("import");
  skyDir.mkpath("import/textual");
  skyDir.mkpath("import/snaps");
  skyDir.mkpath("import/boxart");
  skyDir.mkpath("import/videos");
  //skyDir.mkpath("import/marquees");
  skyDir.mkpath("dbs");
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

  parser.setApplicationDescription("\033[1;34m----------------------------------\033[0m\n\033[1;33mSkyscraper v" VERSION " by Lars Muldjord\033[0m\n\033[1;34m----------------------------------\033[0m\nThis scraper looks for compatible game files in the input directory. It fetches boxart, screenshots and other relevant information for the games based on their filenames, then builds a gamelist file for use with the chosen frontend.");
  parser.addHelpOption();
  QCommandLineOption pOption("p", "The platform you wish to scrape.\n(Currently supports " + platforms + ".)", "platform", "");
  QCommandLineOption fOption("f", "Frontend to scrape for.\n(Currently supports 'emulationstation' and 'attractmode'. Default is 'emulationstation')", "frontend", "");
  QCommandLineOption eOption("e", "Set emulator. This is only required by the 'attractmode' frontend.\n(Default is none)", "emulator", "");
  QCommandLineOption iOption("i", "Folder which contains the game files.\n(default is '/home/pi/RetroPie/roms/[platform]')", "path", "");
  QCommandLineOption gOption("g", "Gamelist export folder.\n(default depends on frontend)", "path", "");
  QCommandLineOption oOption("o", "Game image export folder.\n(default depends on frontend)", "path", "");
  QCommandLineOption vOption("v", "Game video export folder.\n(default depends on frontend)", "path", "");
  QCommandLineOption sOption("s", "Force a certain scraping module instead of the default one for the selected platform.\n(WEB: 'arcadedb', 'openretro', 'screenscraper', 'thegamesdb' and 'worldofspectrum', LOCAL: 'import' and 'localdb')", "scraper", "");
  QCommandLineOption uOption("u", "UserID and Password for use with the selected scraper module.\n(Default is none)", "user:password", "");
  QCommandLineOption mOption("m", "Minimum match percentage when comparing search result titles to filename titles.\n(default is 50)", "0-100", "");
  QCommandLineOption lOption("l", "Maximum game description length. Everything longer than this will be truncated.\n(default is 10000)", "0-10000", "");
  QCommandLineOption tOption("t", "Number of scraper threads to use.\n(default is 4)", "1-8", "");
  QCommandLineOption cOption("c", "Use this config file to set up the scraper.\n(default is '[homedir]/.skyscraper/config.ini')", "filename", "");
  QCommandLineOption dOption("d", "Set local resource database folder.\n(default is '[homedir]/.skyscraper/dbs/[platform]')", "folder", "");
  QCommandLineOption videosOption("videos", "Enables video scraping for any scraping module. Also enables caching of video resources in the local databases. Beware, this takes up a lot of disk space!");
  QCommandLineOption skippedOption("skipped", "Include skipped entries when writing final gamelist.");
  QCommandLineOption nobracketsOption("nobrackets", "Disables any [] and () tags in the frontend game titles.");
  QCommandLineOption nolocaldbOption("nolocaldb", "Disables local db resources. Other local db flags will then be ignored.");
  QCommandLineOption updatedbOption("updatedb", "Refresh all existing resources in local db using selected scraper. Set specific db folder with '-d'. Otherwise default db folder is used.");
  QCommandLineOption cleandbOption("cleandb", "Remove media files that have no entry in the db. Set specific db folder with '-d'. Otherwise default db folder is used.");
  QCommandLineOption mergedbOption("mergedb", "Merge data from a specific db folder into local destination db. Set db you wish to merge from with this flag. Set destination db folder with '-d'. Otherwise default destination db folder is used.", "folder", "");
  QCommandLineOption nosubdirsOption("nosubdirs", "Do not include input folder subdirectories when scraping.");
  QCommandLineOption pretendOption("pretend", "Don't alter any files (except 'skipped.txt'), just print the results on screen.");
  QCommandLineOption unattendOption("unattend", "Don't ask any questions when scraping. It will then always overwrite existing gamelist and not skip existing entries.");
  QCommandLineOption regionOption("region", "Set preferred game region for scraping modules that support it.\n(Default 'wor')", "code", "wor");
  QCommandLineOption langOption("lang", "Set preferred result language for scraping modules that support it.\n(Default 'en')", "code", "en");
  QCommandLineOption verboseOption("verbose", "Print more info while scraping.");
  
  parser.addOption(pOption);
  parser.addOption(iOption);
  parser.addOption(fOption);
  parser.addOption(eOption);
  parser.addOption(gOption);
  parser.addOption(oOption);
  parser.addOption(vOption);
  parser.addOption(sOption);
  parser.addOption(uOption);
  parser.addOption(mOption);
  parser.addOption(lOption);
  parser.addOption(tOption);
  parser.addOption(cOption);
  parser.addOption(dOption);
  parser.addOption(videosOption);
  parser.addOption(nobracketsOption);
  parser.addOption(skippedOption);
  parser.addOption(nolocaldbOption);
  parser.addOption(updatedbOption);
  //parser.addOption(checkdbOption);
  parser.addOption(cleandbOption);
  parser.addOption(mergedbOption);
  parser.addOption(nosubdirsOption);
  parser.addOption(pretendOption);
  parser.addOption(unattendOption);
  parser.addOption(langOption);
  parser.addOption(regionOption);
  parser.addOption(verboseOption);

  parser.process(app);

  if(argc > 1) {
    if(parser.isSet("help") || parser.isSet("h")) {
      parser.showHelp();
    } else {
      Skyscraper *x = new Skyscraper(parser);
      QObject::connect(x, &Skyscraper::finished, &app, &QCoreApplication::quit);
      QTimer::singleShot(0, x, SLOT(run()));
    }
    return app.exec();
  } else {
    Scripter scripter;
    return 0;
  }
}
