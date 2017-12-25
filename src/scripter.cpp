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

Scripter::Scripter()
{
  printf("\033[1;34m------------------------------------------\033[0m\n\033[1;33mRunning Skyscraper v" VERSION " by Lars Muldjord\033[0m\n\033[1;34m------------------------------------------\033[0m\n");
  printf("\033[1;33mIMPORTANT!!!\033[0m \033[1;34mYou are running Skyscraper in 'simple mode'. This mode is meant for first-time scrapings only! For any subsequent scrapings of a platform consider scraping using '\033[0m\033[1;33mSkyscraper -p [platform] -s localdb\033[0m\033[1;34m'. This will not only save you time since it is A LOT faster, but also save bandwidth for the scraping sources since all of the data has already been cached locally and will be used by the 'localdb' scraping module.\n\nFor advanced users be sure be sure to check out all of the available command line options with '\033[0m\033[1;33mSkyscraper --help\033[0m\033[1;34m'. Complete documentation of all features can be found at: \033[0m\033[1;33mhttps://github.com/muldjord/skyscraper\033[0m\033[1;34m\n\nYou will now be asked a bunch of questions. Default for most of these questions will be optimal and can therefore be answered simply by pressing enter. Only change them if you know what you are doing.\033[0m\n");

  std::string overwriteStr = "";
  printf("\n");
  printf("\033[1;31mWARNING!!!\033[0m \033[1;34mContinuing will overwrite your existing game list file. Any manual changes will be GONE! If you wish to keep your existing game list file, please manually create a backup before re-running Skyscraper.\n\nAre you sure you wish to proceed\033[0m (y/N)? ");
  getline(std::cin, overwriteStr);
  if(overwriteStr != "y") {
    printf("\033[1;34mUser chose not to continue, now exiting...\033[0m\n");
    exit(0);
  }

  std::string frontendStr = "";
  printf("\n");
  printf("\033[1;34mAvailable frontends:\033[0m\n");
  printf("* \033[1;33memulationstation\033[0m\n");
  printf("* \033[1;33mattractmode\033[0m\n");
  printf("\033[1;34mPlease enter the frontend you wish to scrape for\033[0m (enter for 'emulationstation'):\033[0m ");
  getline(std::cin, frontendStr);

  if(frontendStr == "")
    frontendStr = "emulationstation";

  printf("User chose: '\033[1;32m%s\033[0m'\n", frontendStr.c_str());
  
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
    printf("\033[1;34mPlease enter the platform you wish to scrape:\033[0m ");
    getline(std::cin, platformStr);
  }

  printf("User chose: '\033[1;32m%s\033[0m'\n", platformStr.c_str());
  
  std::string inputFolderStr = "";
  printf("\n");
  printf("\033[1;34mPlease enter full game/rom input folder\033[0m (enter for default): ");
  getline(std::cin, inputFolderStr);

  std::string gamelistFolderStr = "";
  printf("\033[1;34mPlease enter full 'gamelist.xml' output folder\033[0m (enter for default): ");
  getline(std::cin, gamelistFolderStr);

  std::string emulatorStr = "";
  while(frontendStr == "attractmode" && emulatorStr == "") {
    printf("\033[1;34mPlease enter AttractMode emulator: ");
    getline(std::cin, emulatorStr);
  }
  
  std::string artworkFolderStr = "";
  printf("\033[1;34mPlease enter full artwork export folder\033[0m (enter for default): ");
  getline(std::cin, artworkFolderStr);

  std::string videosStr = "";
  printf("\033[1;34mDo you wish to enable video scraping if the selected platform supports it\033[0m (y/N)? ");
  getline(std::cin, videosStr);

  std::string videosFolderStr = "";
  if(videosStr == "y") {
    printf("\033[1;34mPlease enter full video export folder\033[0m (enter for default): ");
    getline(std::cin, videosFolderStr);
  }

  std::string bracketsStr = "";
  printf("\033[1;34mDo you wish to include bracket notes such as '[AGA]' and '(Psygnosis)' in the final game name\033[0m (Y/n)? ");
  getline(std::cin, bracketsStr);

  std::string minMatchStr = "";
  printf("\033[1;34mWhat is the minimum search result percentage match you wish to accept\033[0m (enter for default)? ");
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
  if(bracketsStr != "")
    commandStr += " --nobrackets";
  if(videosStr == "y") {
    commandStr += " --videos";
    if(videosFolderStr != "")
      commandStr += " -v " + videosFolderStr;
  }

  commandStr += " --unattend";
  
  scriptFile.write("#!/bin/bash\n");
  if(platformStr == "amiga") {
    scriptFile.write((commandStr + " -s openretro\n").c_str());
    scriptFile.write((commandStr + " -s thegamesdb\n").c_str());
    scriptFile.write((commandStr + " -s localdb\n").c_str());
  } else if(platformStr == "apple2") {
    scriptFile.write((commandStr + " -s thegamesdb\n").c_str());
    scriptFile.write((commandStr + " -s screenscraper\n").c_str());
    scriptFile.write((commandStr + " -s localdb\n").c_str());
  } else if(platformStr == "arcade") {
    scriptFile.write((commandStr + " -s arcadedb\n").c_str());
    scriptFile.write((commandStr + " -s openretro\n").c_str());
    scriptFile.write((commandStr + " -s thegamesdb\n").c_str());
    scriptFile.write((commandStr + " -s screenscraper\n").c_str());
    scriptFile.write((commandStr + " -s localdb\n").c_str());
  } else if(platformStr == "atari2600") {
    scriptFile.write((commandStr + " -s openretro\n").c_str());
    scriptFile.write((commandStr + " -s thegamesdb\n").c_str());
    scriptFile.write((commandStr + " -s screenscraper\n").c_str());
    scriptFile.write((commandStr + " -s localdb\n").c_str());
  } else if(platformStr == "atari5200") {
    scriptFile.write((commandStr + " -s openretro\n").c_str());
    scriptFile.write((commandStr + " -s thegamesdb\n").c_str());
    scriptFile.write((commandStr + " -s screenscraper\n").c_str());
    scriptFile.write((commandStr + " -s localdb\n").c_str());
  } else if(platformStr == "atari7800") {
    scriptFile.write((commandStr + " -s openretro\n").c_str());
    scriptFile.write((commandStr + " -s thegamesdb\n").c_str());
    scriptFile.write((commandStr + " -s screenscraper\n").c_str());
    scriptFile.write((commandStr + " -s localdb\n").c_str());
  } else if(platformStr == "atarijaguar") {
    scriptFile.write((commandStr + " -s thegamesdb\n").c_str());
    scriptFile.write((commandStr + " -s screenscraper\n").c_str());
    scriptFile.write((commandStr + " -s localdb\n").c_str());
  } else if(platformStr == "atarilynx") {
    scriptFile.write((commandStr + " -s openretro\n").c_str());
    scriptFile.write((commandStr + " -s thegamesdb\n").c_str());
    scriptFile.write((commandStr + " -s screenscraper\n").c_str());
    scriptFile.write((commandStr + " -s localdb\n").c_str());
  } else if(platformStr == "atarist") {
    scriptFile.write((commandStr + " -s openretro\n").c_str());
    scriptFile.write((commandStr + " -s thegamesdb\n").c_str());
    scriptFile.write((commandStr + " -s screenscraper\n").c_str());
    scriptFile.write((commandStr + " -s localdb\n").c_str());
  } else if(platformStr == "c64") {
    scriptFile.write((commandStr + " -s openretro\n").c_str());
    scriptFile.write((commandStr + " -s thegamesdb\n").c_str());
    scriptFile.write((commandStr + " -s screenscraper\n").c_str());
    scriptFile.write((commandStr + " -s localdb\n").c_str());
  } else if(platformStr == "coleco") {
    scriptFile.write((commandStr + " -s thegamesdb\n").c_str());
    scriptFile.write((commandStr + " -s screenscraper\n").c_str());
    scriptFile.write((commandStr + " -s localdb\n").c_str());
  } else if(platformStr == "gamegear") {
    scriptFile.write((commandStr + " -s thegamesdb\n").c_str());
    scriptFile.write((commandStr + " -s screenscraper\n").c_str());
    scriptFile.write((commandStr + " -s localdb\n").c_str());
  } else if(platformStr == "gb") {
    scriptFile.write((commandStr + " -s openretro\n").c_str());
    scriptFile.write((commandStr + " -s thegamesdb\n").c_str());
    scriptFile.write((commandStr + " -s screenscraper\n").c_str());
    scriptFile.write((commandStr + " -s localdb\n").c_str());
  } else if(platformStr == "gba") {
    scriptFile.write((commandStr + " -s openretro\n").c_str());
    scriptFile.write((commandStr + " -s thegamesdb\n").c_str());
    scriptFile.write((commandStr + " -s screenscraper\n").c_str());
    scriptFile.write((commandStr + " -s localdb\n").c_str());
  } else if(platformStr == "gbc") {
    scriptFile.write((commandStr + " -s openretro\n").c_str());
    scriptFile.write((commandStr + " -s thegamesdb\n").c_str());
    scriptFile.write((commandStr + " -s screenscraper\n").c_str());
    scriptFile.write((commandStr + " -s localdb\n").c_str());
  } else if(platformStr == "genesis") {
    scriptFile.write((commandStr + " -s openretro\n").c_str());
    scriptFile.write((commandStr + " -s thegamesdb\n").c_str());
    scriptFile.write((commandStr + " -s screenscraper\n").c_str());
    scriptFile.write((commandStr + " -s localdb\n").c_str());
  } else if(platformStr == "mastersystem") {
    scriptFile.write((commandStr + " -s openretro\n").c_str());
    scriptFile.write((commandStr + " -s thegamesdb\n").c_str());
    scriptFile.write((commandStr + " -s screenscraper\n").c_str());
    scriptFile.write((commandStr + " -s localdb\n").c_str());
  } else if(platformStr == "megadrive") {
    scriptFile.write((commandStr + " -s openretro\n").c_str());
    scriptFile.write((commandStr + " -s thegamesdb\n").c_str());
    scriptFile.write((commandStr + " -s screenscraper\n").c_str());
    scriptFile.write((commandStr + " -s localdb\n").c_str());
  } else if(platformStr == "msx") {
    scriptFile.write((commandStr + " -s openretro\n").c_str());
    scriptFile.write((commandStr + " -s thegamesdb\n").c_str());
    scriptFile.write((commandStr + " -s screenscraper\n").c_str());
    scriptFile.write((commandStr + " -s localdb\n").c_str());
  } else if(platformStr == "n64") {
    scriptFile.write((commandStr + " -s openretro\n").c_str());
    scriptFile.write((commandStr + " -s thegamesdb\n").c_str());
    scriptFile.write((commandStr + " -s screenscraper\n").c_str());
    scriptFile.write((commandStr + " -s localdb\n").c_str());
  } else if(platformStr == "nds") {
    scriptFile.write((commandStr + " -s thegamesdb\n").c_str());
    scriptFile.write((commandStr + " -s screenscraper\n").c_str());
    scriptFile.write((commandStr + " -s localdb\n").c_str());
  } else if(platformStr == "neogeo") {
    scriptFile.write((commandStr + " -s arcadedb\n").c_str());
    scriptFile.write((commandStr + " -s openretro\n").c_str());
    scriptFile.write((commandStr + " -s thegamesdb\n").c_str());
    scriptFile.write((commandStr + " -s screenscraper\n").c_str());
    scriptFile.write((commandStr + " -s localdb\n").c_str());
  } else if(platformStr == "nes") {
    scriptFile.write((commandStr + " -s openretro\n").c_str());
    scriptFile.write((commandStr + " -s thegamesdb\n").c_str());
    scriptFile.write((commandStr + " -s screenscraper\n").c_str());
    scriptFile.write((commandStr + " -s localdb\n").c_str());
  } else if(platformStr == "ngpc") {
    scriptFile.write((commandStr + " -s thegamesdb\n").c_str());
    scriptFile.write((commandStr + " -s screenscraper\n").c_str());
    scriptFile.write((commandStr + " -s localdb\n").c_str());
  } else if(platformStr == "pcengine") {
    scriptFile.write((commandStr + " -s openretro\n").c_str());
    scriptFile.write((commandStr + " -s thegamesdb\n").c_str());
    scriptFile.write((commandStr + " -s screenscraper\n").c_str());
    scriptFile.write((commandStr + " -s localdb\n").c_str());
  } else if(platformStr == "psp") {
    scriptFile.write((commandStr + " -s thegamesdb\n").c_str());
    scriptFile.write((commandStr + " -s screenscraper\n").c_str());
    scriptFile.write((commandStr + " -s localdb\n").c_str());
  } else if(platformStr == "psx") {
    scriptFile.write((commandStr + " -s openretro\n").c_str());
    scriptFile.write((commandStr + " -s thegamesdb\n").c_str());
    scriptFile.write((commandStr + " -s localdb\n").c_str());
  } else if(platformStr == "scummvm") {
    scriptFile.write((commandStr + " -s openretro\n").c_str());
    scriptFile.write((commandStr + " -s thegamesdb\n").c_str());
    scriptFile.write((commandStr + " -s localdb\n").c_str());
  } else if(platformStr == "sega32x") {
    scriptFile.write((commandStr + " -s thegamesdb\n").c_str());
    scriptFile.write((commandStr + " -s screenscraper\n").c_str());
    scriptFile.write((commandStr + " -s localdb\n").c_str());
  } else if(platformStr == "segacd") {
    scriptFile.write((commandStr + " -s thegamesdb\n").c_str());
    scriptFile.write((commandStr + " -s screenscraper\n").c_str());
    scriptFile.write((commandStr + " -s localdb\n").c_str());
  } else if(platformStr == "snes") {
    scriptFile.write((commandStr + " -s openretro\n").c_str());
    scriptFile.write((commandStr + " -s thegamesdb\n").c_str());
    scriptFile.write((commandStr + " -s screenscraper\n").c_str());
    scriptFile.write((commandStr + " -s localdb\n").c_str());
  } else if(platformStr == "vectrex") {
    scriptFile.write((commandStr + " -s thegamesdb\n").c_str());
    scriptFile.write((commandStr + " -s screenscraper\n").c_str());
    scriptFile.write((commandStr + " -s localdb\n").c_str());
  } else if(platformStr == "videopac") {
    scriptFile.write((commandStr + " -s thegamesdb\n").c_str());
    scriptFile.write((commandStr + " -s screenscraper\n").c_str());
    scriptFile.write((commandStr + " -s localdb\n").c_str());
  } else if(platformStr == "virtualboy") {
    scriptFile.write((commandStr + " -s thegamesdb\n").c_str());
    scriptFile.write((commandStr + " -s screenscraper\n").c_str());
    scriptFile.write((commandStr + " -s localdb\n").c_str());
  } else if(platformStr == "zxspectrum") {
    scriptFile.write((commandStr + " -s worldofspectrum\n").c_str());
    scriptFile.write((commandStr + " -s openretro\n").c_str());
    scriptFile.write((commandStr + " -s thegamesdb\n").c_str());
    scriptFile.write((commandStr + " -s screenscraper\n").c_str());
    scriptFile.write((commandStr + " -s localdb\n").c_str());
  }
  scriptFile.close();
  
  std::string runScriptStr = "";
  printf("\n");
  printf("\033[1;34mThe script '\033[0m\033[1;33m~/.skyscraper/skyscript.sh\033[0m\033[1;34m' has been created. Running this script will do multiple scraping runs for the chosen platforms for the most optimal result. Do you wish to run it now?\033[0m (y/n)? ");
  getline(std::cin, runScriptStr);
  if(overwriteStr != "y") {
    printf("\033[1;34mUser chose not to run script, now exiting...\033[0m\n");
    exit(0);
  }
  printf("\n");
  printf("\033[1;34mRunning script...\033[0m\n");
  QProcess::execute("sh " + QDir::homePath() + "/.skyscraper/skyscript.sh");
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
