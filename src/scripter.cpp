/***************************************************************************
 *            scripter.cpp
 *
 *  Sat Dec 23 10:00:00 CEST 2017
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
#include <QFile>
#include <QProcess>
#include <QDir>

#include "scripter.h"
#include "platform.h"
#include "strtools.h"

Scripter::Scripter()
{
  printf("%s", StrTools::getVersionHeader().toStdString().c_str());

#ifdef Q_OS_WIN
  printf("\033[1;31mYou are running Skyscraper in 'simple mode' on a Windows machine. Windows is not currently supported in this mode. The generated script WILL NOT WORK!\033[0m\n\n");
#endif

  printf("\033[1;33mIMPORTANT!!!\033[0m You are running Skyscraper in 'simple mode'. This mode is meant for first-time scrapings only! For any subsequent scrapings of a platform consider scraping using '\033[1;32mSkyscraper -p [platform]\033[0m' which will make use of the resources you already have available in the local database cache.\n\nFor advanced users be sure be sure to check out all of the available command line options with '\033[1;32mSkyscraper --help\033[0m'. Complete documentation of all features can be found at: \033[1;32mhttps://github.com/muldjord/skyscraper\033[0m\n\nYou will now be asked a bunch of questions. Default for most of these questions will be optimal and can therefore be answered simply by pressing enter. Only change them if you know what you are doing.\033[0m\n");

  std::string overwriteStr = "";
  printf("\n");
  printf("\033[1;31mWARNING!!!\033[0m Continuing will overwrite your existing game list file. Any manual changes will be GONE! If you wish to keep your existing game list file, please manually create a backup before re-running Skyscraper.\n\n\033[1;34mAre you sure you wish to proceed\033[0m (y/N)? ");
  getline(std::cin, overwriteStr);
  if(overwriteStr != "y") {
    printf("User chose not to continue, now exiting...\n");
    exit(0);
  }

  std::string frontendStr = "";
  printf("\n");
  printf("Available frontends:\n");
  printf("* \033[1;33memulationstation\033[0m\n");
  printf("* \033[1;33mattractmode\033[0m\n");
  printf("\033[1;34mPlease enter the frontend you wish to scrape for\033[0m (enter for 'emulationstation'):\033[0m ");
  getline(std::cin, frontendStr);

  if(frontendStr == "")
    frontendStr = "emulationstation";

  std::string emulatorStr = "";
  while(frontendStr == "attractmode" && emulatorStr == "") {
    printf("\033[1;34mPlease enter AttractMode emulator:\033[0m ");
    getline(std::cin, emulatorStr);
  }

  printf("User chose: '\033[1;32m%s\033[0m'\n", frontendStr.c_str());

  std::string platformStr = "";
  printf("\n");
  printf("Available platforms:\n");
  QStringList platforms = Platform::getPlatforms();
  foreach(QString platform, platforms) {
    printf("* \033[1;33m%s\033[0m\n", platform.toStdString().c_str());
  }
  while(platformStr == "") {
    printf("\033[1;34mPlease enter the platform you wish to scrape:\033[0m ");
    getline(std::cin, platformStr);
    if(!platforms.contains(QString(platformStr.c_str()))) {
      printf("\033[1;31mUnknown platform, please type in a platform from the list.\n\033[0m");
      platformStr = "";
    }
  }

  printf("User chose: '\033[1;32m%s\033[0m'\n", platformStr.c_str());
  
  std::string inputFolderStr = "";
  printf("\n");
  printf("\033[1;34mPlease enter full game/rom input folder\033[0m (enter for default): ");
  getline(std::cin, inputFolderStr);

  std::string gamelistFolderStr = "";
  printf("\033[1;34mPlease enter full game list output folder\033[0m (enter for default): ");
  getline(std::cin, gamelistFolderStr);

  std::string artworkFolderStr = "";
  if(frontendStr != "attractmode") {
    printf("\033[1;34mPlease enter full game media output folder\033[0m (enter for default): ");
    getline(std::cin, artworkFolderStr);
  }

  std::string videosStr = "";
  printf("\033[1;34mDo you wish to enable video scraping where supported\033[0m (y/N)? ");
  getline(std::cin, videosStr);

  std::string forceFilenameStr = "";
  printf("\033[1;34mDo you wish to use filenames for game name instead of the one provided by the scraping module\033[0m (y/N)? ");
  getline(std::cin, forceFilenameStr);

  std::string bracketsStr = "";
  printf("\033[1;34mDo you wish to include bracket notes such as '[AGA]' and '(Psygnosis)' in the final game name\033[0m (Y/n)? ");
  getline(std::cin, bracketsStr);

  std::string relativeStr = "";
  if(frontendStr == "emulationstation") {
    printf("\033[1;34mDo you wish to force rom relative paths in the exported gamelist.xml file?\033[0m (y/N)? ");
    getline(std::cin, relativeStr);
  }
  
  std::string minMatchStr = "";
  printf("\033[1;34mWhat is the minimum search result percentage match you wish to accept\033[0m (enter for default)? ");
  getline(std::cin, minMatchStr);

  std::string refreshStr = "";
  printf("\033[1;34mDo you wish to force a refresh of all locally cached data\033[0m (y/N)? ");
  getline(std::cin, refreshStr);

  std::string unpackStr = "";
  printf("\033[1;34mDo you wish to unpack zip and 7z roms before checksumming them (Answer 'y' here if you've zipped your roms manually)?\033[0m (y/N)? ");
  getline(std::cin, unpackStr);

  QFile scriptFile(QDir::homePath() + "/.skyscraper/skyscript.sh");
  if(!scriptFile.open(QIODevice::WriteOnly)) {
    printf("Couldn't open '~/.skyscraper/skyscript.sh' file for writing, please check permissions and rerun Skyscraper\nNow quitting...\n");
    exit(1);
  }

  std::string commandStr = "Skyscraper -p " + platformStr;
  if(inputFolderStr != "")
    commandStr += " -i " + inputFolderStr;
  if(gamelistFolderStr != "")
    commandStr += " -g " + gamelistFolderStr;
  if(artworkFolderStr != "")
    commandStr += " -o " + artworkFolderStr;
  if(frontendStr == "attractmode") {
    commandStr += " -f " + frontendStr;
    if(emulatorStr != "")
      commandStr += " -e " + emulatorStr;
  }
  if(minMatchStr != "")
    commandStr += " -m " + minMatchStr;
  if(forceFilenameStr == "y" || forceFilenameStr == "Y")
    commandStr += " --forcefilename";
  if(refreshStr == "y" || refreshStr == "Y")
    commandStr += " --refresh";
  if(unpackStr == "y" || unpackStr == "Y")
    commandStr += " --unpack";
  if(bracketsStr == "n")
    commandStr += " --nobrackets";
  if(relativeStr == "y" || relativeStr == "Y")
    commandStr += " --relative";
  if(videosStr == "y" || videosStr == "Y")
    commandStr += " --videos";

  commandStr += " --unattend";
  
  scriptFile.write("#!/bin/bash\n");
  foreach(QString scraper, Platform::getScrapers(QString(platformStr.c_str()))) {
    if(scraper == "localdb") {
      scriptFile.write((commandStr + " -s " + scraper.toStdString() + "\n").c_str());
    } else {
      scriptFile.write((commandStr + " -s " + scraper.toStdString() + " --pretend\n").c_str());
    }
  }
  scriptFile.close();
  
  std::string runScriptStr = "";
  printf("\n");
  printf("The script '\033[1;32m~/.skyscraper/skyscript.sh\033[0m' has been created. Running this script will do multiple scraping runs for the chosen platform for the most optimal result. \033[1;34mDo you wish to run it now?\033[0m (Y/n)? ");
  getline(std::cin, runScriptStr);
  if(runScriptStr == "y" || runScriptStr == "Y" || runScriptStr == "") {
    printf("\nRunning script...\n");
    QProcess::execute("sh " + QDir::homePath() + "/.skyscraper/skyscript.sh");
  } else {
    printf("\nUser chose not to run script, now exiting...\n");
    exit(0);
  }
}

Scripter::~Scripter()
{
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
