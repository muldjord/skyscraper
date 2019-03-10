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

Platform::Platform()
{
}

Platform::~Platform()
{
}

QStringList Platform::getPlatforms()
{
  QStringList platforms;
  platforms.append("3do");
  platforms.append("amiga");
  platforms.append("amstradcpc");
  platforms.append("apple2");
  platforms.append("arcade");
  platforms.append("atari800");
  platforms.append("atari2600");
  platforms.append("atari5200");
  platforms.append("atari7800");
  platforms.append("atarijaguar");
  platforms.append("atarilynx");
  platforms.append("atarist");
  platforms.append("c16");
  platforms.append("c64");
  platforms.append("c128");
  platforms.append("coco");
  platforms.append("coleco");
  platforms.append("daphne");
  platforms.append("dragon32");
  platforms.append("dreamcast");
  platforms.append("fba");
  platforms.append("fds");
  platforms.append("gameandwatch");
  platforms.append("gamegear");
  platforms.append("gb");
  platforms.append("gba");
  platforms.append("gbc");
  platforms.append("gc");
  platforms.append("genesis");
  platforms.append("intellivision");
  platforms.append("mame-advmame");
  platforms.append("mame-libretro");
  platforms.append("mame-mame4all");
  platforms.append("mastersystem");
  platforms.append("megacd");
  platforms.append("megadrive");
  platforms.append("msx");
  platforms.append("n64");
  platforms.append("nds");
  platforms.append("neogeo");
  platforms.append("nes");
  platforms.append("ngp");
  platforms.append("ngpc");
  platforms.append("oric");
  platforms.append("pc");
  platforms.append("pc88");
  platforms.append("pcfx");
  platforms.append("pcengine");
  platforms.append("ports");
  platforms.append("ps2");
  platforms.append("psp");
  platforms.append("psx");
  platforms.append("saturn");
  platforms.append("scummvm");
  platforms.append("sega32x");
  platforms.append("segacd");
  platforms.append("sg-1000");
  platforms.append("snes");
  platforms.append("steam");
  platforms.append("ti99");
  platforms.append("trs-80");
  platforms.append("vectrex");
  platforms.append("vic20");
  platforms.append("videopac");
  platforms.append("virtualboy");
  platforms.append("wii");
  platforms.append("wonderswan");
  platforms.append("wonderswancolor");
  platforms.append("x68000");
  platforms.append("x1");
  platforms.append("zmachine");
  platforms.append("zx81");
  platforms.append("zxspectrum");

  return platforms;
}

QStringList Platform::getScrapers(QString platform)
{
  QStringList scrapers;
  if(platform == "3do") {
    scrapers.append("screenscraper");
  } else if(platform == "amiga") {
    scrapers.append("openretro");
    scrapers.append("screenscraper");
  } else if(platform == "amstradcpc") {
    scrapers.append("screenscraper");
  } else if(platform == "apple2") {
    scrapers.append("screenscraper");
  } else if(platform == "arcade") {
    scrapers.append("arcadedb");
    scrapers.append("screenscraper");
  } else if(platform == "atari800") {
    scrapers.append("screenscraper");
  } else if(platform == "atari2600") {
    scrapers.append("screenscraper");
  } else if(platform == "atari5200") {
    scrapers.append("screenscraper");
  } else if(platform == "atari7800") {
    scrapers.append("screenscraper");
  } else if(platform == "atarijaguar") {
    scrapers.append("screenscraper");
  } else if(platform == "atarilynx") {
    scrapers.append("screenscraper");
  } else if(platform == "atarist") {
    scrapers.append("screenscraper");
  } else if(platform == "c16") {
    scrapers.append("screenscraper");
  } else if(platform == "c64") {
    scrapers.append("screenscraper");
  } else if(platform == "c128") {
    scrapers.append("screenscraper");
  } else if(platform == "coco") {
    scrapers.append("screenscraper");
  } else if(platform == "coleco") {
    scrapers.append("screenscraper");
  } else if(platform == "daphne") {
    scrapers.append("screenscraper");
  } else if(platform == "dragon32") {
    scrapers.append("screenscraper");
  } else if(platform == "dreamcast") {
    scrapers.append("screenscraper");
  } else if(platform == "fba") {
    scrapers.append("arcadedb");
    scrapers.append("screenscraper");
  } else if(platform == "fds") {
    scrapers.append("screenscraper");
  } else if(platform == "gameandwatch") {
    scrapers.append("screenscraper");
  } else if(platform == "gamegear") {
    scrapers.append("screenscraper");
  } else if(platform == "gb") {
    scrapers.append("screenscraper");
  } else if(platform == "gba") {
    scrapers.append("screenscraper");
  } else if(platform == "gbc") {
    scrapers.append("screenscraper");
  } else if(platform == "gc") {
    scrapers.append("screenscraper");
  } else if(platform == "genesis") {
    scrapers.append("screenscraper");
  } else if(platform == "intellivision") {
    scrapers.append("screenscraper");
  } else if(platform == "mame-advmame") {
    scrapers.append("arcadedb");
    scrapers.append("screenscraper");
  } else if(platform == "mame-libretro") {
    scrapers.append("arcadedb");
    scrapers.append("screenscraper");
  } else if(platform == "mame-mame4all") {
    scrapers.append("arcadedb");
    scrapers.append("screenscraper");
  } else if(platform == "mastersystem") {
    scrapers.append("screenscraper");
  } else if(platform == "megacd") {
    scrapers.append("screenscraper");
  } else if(platform == "megadrive") {
    scrapers.append("screenscraper");
  } else if(platform == "msx") {
    scrapers.append("screenscraper");
  } else if(platform == "n64") {
    scrapers.append("screenscraper");
  } else if(platform == "nds") {
    scrapers.append("screenscraper");
  } else if(platform == "neogeo") {
    scrapers.append("arcadedb");
    scrapers.append("screenscraper");
  } else if(platform == "nes") {
    scrapers.append("screenscraper");
  } else if(platform == "ngp") {
    scrapers.append("screenscraper");
  } else if(platform == "ngpc") {
    scrapers.append("screenscraper");
  } else if(platform == "oric") {
    scrapers.append("screenscraper");
  } else if(platform == "pc") {
    scrapers.append("thegamesdb");
  } else if(platform == "pc88") {
    scrapers.append("thegamesdb");
  } else if(platform == "pcfx") {
    scrapers.append("screenscraper");
  } else if(platform == "pcengine") {
    scrapers.append("screenscraper");
  } else if(platform == "ports") {
    scrapers.append("thegamesdb");
  } else if(platform == "ps2") {
    scrapers.append("screenscraper");
    scrapers.append("thegamesdb");
  } else if(platform == "psp") {
    scrapers.append("screenscraper");
  } else if(platform == "psx") {
    scrapers.append("screenscraper");
    scrapers.append("thegamesdb");
  } else if(platform == "saturn") {
    scrapers.append("screenscraper");
  } else if(platform == "scummvm") {
    scrapers.append("screenscraper");
    scrapers.append("thegamesdb");
  } else if(platform == "sega32x") {
    scrapers.append("screenscraper");
  } else if(platform == "segacd") {
    scrapers.append("screenscraper");
  } else if(platform == "sg-1000") {
    scrapers.append("screenscraper");
  } else if(platform == "snes") {
    scrapers.append("screenscraper");
  } else if(platform == "steam") {
    scrapers.append("screenscraper");
  } else if(platform == "ti99") {
    scrapers.append("screenscraper");
  } else if(platform == "trs-80") {
    scrapers.append("screenscraper");
  } else if(platform == "vectrex") {
    scrapers.append("screenscraper");
  } else if(platform == "vic20") {
    scrapers.append("screenscraper");
  } else if(platform == "videopac") {
    scrapers.append("screenscraper");
  } else if(platform == "virtualboy") {
    scrapers.append("screenscraper");
  } else if(platform == "wii") {
    scrapers.append("screenscraper");
  } else if(platform == "wonderswan") {
    scrapers.append("screenscraper");
  } else if(platform == "wonderswancolor") {
    scrapers.append("screenscraper");
  } else if(platform  == "x68000") {
    scrapers.append("screenscraper");
  } else if(platform  == "x1") {
    scrapers.append("screenscraper");
  } else if(platform  == "zmachine") {
    scrapers.append("thegamesdb");
  } else if(platform  == "zx81") {
    scrapers.append("screenscraper");
  } else if(platform  == "zxspectrum") {
    scrapers.append("worldofspectrum");
    scrapers.append("screenscraper");
  }

  // Always add 'cache' as the last one
  scrapers.append("cache");
  
  return scrapers;
}

QString Platform::getFormats(QString platform, QString extensions, QString addExtensions)
{
  if(!extensions.isEmpty() && extensions.contains("*.")) {
    return extensions;
  }

  QString formats = "*.zip *.7z *.ml ";
  if(!addExtensions.isEmpty() && addExtensions.contains("*.")) {
    formats.append(addExtensions);
  }
  if(formats.right(1) != " ") {
    formats.append(" ");
  }
  if(platform == "3do") {
    formats.append("*.iso");
  } else if(platform == "amiga") {
    formats.append("*.uae *.adf *.dms *.adz *.rp9 *.lha *.cue *.img *.iso");
  } else if(platform == "amstradcpc") {
    formats.append("*.dsk *.cpc");
  } else if(platform == "apple2") {
    formats.append("*.dsk");
  } else if(platform == "arcade") {
    formats.append("");
  } else if(platform == "atari800") {
    formats.append("*.a52 *.bas *.bin *.car *.xex *.atr *.xfd *.dcm *.atr.gz *.xfd.gz");
  } else if(platform == "atari2600") {
    formats.append("*.bin *.a26 *.rom *.gz");
  } else if(platform == "atari5200") {
    formats.append("*.a52 *.bas *.bin *.car *.xex *.atr *.xfd *.dcm *.atr.gz *.xfd.gz");
  } else if(platform == "atari7800") {
    formats.append("*.a78 *.bin");
  } else if(platform == "atarijaguar") {
    formats.append("*.j64 *.jag");
  } else if(platform == "atarilynx") {
    formats.append("*.lnx");
  } else if(platform == "atarist") {
    formats.append("*.st *.stx *.img *.rom *.raw *.ipf *.ctr");
  } else if(platform == "c16") {
    formats.append("*.crt *.d64 *.prg *.tap *.t64 *.g64 *.x64 *.vsf");
  } else if(platform == "c64") {
    formats.append("*.crt *.d64 *.prg *.tap *.t64 *.g64 *.x64 *.vsf");
  } else if(platform == "c128") {
    formats.append("*.crt *.d64 *.prg *.tap *.t64 *.g64 *.x64 *.vsf");
  } else if(platform == "coco") {
    formats.append("*.cas *.wav *.bas *.asc *.dmk *.jvc *.os9 *.dsk *.vdk *.rom *.ccc *.sna");
  } else if(platform == "coleco") {
    formats.append("*.bin *.col *.rom");
  } else if(platform == "daphne") {
    formats.append("*.daphne");
  } else if(platform == "dragon32") {
    formats.append("*.cas *.wav *.bas *.asc *.dmk *.jvc *.os9 *.dsk *.vdk *.rom *.ccc *.sna");
  } else if(platform == "dreamcast") {
    formats.append("*.cdi *.gdi *.iso *.chd *.cue");
  } else if(platform == "fba") {
    formats.append("");
  } else if(platform == "fds") {
    formats.append("*.fds *.nes");
  } else if(platform == "gameandwatch") {
    formats.append("*.mgw");
  } else if(platform == "gamegear") {
    formats.append("*.gg");
  } else if(platform == "gb") {
    formats.append("*.gb");
  } else if(platform == "gba") {
    formats.append("*.gba");
  } else if(platform == "gbc") {
    formats.append("*.gbc");
  } else if(platform == "gc") {
    formats.append("*.iso *.cso *.gcz");
  } else if(platform == "genesis") {
    formats.append("*.smd *.bin *.gen *.md *.sg");
  } else if(platform == "intellivision") {
    formats.append("*.int *.bin");
  } else if(platform == "mame-advmame") {
    formats.append("");
  } else if(platform == "mame-libretro") {
    formats.append("");
  } else if(platform == "mame-mame4all") {
    formats.append("");
  } else if(platform == "mastersystem") {
    formats.append("*.sms");
  } else if(platform == "megacd") {
    formats.append("*.cue *.iso *.chd");
  } else if(platform == "megadrive") {
    formats.append("*.smd *.bin *.gen *.md *.sg");
  } else if(platform == "msx") {
    formats.append("*.rom *.mx1 *.mx2 *.col *.dsk");
  } else if(platform == "n64") {
    formats.append("*.z64 *.n64 *.v64");
  } else if(platform == "nds") {
    formats.append("*.nds");
  } else if(platform == "neogeo") {
    formats.append("");
  } else if(platform == "nes") {
    formats.append("*.nes *.smc *.sfc *.fig *.swc *.mgd");
  } else if(platform == "ngp") {
    formats.append(" *.ngp");
  } else if(platform == "ngpc") {
    formats.append(" *.ngc");
  } else if(platform == "oric") {
    formats.append(" *.dsk *.tap");
  } else if(platform == "pc") {
    formats.append("*.com *.sh *.bat *.exe *.conf");
  } else if(platform == "pc88") {
    formats.append("*.d88 *.88d *.cmt *.t88");
  } else if(platform == "pcfx") {
    formats.append("*.img *.iso *.ccd *.cue");
  } else if(platform == "pcengine") {
    formats.append("*.pce *.chd *.cue");
  } else if(platform == "ports") {
    formats.append("*.com *.sh *.bat *.exe *.conf");
  } else if(platform == "ps2") {
    formats.append("*.iso *.img *.bin *.mdf *.z *.z2 *.bz2 *.dump *.cso *.ima *.gz");
  } else if(platform == "psp") {
    formats.append("*.cso *.iso *.pbp");
  } else if(platform == "psx") {
    formats.append("*.cue *.cbn *.img *.iso *.m3u *.mdf *.pbp *.toc *.z *.znx *.chd");
  } else if(platform == "saturn") {
    formats.append("*.cue *.bin *.iso *.mdf");
  } else if(platform == "scummvm") {
    formats.append("*.svm *.scummvm");
  } else if(platform == "sega32x") {
    formats.append("*.32x *.bin *.md *.smd");
  } else if(platform == "segacd") {
    formats.append("*.cue *.iso *.chd");
  } else if(platform == "sg-1000") {
    formats.append("*.bin *.sg");
  } else if(platform == "snes") {
    formats.append("*.smc *.sfc *.fig *.swc *.mgd *.bin");
  } else if(platform == "ti99") {
    formats.append("*.ctg");
  } else if(platform == "trs-80") {
    formats.append("*.dsk");
  } else if(platform == "vectrex") {
    formats.append("*.bin *.gam *.vec");
  } else if(platform == "vic20") {
    formats.append("*.crt *.d64 *.prg *.tap *.t64 *.g64 *.x64 *.vsf");
  } else if(platform == "videopac") {
    formats.append("*.bin");
  } else if(platform == "virtualboy") {
    formats.append("*.vb");
  } else if(platform == "wii") {
    formats.append("*.iso *.cso *.gcz *.wbfs");
  } else if(platform == "wonderswan") {
    formats.append("*.ws");
  } else if(platform == "wonderswancolor") {
    formats.append("*.wsc");
  } else if(platform == "x68000") {
    formats.append("*.dim *.m3u");
  } else if(platform == "x1") {
    formats.append("*.dx1 *.zip *.2d *.2hd *.tfd *.d88 *.88d *.hdm *.xdf *.dup *.cmd");
  } else if(platform == "zmachine") {
    formats.append("*.dat *.z1 *.z2 *.z3 *.z4 *.z5 *.z6 *.z7 *.z8");
  } else if(platform == "zx81") {
    formats.append("*.p *.tzx *.t81");
  } else if(platform == "zxspectrum") {
    formats.append("*.sna *.szx *.z80 *.tap *.tzx *.gz *.udi *.mgt *.img *.trd *.scl *.dsk");
  }
  return formats;
}

// If user provides no scraping source with '-s' this sets the default for the platform
QString Platform::getDefaultScraper(QString platform)
{
  QString scraper;
  if(platform == "3do") {
    scraper = "cache";
  } else if(platform == "amiga") {
    scraper = "cache";
  } else if(platform == "amstradcpc") {
    scraper = "cache";
  } else if(platform == "arcade") {
    scraper = "cache";
  } else if(platform == "apple2") {
    scraper = "cache";
  } else if(platform == "atari800") {
    scraper = "cache";
  } else if(platform == "atari2600") {
    scraper = "cache";
  } else if(platform == "atari5200") {
    scraper = "cache";
  } else if(platform == "atari7800") {
    scraper = "cache";
  } else if(platform == "atarijaguar") {
    scraper = "cache";
  } else if(platform == "atarilynx") {
    scraper = "cache";
  } else if(platform == "atarist") {
    scraper = "cache";
  } else if(platform == "c16") {
    scraper = "cache";
  } else if(platform == "c64") {
    scraper = "cache";
  } else if(platform == "c128") {
    scraper = "cache";
  } else if(platform == "coco") {
    scraper = "cache";
  } else if(platform == "coleco") {
    scraper = "cache";
  } else if(platform == "daphne") {
    scraper = "cache";
  } else if(platform == "dragon32") {
    scraper = "cache";
  } else if(platform == "dreamcast") {
    scraper = "cache";
  } else if(platform == "fba") {
    scraper = "cache";
  } else if(platform == "fds") {
    scraper = "cache";
  } else if(platform == "gameandwatch") {
    scraper = "cache";
  } else if(platform == "gamegear") {
    scraper = "cache";
  } else if(platform == "gb") {
    scraper = "cache";
  } else if(platform == "gba") {
    scraper = "cache";
  } else if(platform == "gbc") {
    scraper = "cache";
  } else if(platform == "gc") {
    scraper = "cache";
  } else if(platform == "genesis") {
    scraper = "cache";
  } else if(platform == "intellivision") {
    scraper = "cache";
  } else if(platform == "megacd") {
    scraper = "cache";
  } else if(platform == "megadrive") {
    scraper = "cache";
  } else if(platform == "mame-advmame") {
    scraper = "cache";
  } else if(platform == "mame-libretro") {
    scraper = "cache";
  } else if(platform == "mame-mame4all") {
    scraper = "cache";
  } else if(platform == "mastersystem") {
    scraper = "cache";
  } else if(platform == "msx") {
    scraper = "cache";
  } else if(platform == "n64") {
    scraper = "cache";
  } else if(platform == "nds") {
    scraper = "cache";
  } else if(platform == "neogeo") {
    scraper = "cache";
  } else if(platform == "nes") {
    scraper = "cache";
  } else if(platform == "ngp") {
    scraper = "cache";
  } else if(platform == "ngpc") {
    scraper = "cache";
  } else if(platform == "oric") {
    scraper = "cache";
  } else if(platform == "pc") {
    scraper = "cache";
  } else if(platform == "pc88") {
    scraper = "cache";
  } else if(platform == "pcfx") {
    scraper = "cache";
  } else if(platform == "pcengine") {
    scraper = "cache";
  } else if(platform == "ports") {
    scraper = "cache";
  } else if(platform == "ps2") {
    scraper = "cache";
  } else if(platform == "psp") {
    scraper = "cache";
  } else if(platform == "psx") {
    scraper = "cache";
  } else if(platform == "saturn") {
    scraper = "cache";
  } else if(platform == "scummvm") {
    scraper = "cache";
  } else if(platform == "sega32x") {
    scraper = "cache";
  } else if(platform == "segacd") {
    scraper = "cache";
  } else if(platform == "sg-1000") {
    scraper = "cache";
  } else if(platform == "snes") {
    scraper = "cache";
  } else if(platform == "steam") {
    scraper = "cache";
  } else if(platform == "ti99") {
    scraper = "cache";
  } else if(platform == "trs-80") {
    scraper = "cache";
  } else if(platform == "vectrex") {
    scraper = "cache";
  } else if(platform == "vic20") {
    scraper = "cache";
  } else if(platform == "videopac") {
    scraper = "cache";
  } else if(platform == "virtualboy") {
    scraper = "cache";
  } else if(platform == "wii") {
    scraper = "cache";
  } else if(platform == "wonderswan") {
    scraper = "cache";
  } else if(platform == "wonderswancolor") {
    scraper = "cache";
  } else if(platform == "x68000") {
    scraper = "cache";
  } else if(platform == "x1") {
    scraper = "cache";
  } else if(platform == "zmachine") {
    scraper = "cache";
  } else if(platform == "zx81") {
    scraper = "cache";
  } else if(platform == "zxspectrum") {
    scraper = "cache";
  }
  return scraper;
}

// This contains all known platform aliases as listed on each of the scraping source sites
QStringList Platform::getAliases(QString platform)
{
  QStringList aliases;
  // Platform name itself is always appended as the first alias
  aliases.append(platform);

  // If you don't find the platform listed below, it's probably because the needed alias is
  // the same as the platform name and has already been appended above.
  if(platform == "amiga") {
    aliases.append("amiga (aga)");
    aliases.append("amiga cdtv");
    aliases.append("amiga cd32");
    aliases.append("amiga cd32 (hack)");
    aliases.append("amiga cd");
  } else if(platform == "amstradcpc") {
    aliases.append("amstrad cpc");
    aliases.append("cpc");
  } else if(platform == "apple2") {
    aliases.append("apple ii");
  } else if(platform == "arcade") {
    aliases.append("neo geo");
    aliases.append("neo-geo");
    aliases.append("neo geo cd");
    aliases.append("neo-geo cd");
    aliases.append("neo-geo mvs");
    aliases.append("capcom play system");
    aliases.append("capcom play system 2");
    aliases.append("capcom play system 3");
    aliases.append("another arcade emulator");
    aliases.append("cave");
    aliases.append("daphne");
    aliases.append("atomiswave");
    aliases.append("model 2");
    aliases.append("model 3");
    aliases.append("naomi");
    aliases.append("sega st-v");
    aliases.append("mame");
    aliases.append("type x");
    aliases.append("sega classics");
    aliases.append("irem classics");
    aliases.append("seta");
    aliases.append("midway classics");
    aliases.append("capcom classics");
    aliases.append("eighting / raizing");
    aliases.append("tecmo");
    aliases.append("snk classics");
    aliases.append("namco classics");
    aliases.append("namco system 22");
    aliases.append("taito classics");
    aliases.append("konami classics");
    aliases.append("jaleco");
    aliases.append("atari classics");
    aliases.append("nintendo classics");
    aliases.append("data east classics");
    aliases.append("nmk");
    aliases.append("sammy classics");
    aliases.append("exidy");
    aliases.append("acclaim");
    aliases.append("psikyo");
    aliases.append("non jeu");
    aliases.append("technos");
    aliases.append("american laser games");
    aliases.append("dynax");
    aliases.append("kaneko");
    aliases.append("video system co.");
    aliases.append("igs");
    aliases.append("comad");
    aliases.append("amcoe");
    aliases.append("century electronics");
    aliases.append("nichibutsu");
    aliases.append("visco");
    aliases.append("alpha denshi co.");
    aliases.append("coleco");
    aliases.append("playchoice");
    aliases.append("atlus");
    aliases.append("banpresto");
    aliases.append("semicom");
    aliases.append("universal");
    aliases.append("mitchell");
    aliases.append("seibu kaihatsu");
    aliases.append("toaplan");
    aliases.append("cinematronics");
    aliases.append("incredible technologies");
    aliases.append("gaelco");
    aliases.append("mega-tech");
    aliases.append("mega-play");
  } else if(platform == "atari800") {
    aliases.append("atari 800");
    aliases.append("atari 8-bit");
    aliases.append("atari 5200");
  } else if(platform == "atari2600") {
    aliases.append("atari 2600");
  } else if(platform == "atari5200") {
    aliases.append("atari 5200");
  } else if(platform == "atari7800") {
    aliases.append("atari 7800");
  } else if(platform == "atarijaguar") {
    aliases.append("atari jaguar");
    aliases.append("jaguar");
  } else if(platform == "atarilynx") {
    aliases.append("atari lynx");
    aliases.append("lynx");
  } else if(platform == "atarist") {
    aliases.append("atari st");
    aliases.append("atari st/ste");
  } else if(platform == "c16") {
    aliases.append("plus/4");
    aliases.append("commodore plus/4");
    aliases.append("commodore 16");
    aliases.append("commodore 16, plus/4");
  } else if(platform == "c64") {
    aliases.append("commodore 64");
    aliases.append("commodore c64/128");
  } else if(platform == "c128") {
    aliases.append("commodore 128");
    aliases.append("commodore 64");
    aliases.append("commodore c64/128");
  } else if(platform == "cd32") {
    aliases.append("amiga cd32");
    aliases.append("amiga cd32 (hack)");
  } else if(platform == "cdtv") {
    aliases.append("amiga cdtv");
  } else if(platform == "coco") {
    aliases.append("trs-80 color computer");
    aliases.append("trs-80 coco");
  } else if(platform == "coleco") {
    aliases.append("colecovision");
  } else if(platform == "daphne") {
    aliases.append("pioneer laseractive");
    aliases.append("laseractive");
    aliases.append("american laser games");
  } else if(platform == "dragon32") {
    aliases.append("dragon 32/64");
  } else if(platform == "dreamcast") {
    aliases.append("sega dreamcast");
  } else if(platform == "fba") {
    aliases.append("arcade");
    aliases.append("neo geo");
    aliases.append("neo-geo");
    aliases.append("neo geo cd");
    aliases.append("neo-geo cd");
    aliases.append("neo-geo mvs");
    aliases.append("capcom play system");
    aliases.append("capcom play system 2");
    aliases.append("capcom play system 3");
    aliases.append("another arcade emulator");
    aliases.append("cave");
    aliases.append("daphne");
    aliases.append("atomiswave");
    aliases.append("model 2");
    aliases.append("model 3");
    aliases.append("naomi");
    aliases.append("sega st-v");
    aliases.append("mame");
    aliases.append("type x");
    aliases.append("sega classics");
    aliases.append("irem classics");
    aliases.append("seta");
    aliases.append("midway classics");
    aliases.append("capcom classics");
    aliases.append("eighting / raizing");
    aliases.append("tecmo");
    aliases.append("snk classics");
    aliases.append("namco classics");
    aliases.append("namco system 22");
    aliases.append("taito classics");
    aliases.append("konami classics");
    aliases.append("jaleco");
    aliases.append("atari classics");
    aliases.append("nintendo classics");
    aliases.append("data east classics");
    aliases.append("nmk");
    aliases.append("sammy classics");
    aliases.append("exidy");
    aliases.append("acclaim");
    aliases.append("psikyo");
    aliases.append("non jeu");
    aliases.append("technos");
    aliases.append("american laser games");
    aliases.append("dynax");
    aliases.append("kaneko");
    aliases.append("video system co.");
    aliases.append("igs");
    aliases.append("comad");
    aliases.append("amcoe");
    aliases.append("century electronics");
    aliases.append("nichibutsu");
    aliases.append("visco");
    aliases.append("alpha denshi co.");
    aliases.append("coleco");
    aliases.append("playchoice");
    aliases.append("atlus");
    aliases.append("banpresto");
    aliases.append("semicom");
    aliases.append("universal");
    aliases.append("mitchell");
    aliases.append("seibu kaihatsu");
    aliases.append("toaplan");
    aliases.append("cinematronics");
    aliases.append("incredible technologies");
    aliases.append("gaelco");
    aliases.append("mega-tech");
    aliases.append("mega-play");
  } else if(platform == "fds") {
    aliases.append("famicom disk system");
    aliases.append("family computer disk system");
    aliases.append("famiri konpyuta disuku shisutemu");
    aliases.append("nintendo entertainment system (nes)");
    aliases.append("nintendo");
  } else if(platform == "gameandwatch") {
    aliases.append("game & watch");
  } else if(platform == "gamegear") {
    aliases.append("sega game gear");
    aliases.append("game gear");
  } else if(platform == "gb") {
    aliases.append("nintendo game boy");
    aliases.append("game boy");
  } else if(platform == "gba") {
    aliases.append("nintendo game boy advance");
    aliases.append("game boy advance");
  } else if(platform == "gbc") {
    aliases.append("nintendo game boy color");
    aliases.append("game boy color");
    // Workaround: ScreenScraper sometimes returns gbc results with gb platform applied
    // This might bite me in the ass if other modules now return gb results for gbc games...
    aliases.append("game boy");
  } else if(platform == "gc") {
    aliases.append("gamecube");
    aliases.append("nintendo gamecube");
  } else if(platform == "genesis") {
    aliases.append("megadrive");
    aliases.append("sega genesis");
    aliases.append("mega drive");
    aliases.append("sega mega drive");
    aliases.append("sega mega drive/genesis");
  } else if(platform == "mame-advmame") {
    aliases.append("arcade");
    aliases.append("neo geo");
    aliases.append("neo-geo");
    aliases.append("neo geo cd");
    aliases.append("neo-geo cd");
    aliases.append("neo-geo mvs");
    aliases.append("capcom play system");
    aliases.append("capcom play system 2");
    aliases.append("capcom play system 3");
    aliases.append("another arcade emulator");
    aliases.append("cave");
    aliases.append("daphne");
    aliases.append("atomiswave");
    aliases.append("model 2");
    aliases.append("model 3");
    aliases.append("naomi");
    aliases.append("sega st-v");
    aliases.append("mame");
    aliases.append("type x");
    aliases.append("sega classics");
    aliases.append("irem classics");
    aliases.append("seta");
    aliases.append("midway classics");
    aliases.append("capcom classics");
    aliases.append("eighting / raizing");
    aliases.append("tecmo");
    aliases.append("snk classics");
    aliases.append("namco classics");
    aliases.append("namco system 22");
    aliases.append("taito classics");
    aliases.append("konami classics");
    aliases.append("jaleco");
    aliases.append("atari classics");
    aliases.append("nintendo classics");
    aliases.append("data east classics");
    aliases.append("nmk");
    aliases.append("sammy classics");
    aliases.append("exidy");
    aliases.append("acclaim");
    aliases.append("psikyo");
    aliases.append("non jeu");
    aliases.append("technos");
    aliases.append("american laser games");
    aliases.append("dynax");
    aliases.append("kaneko");
    aliases.append("video system co.");
    aliases.append("igs");
    aliases.append("comad");
    aliases.append("amcoe");
    aliases.append("century electronics");
    aliases.append("nichibutsu");
    aliases.append("visco");
    aliases.append("alpha denshi co.");
    aliases.append("coleco");
    aliases.append("playchoice");
    aliases.append("atlus");
    aliases.append("banpresto");
    aliases.append("semicom");
    aliases.append("universal");
    aliases.append("mitchell");
    aliases.append("seibu kaihatsu");
    aliases.append("toaplan");
    aliases.append("cinematronics");
    aliases.append("incredible technologies");
    aliases.append("gaelco");
    aliases.append("mega-tech");
    aliases.append("mega-play");
  } else if(platform == "mame-libretro") {
    aliases.append("arcade");
    aliases.append("neo geo");
    aliases.append("neo-geo");
    aliases.append("neo geo cd");
    aliases.append("neo-geo cd");
    aliases.append("neo-geo mvs");
    aliases.append("capcom play system");
    aliases.append("capcom play system 2");
    aliases.append("capcom play system 3");
    aliases.append("another arcade emulator");
    aliases.append("cave");
    aliases.append("daphne");
    aliases.append("atomiswave");
    aliases.append("model 2");
    aliases.append("model 3");
    aliases.append("naomi");
    aliases.append("sega st-v");
    aliases.append("mame");
    aliases.append("type x");
    aliases.append("sega classics");
    aliases.append("irem classics");
    aliases.append("seta");
    aliases.append("midway classics");
    aliases.append("capcom classics");
    aliases.append("eighting / raizing");
    aliases.append("tecmo");
    aliases.append("snk classics");
    aliases.append("namco classics");
    aliases.append("namco system 22");
    aliases.append("taito classics");
    aliases.append("konami classics");
    aliases.append("jaleco");
    aliases.append("atari classics");
    aliases.append("nintendo classics");
    aliases.append("data east classics");
    aliases.append("nmk");
    aliases.append("sammy classics");
    aliases.append("exidy");
    aliases.append("acclaim");
    aliases.append("psikyo");
    aliases.append("non jeu");
    aliases.append("technos");
    aliases.append("american laser games");
    aliases.append("dynax");
    aliases.append("kaneko");
    aliases.append("video system co.");
    aliases.append("igs");
    aliases.append("comad");
    aliases.append("amcoe");
    aliases.append("century electronics");
    aliases.append("nichibutsu");
    aliases.append("visco");
    aliases.append("alpha denshi co.");
    aliases.append("coleco");
    aliases.append("playchoice");
    aliases.append("atlus");
    aliases.append("banpresto");
    aliases.append("semicom");
    aliases.append("universal");
    aliases.append("mitchell");
    aliases.append("seibu kaihatsu");
    aliases.append("toaplan");
    aliases.append("cinematronics");
    aliases.append("incredible technologies");
    aliases.append("gaelco");
    aliases.append("mega-tech");
    aliases.append("mega-play");
  } else if(platform == "mame-mame4all") {
    aliases.append("arcade");
    aliases.append("neo geo");
    aliases.append("neo-geo");
    aliases.append("neo geo cd");
    aliases.append("neo-geo cd");
    aliases.append("neo-geo mvs");
    aliases.append("capcom play system");
    aliases.append("capcom play system 2");
    aliases.append("capcom play system 3");
    aliases.append("another arcade emulator");
    aliases.append("cave");
    aliases.append("daphne");
    aliases.append("atomiswave");
    aliases.append("model 2");
    aliases.append("model 3");
    aliases.append("naomi");
    aliases.append("sega st-v");
    aliases.append("mame");
    aliases.append("type x");
    aliases.append("sega classics");
    aliases.append("irem classics");
    aliases.append("seta");
    aliases.append("midway classics");
    aliases.append("capcom classics");
    aliases.append("eighting / raizing");
    aliases.append("tecmo");
    aliases.append("snk classics");
    aliases.append("namco classics");
    aliases.append("namco system 22");
    aliases.append("taito classics");
    aliases.append("konami classics");
    aliases.append("jaleco");
    aliases.append("atari classics");
    aliases.append("nintendo classics");
    aliases.append("data east classics");
    aliases.append("nmk");
    aliases.append("sammy classics");
    aliases.append("exidy");
    aliases.append("acclaim");
    aliases.append("psikyo");
    aliases.append("non jeu");
    aliases.append("technos");
    aliases.append("american laser games");
    aliases.append("dynax");
    aliases.append("kaneko");
    aliases.append("video system co.");
    aliases.append("igs");
    aliases.append("comad");
    aliases.append("amcoe");
    aliases.append("century electronics");
    aliases.append("nichibutsu");
    aliases.append("visco");
    aliases.append("alpha denshi co.");
    aliases.append("coleco");
    aliases.append("playchoice");
    aliases.append("atlus");
    aliases.append("banpresto");
    aliases.append("semicom");
    aliases.append("universal");
    aliases.append("mitchell");
    aliases.append("seibu kaihatsu");
    aliases.append("toaplan");
    aliases.append("cinematronics");
    aliases.append("incredible technologies");
    aliases.append("gaelco");
    aliases.append("mega-tech");
    aliases.append("mega-play");
  } else if(platform == "mastersystem") {
    aliases.append("sega master system");
    aliases.append("master system");
  } else if(platform == "megacd") {
    aliases.append("segacd");
    aliases.append("sega cd");
    aliases.append("mega-cd");
  } else if(platform == "megadrive") {
    aliases.append("genesis");
    aliases.append("sega mega drive");
    aliases.append("sega mega drive/genesis");
    aliases.append("mega drive");
  } else if(platform == "msx") {
    aliases.append("msx2");
    aliases.append("msx2+");
    aliases.append("msx r turbo");
  } else if(platform == "n64") {
    aliases.append("nintendo 64");
  } else if(platform == "nds") {
    aliases.append("nintendo ds");
  } else if(platform == "neogeo") {
    aliases.append("neo-geo");
    aliases.append("neo geo");
    aliases.append("neo geo aes");
    aliases.append("neo geo mvs");
    aliases.append("neo geo cd");
    aliases.append("arcade");
  } else if(platform == "nes") {
    aliases.append("nintendo entertainment system (nes)");
    aliases.append("nintendo");
  } else if(platform == "ngp") {
    aliases.append("neo geo pocket");
    aliases.append("neo-geo pocket");
  } else if(platform == "ngpc") {
    aliases.append("neo geo pocket color");
    aliases.append("neo-geo pocket color");
  } else if(platform == "oric") {
    aliases.append("oric 1 / atmos");
  } else if(platform == "pc") {
    aliases.append("pc dos");
    aliases.append("dos");
    aliases.append("windows");
    aliases.append("windows apps");
    aliases.append("pc (microsoft windows");
    aliases.append("steamos");
  } else if(platform == "pc88") {
    aliases.append("pc-88");
    aliases.append("pc-8801");
  } else if(platform == "pcfx") {
    aliases.append("pc-fx");
  } else if(platform == "pcengine") {
    aliases.append("turbografx 16");
    aliases.append("turbografx cd");
    aliases.append("turbografx-16");
    aliases.append("turbografx-16/pc engine");
    aliases.append("turbografx-16/pc engine cd");
    aliases.append("pc engine");
    aliases.append("pc engine cd-rom");
    aliases.append("pc engine supergrafx");
  } else if(platform == "ports") {
    aliases.append("pc dos");
    aliases.append("dos");
    aliases.append("windows");
    aliases.append("pc (microsoft windows");
  } else if(platform == "psx") {
    aliases.append("sony playstation");
    aliases.append("playstation");
  } else if(platform == "ps2") {
    aliases.append("playstation 2");
    aliases.append("sony playstation 2");
  } else if(platform == "psp") {
    aliases.append("sony playstation portable");
    aliases.append("playstation portable");
  } else if(platform == "saturn") {
    aliases.append("sega saturn");
  } else if(platform == "scummvm") {
    aliases.append("pc dos");
    aliases.append("pc");
    aliases.append("amiga");
    aliases.append("amiga cd32");
    aliases.append("dos");
    aliases.append("cd32");
    aliases.append("pc (microsoft windows");
    aliases.append("windows");
    aliases.append("windows apps");
  } else if(platform == "sega32x") {
    aliases.append("sega 32x");
    aliases.append("megadrive 32x");
  } else if(platform == "segacd") {
    aliases.append("megacd");
    aliases.append("sega cd");
    aliases.append("mega-cd");
  } else if(platform == "sg-1000") {
    aliases.append("sega sg-1000");
  } else if(platform == "snes") {
    aliases.append("super nintendo (snes)");
    aliases.append("super nintendo entertainment system (snes)");
    aliases.append("super nintendo");
    aliases.append("super famicom");
  } else if(platform == "steam") {
    aliases.append("pc dos");
    aliases.append("dos");
    aliases.append("windows");
    aliases.append("windows apps");
    aliases.append("pc (microsoft windows");
    aliases.append("steamos");
    // Adding these consoles because 'thegamesdb' often only return these instead of PC versions
    aliases.append("microsoft xbox one");
    aliases.append("sony playstation 4");
  } else if(platform == "ti99") {
    aliases.append("ti-99/4a");
    aliases.append("texas instruments ti-99");
    aliases.append("texas instruments ti-99/4a");
  } else if(platform == "trs-80") {
    aliases.append("trs-80 color computer");
  } else if(platform == "vic20") {
    aliases.append("commodore vic-20");
    aliases.append("vic-20");
  } else if(platform == "videopac") {
    aliases.append("odyssey 2");
    aliases.append("magnavox odyssey 2");
    aliases.append("videopac g7000");
    aliases.append("philips videopac g7000");
    aliases.append("videopac+ g7400");
    aliases.append("magnavox odyssey²");
  } else if(platform == "virtualboy") {
    aliases.append("nintendo virtual boy");
    aliases.append("virtual boy");
  } else if(platform == "wii") {
    aliases.append("nintendo wii");
    aliases.append("wiiware");
  } else if(platform == "wonderswancolor") {
    aliases.append("wonderswan color");
  } else if(platform == "x68000") {
    aliases.append("sharp x68000");
  } else if(platform == "zmachine") {
    aliases.append("x1");
  } else if(platform == "sharp x1") {
    aliases.append("z-machine");
  } else if(platform == "zxspectrum") {
    aliases.append("sinclair zx spectrum");
    aliases.append("zx spectrum");
  }
  return aliases;
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
