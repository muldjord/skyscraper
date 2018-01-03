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

struct Settings {
  QString dbFolder = "";
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
  int doneThreads = 0;
  int threads = 4;
  int minMatch = 50;
  int notFound = 0;
  int found = 0;
  int currentFile = 0;
  int totalFiles = 0;
  int maxLength = 2500;
  bool videos = false;
  bool brackets = true;
  bool localDb = true;
  bool updateDb = false;
  bool checkDb = false;
  bool cleanDb = false;
  bool noResize = false;
  QString mergeDb = "";
  bool subDirs = true;
  bool pretend = false;
  bool unattend = false;
  bool forceFilename = false;
  bool stats = false;
  bool verbose = false;
  bool skipped = false;
  
  bool cacheCovers = true;
  bool cacheScreenshots = true;
  bool cacheWheels = true;
  bool cacheMarquees = true;

  QString user = "";
  QString password = "";
  QString lang = "en";
  QString region = "wor";

};

#endif // SETTINGS_H
