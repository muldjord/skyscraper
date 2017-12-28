/***************************************************************************
 *            attractmode.cpp
 *
 *  Wed Jun 18 12:00:00 CEST 2017
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

#include <QDir>
#include <QDate>

#include "attractmode.h"
#include "strtools.h"

AttractMode::AttractMode()
{
}

void AttractMode::skipExisting(const QString &gameListFileString,
			       QList<GameEntry> &gameEntries, QList<QFileInfo> &inputFiles)
{
  printf("Parsing existing romlist entries, please wait...\n");
  QFile gameListFile(gameListFileString);
  if(gameListFile.open(QIODevice::ReadOnly)) {
    while(!gameListFile.atEnd()) {
      QList<QByteArray> snippets = gameListFile.readLine().split(';');
      if(snippets.length() >= 17) {
	if(snippets.at(0) == "#Name") {
	  continue;
	}
	GameEntry entry;
	entry.baseName = snippets.at(0);
	setNotes(entry, entry.baseName);
	QString title = snippets.at(1);
	entry.title = title.left(title.indexOf("(")).left(title.indexOf("[")).simplified();
	entry.releaseDate = snippets.at(4) + "0101";
	entry.publisher = snippets.at(5);
	entry.tags = snippets.at(6);
	entry.players = snippets.at(7);
	gameEntries.append(entry);
      }
    }
    gameListFile.close();
  } else {
    printf("Error while trying to load existing gamelist!\n");
    printf("Can't resolve existing entries... :(\n");
    exit(1);
  }
  printf("Resolving missing entries...");
  int dots = 0;
  for(int a = 0; a < gameEntries.length(); ++a) {
    dots++;
    if(dots % 50 == 0) {
      printf(".");
    }
    for(int b = 0; b < inputFiles.length(); ++b) {
      if(gameEntries.at(a).baseName == inputFiles.at(b).completeBaseName()) {
	inputFiles.removeAt(b);
	// We assume baseName is unique, so break after getting first hit
	break;
      }
    }
  }
    printf(" \033[1;32mDone!\033[0m\n");
}

void AttractMode::setNotes(GameEntry &entry, QString baseName)
{
  QString baseNameOrig = baseName;
  // Get square notes
  while(baseName.indexOf("[") != -1 && baseName.indexOf("]") != -1) {
    if(baseName.indexOf("[") != -1 && baseName.indexOf("]") != -1) {
      entry.sqrNotes.append(baseName.mid(baseName.indexOf("["),
				  baseName.indexOf("]") - baseName.indexOf("[") + 1));
    }
    baseName.remove(baseName.indexOf("["),
		    baseName.indexOf("]") - baseName.indexOf("[") + 1);
  }
  entry.sqrNotes = entry.sqrNotes.simplified();

  // Reset
  baseName = baseNameOrig;

  // Get parenthesis notes  
  while(baseName.indexOf("(") != -1 && baseName.indexOf(")") != -1) {
    if(baseName.indexOf("(") != -1 && baseName.indexOf(")") != -1) {
      entry.parNotes.append(baseName.mid(baseName.indexOf("("),
				  baseName.indexOf(")") - baseName.indexOf("(") + 1) + " ");
    }
    baseName.remove(baseName.indexOf("("),
		    baseName.indexOf(")") - baseName.indexOf("(") + 1);
  }
  entry.parNotes = entry.parNotes.simplified();

  // Reset
  baseName = baseNameOrig;
}

void AttractMode::assembleList(QString &finalOutput, const QList<GameEntry> &gameEntries, int)
{
  finalOutput.append("#Name;Title;Emulator;CloneOf;Year;Manufacturer;Category;Players;Rotation;Control;Status;DisplayCount;DisplayType;AltRomname;AltTitle;Extra;Buttons\n");
  foreach(GameEntry entry, gameEntries) {
    QFileInfo emuInfo(config->emulator);
    finalOutput.append(entry.baseName + ";" +
		       entry.title);
    if(config->brackets) {
      finalOutput.append((entry.parNotes != ""?" " + entry.parNotes:"") + (entry.sqrNotes != ""?" " + entry.sqrNotes:""));
    }
    finalOutput.append(";" +		       
		       emuInfo.completeBaseName() + ";" +
		       ";" +
		       QDate::fromString(entry.releaseDate, "yyyyMMdd").toString("yyyy") + ";" +
		       entry.publisher + ";" +
		       entry.tags + ";" +
		       entry.players + ";" +
		       "0" + ";" +
		       ";" +
		       ";" +
		       ";" +
		       ";" +
		       ";" +
		       ";" +
		       ";" +
		       ";\n");
  }
}

void AttractMode::checkReqs()
{
  if(config->emulator.isEmpty()) {
    printf("Frontend 'attractmode' requires emulator set with '-e'. Check '--help' for more information.\n");
    exit(0);
  }
  if(config->emulator.indexOf(".cfg") == -1) {
    config->emulator.append(".cfg");
  }
  
  printf("Looking for emulator cfg file:\n");

  if(checkEmulatorFile(config->emulator)) {
    return;
  }

  if(checkEmulatorFile(QDir::homePath() + "/.attract/emulators/" + config->emulator)) {
    return;
  }

  if(checkEmulatorFile("/opt/retropie/configs/all/attractmode/emulators/" + config->emulator)) {
    return;
  }

  printf("Couldn't locate emulator cfg file, exiting...\n");
  exit(1);
}

bool AttractMode::checkEmulatorFile(QString fileName)
{
  QFileInfo info(fileName);
  printf("Trying '%s'... ", info.absoluteFilePath().toStdString().c_str());

  if(info.exists() && info.isFile()) {
    config->emulator = info.absoluteFilePath();
    printf("\033[1;32mFound!\033[0m\n\n");
    return true;
  } else {
    printf("Not found!\n");
    return false;
  }
}

bool AttractMode::canSkip()
{
  return true;
}

QString AttractMode::getGameListFileName()
{
  QFileInfo info(config->emulator);
  return QString(info.completeBaseName() + ".txt");
}

QString AttractMode::getInputFolder()
{
  return QString("/home/pi/RetroPie/roms/" + config->platform);
}

QString AttractMode::getGameListFolder()
{
  if(QFileInfo::exists(QDir::homePath() + "/.attract/romlists")) {
    return QString(QDir::homePath() + "/.attract/romlists");
  }
  if(QFileInfo::exists("/opt/retropie/configs/all/attractmode/romlists")) {
    return QString("/opt/retropie/configs/all/attractmode/romlists");
  }
  
  return config->inputFolder;
}

QString AttractMode::getCoversFolder()
{
  return getMediaTypeFolder("flyer");
}

QString AttractMode::getScreenshotsFolder()
{
  return getMediaTypeFolder("snap");
}

QString AttractMode::getWheelsFolder()
{
  return getMediaTypeFolder("wheel");
}

QString AttractMode::getMarqueesFolder()
{
  return getMediaTypeFolder("marquee");
}

QString AttractMode::getVideosFolder()
{
  return getMediaTypeFolder("snap");
}

QString AttractMode::getMediaTypeFolder(QString type)
{
  QFile emulatorFile(config->emulator);
  QString mediaTypeFolder = config->mediaFolder + "/" + type + "s";
  
  if(emulatorFile.exists() && emulatorFile.open(QIODevice::ReadOnly)) {
    while(!emulatorFile.atEnd()) {
      QList<QByteArray> snippets = emulatorFile.readLine().simplified().split(' ');
      foreach(QByteArray snippet, snippets) {
      }
      if(snippets.length() == 3 && snippets.at(0) == "artwork" && snippets.at(1) == type) {
	mediaTypeFolder = snippets.at(2);
      }
    }
    emulatorFile.close();
  }

  return mediaTypeFolder;
}

/*
Emulator files: /opt/retropie/configs/all/attractmode/emulators
Romlists: /opt/retropie/configs/all/attractmode/romlists

---- Example emulator file begin ----
# Generated by Attract-Mode v2.2.1
#
executable           /opt/retropie/supplementary/runcommand/runcommand.sh
args                 0 _SYS_ amiga "[romfilename]"
rompath              /home/pi/RetroPie/roms/amiga
romext               .adf;.adz;.dms;.ipf;.uae;.zip;.sh;.ADF;.ADZ;.DMS;.IPF;.UAE;.ZIP;.SH
system               Commodore Amiga
info_source          thegamesdb.net
artwork    boxart          /home/pi/RetroPie/roms/amiga/boxart
artwork    cartart         /home/pi/RetroPie/roms/amiga/cartart
artwork    snap            /home/pi/RetroPie/roms/amiga/snap
artwork    wheel           /home/pi/RetroPie/roms/amiga/wheel
---- End ----

---- Example romlist entry begin ----
#Name;Title;Emulator;CloneOf;Year;Manufacturer;Category;Players;Rotation;Control;Status;DisplayCount;DisplayType;AltRomname;AltTitle;Extra;Buttons
Bubble Bobble;Bubble Bobble;Commodore Amiga;;1987;Firebird / Taito;arcade, cartoonish, cooperative, cute, jumponthings, platform, powerup, single;1 - 2 (2 simultaneous);0;;;;;;;;;
Games - Winter Edition;The Games: Winter Edition;Commodore Amiga;;1989;U.S. Gold / Epyx;multievent, olympics, sports;1 - 8 (1 simultaneous);0;;;;;;;;;
---- End ----
*/
