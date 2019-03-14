/***************************************************************************
 *            settings.h
 *
 *  Wed Jun 14 12:00:00 CEST 2017
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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QMap>

struct Settings {
  QString currentDir = "";

  QString cacheFolder = "";
  QString gameListFileString = "";
  QString skippedFileString = "";
  QString configFile = "";
  QString frontend = "emulationstation";
  QString emulator = "";
  QString platform = "";
  QString scraper = "";
  QString userCreds = "";
  QString inputFolder = "";
  QString gameListFolder = "";
  QString mediaFolder = "";
  QString screenshotsFolder = "";
  QString coversFolder = "";
  QString wheelsFolder = "";
  QString marqueesFolder = "";
  QString videosFolder = "";
  QString importFolder = "import";
  int doneThreads = 0;
  int threads = 4;
  bool threadsSet = false;
  int minMatch = 65;
  bool minMatchSet = false;
  int notFound = 0;
  int found = 0;
  int currentFile = 0;
  int totalFiles = 0;
  int maxLength = 2500;
  bool videos = false;
  bool symlink = false;
  bool brackets = true;
  bool refresh = false;
  QString cacheOptions = "";
  bool noResize = false;
  bool subdirs = true;
  QString startAt = "";
  QString endAt = "";
  bool pretend = false;
  bool unattend = false;
  bool unattendSkip = false;
  bool interactive = false;
  bool forceFilename = false;
  bool stats = false;
  bool nohints = false;
  int verbosity = 0;
  int maxFails = 42;
  bool skipped = false;
  QString artworkConfig = "artwork.xml";
  QByteArray artworkXml = "";
  bool relativePaths = false;
  QString extensions = "";
  QString addExtensions = "";
  bool unpack = false;
  bool preserveOldGameList = true;
  bool spaceCheck = true;
  bool memCheck = true;
  
  int romLimit = -1;

  bool cacheCovers = true;
  bool cacheScreenshots = true;
  bool cacheWheels = true;
  bool cacheMarquees = true;
  
  QString user = "";
  QString password = "";
  QString lang = "";
  QString region = "";
  QString langPriosStr = "";
  QString regionPriosStr = "";
  
  QString searchName = "";
  
  QMap<QString, QImage> resources;
  QMap<QString, QString> mameMap;
  QMap<QString, QString> aliasMap;
  QMap<QString, QPair<QString, QString> > whdLoadMap;

  QList<QString> regionPrios;
  QList<QString> langPrios;

};

#endif // SETTINGS_H
