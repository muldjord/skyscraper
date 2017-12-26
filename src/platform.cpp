/***************************************************************************
 *            platform.cpp
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

#include "platform.h"

Platform::Platform(QString platform)
{
  this->platform = platform;
}

Platform::~Platform()
{
}

QStringList Platform::getScrapers()
{
  QStringList scrapers;
  if(platform == "amiga") {
    scrapers.append("openretro");
    scrapers.append("thegamesdb");
    scrapers.append("localdb");
  } else if(platform == "apple2") {
    scrapers.append("thegamesdb");
    scrapers.append("screenscraper");
    scrapers.append("localdb");
  } else if(platform == "arcade") {
    scrapers.append("arcadedb");
    scrapers.append("openretro");
    scrapers.append("thegamesdb");
    scrapers.append("screenscraper");
    scrapers.append("localdb");
  } else if(platform == "atari2600") {
    scrapers.append("openretro");
    scrapers.append("thegamesdb");
    scrapers.append("screenscraper");
    scrapers.append("localdb");
  } else if(platform == "atari5200") {
    scrapers.append("openretro");
    scrapers.append("thegamesdb");
    scrapers.append("screenscraper");
    scrapers.append("localdb");
  } else if(platform == "atari7800") {
    scrapers.append("openretro");
    scrapers.append("thegamesdb");
    scrapers.append("screenscraper");
    scrapers.append("localdb");
  } else if(platform == "atarijaguar") {
    scrapers.append("thegamesdb");
    scrapers.append("screenscraper");
    scrapers.append("localdb");
  } else if(platform == "atarilynx") {
    scrapers.append("openretro");
    scrapers.append("thegamesdb");
    scrapers.append("screenscraper");
    scrapers.append("localdb");
  } else if(platform == "atarist") {
    scrapers.append("openretro");
    scrapers.append("thegamesdb");
    scrapers.append("screenscraper");
    scrapers.append("localdb");
  } else if(platform == "c64") {
    scrapers.append("openretro");
    scrapers.append("thegamesdb");
    scrapers.append("screenscraper");
    scrapers.append("localdb");
  } else if(platform == "coleco") {
    scrapers.append("thegamesdb");
    scrapers.append("screenscraper");
    scrapers.append("localdb");
  } else if(platform == "gamegear") {
    scrapers.append("thegamesdb");
    scrapers.append("screenscraper");
    scrapers.append("localdb");
  } else if(platform == "gb") {
    scrapers.append("openretro");
    scrapers.append("thegamesdb");
    scrapers.append("screenscraper");
    scrapers.append("localdb");
  } else if(platform == "gba") {
    scrapers.append("openretro");
    scrapers.append("thegamesdb");
    scrapers.append("screenscraper");
    scrapers.append("localdb");
  } else if(platform == "gbc") {
    scrapers.append("openretro");
    scrapers.append("thegamesdb");
    scrapers.append("screenscraper");
    scrapers.append("localdb");
  } else if(platform == "genesis") {
    scrapers.append("openretro");
    scrapers.append("thegamesdb");
    scrapers.append("screenscraper");
    scrapers.append("localdb");
  } else if(platform == "mastersystem") {
    scrapers.append("openretro");
    scrapers.append("thegamesdb");
    scrapers.append("screenscraper");
    scrapers.append("localdb");
  } else if(platform == "megadrive") {
    scrapers.append("openretro");
    scrapers.append("thegamesdb");
    scrapers.append("screenscraper");
    scrapers.append("localdb");
  } else if(platform == "msx") {
    scrapers.append("openretro");
    scrapers.append("thegamesdb");
    scrapers.append("screenscraper");
    scrapers.append("localdb");
  } else if(platform == "n64") {
    scrapers.append("openretro");
    scrapers.append("thegamesdb");
    scrapers.append("screenscraper");
    scrapers.append("localdb");
  } else if(platform == "nds") {
    scrapers.append("thegamesdb");
    scrapers.append("screenscraper");
    scrapers.append("localdb");
  } else if(platform == "neogeo") {
    scrapers.append("arcadedb");
    scrapers.append("openretro");
    scrapers.append("thegamesdb");
    scrapers.append("screenscraper");
    scrapers.append("localdb");
  } else if(platform == "nes") {
    scrapers.append("openretro");
    scrapers.append("thegamesdb");
    scrapers.append("screenscraper");
    scrapers.append("localdb");
  } else if(platform == "ngpc") {
    scrapers.append("thegamesdb");
    scrapers.append("screenscraper");
    scrapers.append("localdb");
  } else if(platform == "pcengine") {
    scrapers.append("openretro");
    scrapers.append("thegamesdb");
    scrapers.append("screenscraper");
    scrapers.append("localdb");
  } else if(platform == "psp") {
    scrapers.append("thegamesdb");
    scrapers.append("screenscraper");
    scrapers.append("localdb");
  } else if(platform == "psx") {
    scrapers.append("openretro");
    scrapers.append("thegamesdb");
    scrapers.append("localdb");
  } else if(platform == "scummvm") {
    scrapers.append("openretro");
    scrapers.append("thegamesdb");
    scrapers.append("localdb");
  } else if(platform == "sega32x") {
    scrapers.append("thegamesdb");
    scrapers.append("screenscraper");
    scrapers.append("localdb");
  } else if(platform == "segacd") {
    scrapers.append("thegamesdb");
    scrapers.append("screenscraper");
    scrapers.append("localdb");
  } else if(platform == "snes") {
    scrapers.append("openretro");
    scrapers.append("thegamesdb");
    scrapers.append("screenscraper");
    scrapers.append("localdb");
  } else if(platform == "vectrex") {
    scrapers.append("thegamesdb");
    scrapers.append("screenscraper");
    scrapers.append("localdb");
  } else if(platform == "videopac") {
    scrapers.append("thegamesdb");
    scrapers.append("screenscraper");
    scrapers.append("localdb");
  } else if(platform == "virtualboy") {
    scrapers.append("thegamesdb");
    scrapers.append("screenscraper");
    scrapers.append("localdb");
  } else if(platform  == "zxspectrum") {
    scrapers.append("worldofspectrum");
    scrapers.append("openretro");
    scrapers.append("thegamesdb");
    scrapers.append("screenscraper");
    scrapers.append("localdb");
  }
  return scrapers;
}

QDir Platform::getInputDir(QString &inputFolder)
{
  QDir inputDir;
  if(platform == "amiga") {
    inputDir = QDir(inputFolder, "*.uae *.zip *.7z *.adf *.dms *.adz *.rp9", QDir::Name, QDir::Files);
  } else if(platform == "apple2") {
    inputDir = QDir(inputFolder, "*.dsk *.zip *.7z", QDir::Name, QDir::Files);
  } else if(platform == "arcade") {
    inputDir = QDir(inputFolder, "*.zip *.7z", QDir::Name, QDir::Files);
  } else if(platform == "atari2600") {
    inputDir = QDir(inputFolder, "*.bin *.a26 *.rom *.zip *.7z *.gz", QDir::Name, QDir::Files);
  } else if(platform == "atari5200") {
    inputDir = QDir(inputFolder, "*.a52 *.bas *.bin *.car *.xex *.atr *.xfd *.dcm *.atr.gz *.xfd.gz *.zip *.7z", QDir::Name, QDir::Files);
  } else if(platform == "atari7800") {
    inputDir = QDir(inputFolder, "*.a78 *.bin *.zip *.7z", QDir::Name, QDir::Files);
  } else if(platform == "atarijaguar") {
    inputDir = QDir(inputFolder, "*.j64 *.jag *.zip *.7z", QDir::Name, QDir::Files);
  } else if(platform == "atarilynx") {
    inputDir = QDir(inputFolder, "*.lnx *.zip *.7z", QDir::Name, QDir::Files);
  } else if(platform == "atarist") {
    inputDir = QDir(inputFolder, "*.st *.stx *.img *.rom *.raw *.ipf *.ctr *.zip *.7z", QDir::Name, QDir::Files);
  } else if(platform == "c64") {
    inputDir = QDir(inputFolder, "*.crt *.d64 *.prg *.zip *.7z *.tap *.t64 *.g64", QDir::Name, QDir::Files);
  } else if(platform == "coleco") {
    inputDir = QDir(inputFolder, "*.bin *.col *.rom *.zip *.7z", QDir::Name, QDir::Files);
  } else if(platform == "gb") {
    inputDir = QDir(inputFolder, "*.gb *.zip *.7z", QDir::Name, QDir::Files);
  } else if(platform == "gba") {
    inputDir = QDir(inputFolder, "*.gba *.zip *.7z", QDir::Name, QDir::Files);
  } else if(platform == "gbc") {
    inputDir = QDir(inputFolder, "*.gbc *.zip *.7z", QDir::Name, QDir::Files);
  } else if(platform == "gamegear") {
    inputDir = QDir(inputFolder, "*.gg *.zip *.7z", QDir::Name, QDir::Files);
  } else if(platform == "genesis") {
    inputDir = QDir(inputFolder, "*.smd *.bin *.gen *.md *.sg *.zip *.7z", QDir::Name, QDir::Files);
  } else if(platform == "mastersystem") {
    inputDir = QDir(inputFolder, "*.sms *.zip *.7z", QDir::Name, QDir::Files);
  } else if(platform == "megadrive") {
    inputDir = QDir(inputFolder, "*.smd *.bin *.gen *.md *.sg *.zip *.7z", QDir::Name, QDir::Files);
  } else if(platform == "msx") {
    inputDir = QDir(inputFolder, "*.rom *.mx1 *.mx2 *.col *.dsk *.zip *.7z", QDir::Name, QDir::Files);
  } else if(platform == "n64") {
    inputDir = QDir(inputFolder, "*.z64 *.n64 *.v64 *.zip *.7z", QDir::Name, QDir::Files);
  } else if(platform == "nds") {
    inputDir = QDir(inputFolder, "*.nds *.zip *.7z", QDir::Name, QDir::Files);
  } else if(platform == "neogeo") {
    inputDir = QDir(inputFolder, "*.zip *.7z", QDir::Name, QDir::Files);
  } else if(platform == "nes") {
    inputDir = QDir(inputFolder, "*.nes *.smc *.sfc *.fig *.swc *.mgd *.zip *.7z", QDir::Name, QDir::Files);
  } else if(platform == "ngpc") {
    inputDir = QDir(inputFolder, " *.ngc *.zip *.7z", QDir::Name, QDir::Files);
  } else if(platform == "pcengine") {
    inputDir = QDir(inputFolder, "*.pce *.zip *.7z", QDir::Name, QDir::Files);
  } else if(platform == "psp") {
    inputDir = QDir(inputFolder, "*.cso *.iso *.pbp *.zip *.7z", QDir::Name, QDir::Files);
  } else if(platform == "psx") {
    inputDir = QDir(inputFolder, "*.cue *.cbn *.img *.iso *.m3u *.mdf *.pbp *.toc *.z *.znx *.zip *.7z", QDir::Name, QDir::Files);
  } else if(platform == "scummvm") {
    inputDir = QDir(inputFolder, "*.svm", QDir::Name, QDir::Files);
  } else if(platform == "sega32x") {
    inputDir = QDir(inputFolder, "*.32x *.bin *.md *.smd *.zip *.7z", QDir::Name, QDir::Files);
  } else if(platform == "snes") {
    inputDir = QDir(inputFolder, "*.smc *.sfc *.fig *.swc *.zip *.7z", QDir::Name, QDir::Files);
  } else if(platform == "segacd") {
    inputDir = QDir(inputFolder, "*.cue *.iso *.zip *.7z", QDir::Name, QDir::Files);
  } else if(platform == "vectrex") {
    inputDir = QDir(inputFolder, "*.bin *.gam *.vec *.zip *.7z", QDir::Name, QDir::Files);
  } else if(platform == "videopac") {
    inputDir = QDir(inputFolder, "*.bin *.zip *.7z", QDir::Name, QDir::Files);
  } else if(platform == "virtualboy") {
    inputDir = QDir(inputFolder, "*.vb *.zip *.7z", QDir::Name, QDir::Files);
  } else if(platform == "zxspectrum") {
    inputDir = QDir(inputFolder, "*.sna *.szx *.z80 *.tap *.tzx *.gz *.udi *.mgt *.img *.trd *.scl *.dsk *.zip *.7z", QDir::Name, QDir::Files);
  }
  return inputDir;
}

QString Platform::getDefaultScraper()
{
  QString scraper;
  if(platform == "amiga") {
    scraper = "openretro";
  } else if(platform == "arcade") {
    scraper = "arcadedb";
  } else if(platform == "apple2") {
    scraper = "thegamesdb";
  } else if(platform == "atari2600") {
    scraper = "thegamesdb";
  } else if(platform == "atari5200") {
    scraper = "thegamesdb";
  } else if(platform == "atari7800") {
    scraper = "thegamesdb";
  } else if(platform == "atarijaguar") {
    scraper = "thegamesdb";
  } else if(platform == "atarilynx") {
    scraper = "thegamesdb";
  } else if(platform == "atarist") {
    scraper = "thegamesdb";
  } else if(platform == "c64") {
    scraper = "thegamesdb";
  } else if(platform == "coleco") {
    scraper = "thegamesdb";
  } else if(platform == "gamegear") {
    scraper = "thegamesdb";
  } else if(platform == "gb") {
    scraper = "thegamesdb";
  } else if(platform == "gba") {
    scraper = "thegamesdb";
  } else if(platform == "gbc") {
    scraper = "thegamesdb";
  } else if(platform == "genesis") {
    scraper = "thegamesdb";
  } else if(platform == "megadrive") {
    scraper = "thegamesdb";
  } else if(platform == "mastersystem") {
    scraper = "thegamesdb";
  } else if(platform == "msx") {
    scraper = "thegamesdb";
  } else if(platform == "n64") {
    scraper = "thegamesdb";
  } else if(platform == "nds") {
    scraper = "thegamesdb";
  } else if(platform == "neogeo") {
    scraper = "arcadedb";
  } else if(platform == "nes") {
    scraper = "thegamesdb";
  } else if(platform == "ngpc") {
    scraper = "thegamesdb";
  } else if(platform == "pcengine") {
    scraper = "thegamesdb";
  } else if(platform == "psp") {
    scraper = "thegamesdb";
  } else if(platform == "psx") {
    scraper = "thegamesdb";
  } else if(platform == "scummvm") {
    scraper = "thegamesdb";
  } else if(platform == "segacd") {
    scraper = "thegamesdb";
  } else if(platform == "sega32x") {
    scraper = "thegamesdb";
  } else if(platform == "snes") {
    scraper = "thegamesdb";
  } else if(platform == "vectrex") {
    scraper = "thegamesdb";
  } else if(platform == "videopac") {
    scraper = "thegamesdb";
  } else if(platform == "virtualboy") {
    scraper = "thegamesdb";
  } else if(platform == "zxspectrum") {
    scraper = "worldofspectrum";
  }
  return scraper;
}

QString Platform::getAliases()
{
  QString aliases;
  return aliases;
}

QStringList Platform::getPlatforms()
{
  QStringList platforms;
  platforms.append("amiga");
  platforms.append("apple2");
  platforms.append("arcade");
  platforms.append("atari2600");
  platforms.append("atari5200");
  platforms.append("atari7800");
  platforms.append("atarijaguar");
  platforms.append("atarilynx");
  platforms.append("atarist");
  platforms.append("c64");
  platforms.append("coleco");
  platforms.append("gamegear");
  platforms.append("gb");
  platforms.append("gba");
  platforms.append("gbc");
  platforms.append("genesis");
  platforms.append("mastersystem");
  platforms.append("megadrive");
  platforms.append("msx");
  platforms.append("n64");
  platforms.append("nds");
  platforms.append("neogeo");
  platforms.append("nes");
  platforms.append("ngpc");
  platforms.append("pcengine");
  platforms.append("psp");
  platforms.append("psx");
  platforms.append("scummvm");
  platforms.append("sega32x");
  platforms.append("segacd");
  platforms.append("snes");
  platforms.append("vectrex");
  platforms.append("videopac");
  platforms.append("virtualboy");
  platforms.append("zxspectrum");

  return platforms;
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
