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

#include "scripter.h"

Scripter::Scripter()
{
  printf("\033[1;34m------------------------------------------\033[0m\n\033[1;33mRunning Skyscraper v" VERSION " by Lars Muldjord\033[0m\n\033[1;34m------------------------------------------\033[0m\n");

  std::string overwriteStr = "";
  printf("\033[1;31mWARNING!!!\033[0m You are running Skyscraper in scripting mode. This will overwrite your existing game list file. Any manual changes will be GONE! Do you still wish to continue (y/N)? ");
  getline(std::cin, overwriteStr);
  if(overwriteStr != "y") {
    exit(0);
  }

  std::string frontendStr = "";
  printf("\n");
  printf("\033[1;34mAvailable frontends:\033[0m\n");
  printf("* \033[1;33memulationstation\033[0m\n");
  printf("* \033[1;33mattractmode\033[0m\n");
  printf("Please enter the frontend you wish to scrape for (leave empty for 'emulationstation'): ");
  getline(std::cin, frontendStr);

  if(frontendStr == "")
    frontendStr = "emulationstation";
  
  std::string platformStr = "";
  printf("\n");
  printf("\033[1;34mAvailable platforms:\033[0m\n");
  printf("* \033[1;33mamiga\033[0m\n");
  printf("* \033[1;33mapple2\033[0m\n");
  printf("* \033[1;33marcade\033[0m\n");
  printf("* \033[1;33matari2600\033[0m\n");
  printf("* \033[1;33matari5200\033[0m\n");
  printf("* \033[1;33matari7800\033[0m\n");
  printf("* \033[1;33matarijaguar\033[0m\n");
  printf("* \033[1;33matarilynx\033[0m\n");
  printf("* \033[1;33matarist\033[0m\n");
  printf("* \033[1;33mc64\033[0m\n");
  printf("* \033[1;33mcoleco\033[0m\n");
  printf("* \033[1;33mgamegear\033[0m\n");
  printf("* \033[1;33mgb\033[0m\n");
  printf("* \033[1;33mgba\033[0m\n");
  printf("* \033[1;33mgbc\033[0m\n");
  printf("* \033[1;33mgenesis\033[0m\n");
  printf("* \033[1;33mmastersystem\033[0m\n");
  printf("* \033[1;33mmegadrive\033[0m\n");
  printf("* \033[1;33mmsx\033[0m\n");
  printf("* \033[1;33mn64\033[0m\n");
  printf("* \033[1;33mnds\033[0m\n");
  printf("* \033[1;33mneogeo\033[0m\n");
  printf("* \033[1;33mnes\033[0m\n");
  printf("* \033[1;33mngpc\033[0m\n");
  printf("* \033[1;33mpcengine\033[0m\n");
  printf("* \033[1;33mpsp\033[0m\n");
  printf("* \033[1;33mpsx\033[0m\n");
  printf("* \033[1;33mscummvm\033[0m\n");
  printf("* \033[1;33msega32x\033[0m\n");
  printf("* \033[1;33msegacd\033[0m\n");
  printf("* \033[1;33msnes\033[0m\n");
  printf("* \033[1;33mvectrex\033[0m\n");
  printf("* \033[1;33mvideopac\033[0m\n");
  printf("* \033[1;33mvirtualboy\033[0m\n");
  printf("* \033[1;33mzxspectrum\033[0m\n");
  while(platformStr == "") {
    printf("Please enter the platform you wish to scrape: ");
    getline(std::cin, platformStr);
  }
  
  std::string inputFolderStr = "";
  printf("Please enter game/rom input folder (enter for default): ");
  getline(std::cin, inputFolderStr);

  std::string gamelistFolderStr = "";
  printf("Please enter 'gamelist.xml' output folder (enter for default): ");
  getline(std::cin, gamelistFolderStr);

  std::string emulatorStr = "";
  while(frontendStr == "attractmode" && emulatorStr == "") {
    printf("Please enter AttractMode emulator: ");
    getline(std::cin, emulatorStr);
  }
  
  std::string artworkFolderStr = "";
  printf("Please enter artwork export folder (enter for default): ");
  getline(std::cin, artworkFolderStr);

  std::string videosStr = "n";
  printf("Do you wish to enable video scraping if the selected platform supports it (y/N)? ");
  getline(std::cin, videosStr);

  std::string videosFolderStr = "";
  if(videosStr == "y") {
    printf("Please enter video export folder (enter for default): ");
    getline(std::cin, videosFolderStr);
  }

  std::string bracketsStr = "y";
  printf("Do you wish to include bracket notes such as '[AGA]' and '(Psygnosis)' in the final game name (Y/n)? ");
  getline(std::cin, bracketsStr);

  std::string minMatchStr = "";
  printf("What is the minimum search result percentage match you wish to accept (enter for default)? ");
  getline(std::cin, minMatchStr);

  QFile scriptFile("skyscript.sh");
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
  if(frontendStr == "attractmode" && emulatorStr != "")
    commandStr += " -e " + emulatorStr;
  if(minMatchStr != "")
    commandStr += " -m " + minMatchStr;
  if(bracketsStr != "n")
    commandStr += " --nobrackets";
  if(videosStr != "y") {
    commandStr += " --videos";
    if(videosFolderStr != "")
      commandStr += " -v " + videosFolderStr;
  }

  commandStr += " --updatedb";
  
  scriptFile.write("#!/bin/bash\n");
  scriptFile.write(commandStr.c_str());
  scriptFile.close();
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
