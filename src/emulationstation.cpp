/***************************************************************************
 *            emulationstation.cpp
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

#include "emulationstation.h"
#include "xmlreader.h"
#include "strtools.h"

#include <QDir>

EmulationStation::EmulationStation()
{
}

bool EmulationStation::loadOldGameList(const QString &gameListFileString)
{
  // Load old game list entries so we can preserve metadata later when assembling xml
  XmlReader gameListReader;
  if(gameListReader.setFile(gameListFileString)) {
    oldEntries = gameListReader.getEntries();
    return true;
  }

  return false;
}

bool EmulationStation::skipExisting(QList<GameEntry> &gameEntries, QSharedPointer<Queue> queue, const QString inputFolder) 
{
  gameEntries = oldEntries;

  printf("Resolving missing entries...\n");
  int dots = 0;
  for(int a = 0; a < gameEntries.length(); ++a) {
    dots++;
    if(dots % 100 == 0) {
      printf(".");
      fflush(stdout);
    }
    QString filePath = gameEntries.at(a).path;
    // Make sure we readd the input path if the 'path' is relative
    if(filePath.left(1) == ".") {
      filePath.remove(0, 1);
      filePath.prepend(inputFolder);
    }
    QFileInfo current(filePath);
    for(int b = 0; b < queue->length(); ++b) {
      if(current.isFile()) {
	if(current.fileName() == queue->at(b).fileName()) {
	  queue->removeAt(b);
	  // We assume filename is unique, so break after getting first hit
	  break;
	}
      } else if(current.isDir()) {
	// Use current.absoluteFilePath here since it is already a path. Otherwise it will use
	// the parent folder
	if(current.absoluteFilePath() == queue->at(b).absolutePath()) {
	  queue->removeAt(b);
	  // We assume filename is unique, so break after getting first hit
	  break;
	}
      }
    }
  }
  return true;
}

void EmulationStation::preserveFromOld(GameEntry &entry)
{
  foreach(GameEntry oldEntry, oldEntries) {
    QString oldFileName = oldEntry.path.mid(oldEntry.path.indexOf("/"), oldEntry.path.length());
    QString fileName = entry.path.mid(entry.path.indexOf("/"), entry.path.length());
    if(oldFileName == fileName) {
      if(entry.eSFavorite.isEmpty())
	entry.eSFavorite = oldEntry.eSFavorite;
      if(entry.eSHidden.isEmpty())
	entry.eSHidden = oldEntry.eSHidden;
      if(entry.eSPlayCount.isEmpty())
	entry.eSPlayCount = oldEntry.eSPlayCount;
      if(entry.eSLastPlayed.isEmpty())
	entry.eSLastPlayed = oldEntry.eSLastPlayed;
      if(entry.eSKidGame.isEmpty())
	entry.eSKidGame = oldEntry.eSKidGame;
      if(entry.eSSortName.isEmpty())
	entry.eSSortName = oldEntry.eSSortName;
      if(entry.developer.isEmpty())
	entry.developer = oldEntry.developer;
      if(entry.publisher.isEmpty())
	entry.publisher = oldEntry.publisher;
      if(entry.players.isEmpty())
	entry.players = oldEntry.players;
      if(entry.description.isEmpty())
	entry.description = oldEntry.description;
      if(entry.rating.isEmpty())
	entry.rating = oldEntry.rating;
      if(entry.releaseDate.isEmpty())
	entry.releaseDate = oldEntry.releaseDate;
      if(entry.tags.isEmpty())
	entry.tags = oldEntry.tags;
      break;
    }
  }
}

void EmulationStation::assembleList(QString &finalOutput, const QList<GameEntry> &gameEntries)
{
  int dots = 0;
  // Always make dotMod at least 1 or it will give "floating point exception" when modulo
  int dotMod = gameEntries.length() * 0.1 + 1;
  if(dotMod == 0)
    dotMod = 1;
  finalOutput.append("<?xml version=\"1.0\"?>\n<gameList>\n");
  foreach(GameEntry entry, gameEntries) {
    if(dots % dotMod == 0) {
      printf(".");
      fflush(stdout);
    }
    dots++;

    QString entryType = "game";

    // Make sure we readd the input path if the 'path' is relative. This happens when user
    // skips entries.
    if(entry.path.left(1) == ".") {
      entry.path.remove(0, 1);
      entry.path.prepend(config->inputFolder);
    }
    QFileInfo entryInfo(entry.path);
    
    if(entryInfo.isFile()) {
      // Check if game is in subfolder and if it's a .cue or .m3u file.
      // If so, change entry to <folder> type.
      QString entryAbsolutePath = entryInfo.absolutePath();
      if(entryAbsolutePath.count("/") == config->inputFolder.count("/") + 1) {
	QString entrySuffix = entryInfo.suffix();
	if((entrySuffix == "cue" ||
	    entrySuffix == "m3u") &&
	   QDir(entryAbsolutePath, "*.cue *.m3u").count() == 1) {
	  entryType = "folder";
	  entry.path = entryAbsolutePath;
	}
      }
    } else if(entryInfo.isDir()) {
      entryType = "folder";
    }

    // Preserve certain data from old game list entry, but only for empty data
    preserveFromOld(entry);

    if(config->relativePaths) {
      entry.path.replace(config->inputFolder, ".");
    }

    finalOutput.append("  <" + entryType + ">\n");
    finalOutput.append("    <path>" + StrTools::xmlEscape(entry.path) + "</path>\n");
    if(config->brackets) {
      finalOutput.append("    <name>" + StrTools::xmlEscape(entry.title + (entry.parNotes != ""?" " + entry.parNotes:"") + (entry.sqrNotes != ""?" " + entry.sqrNotes:"")) + "</name>\n");
    } else {
      finalOutput.append("    <name>" + StrTools::xmlEscape(entry.title) + "</name>\n");
    }
    if(entry.coverFile.isEmpty()) {
      finalOutput.append("    <cover />\n");
    } else {
      finalOutput.append("    <cover>" + (config->relativePaths?StrTools::xmlEscape(entry.coverFile).replace(config->inputFolder, "."):StrTools::xmlEscape(entry.coverFile)) + "</cover>\n");
    }
    if(entry.screenshotFile.isEmpty()) {
      finalOutput.append("    <image />\n");
    } else {
      finalOutput.append("    <image>" + (config->relativePaths?StrTools::xmlEscape(entry.screenshotFile).replace(config->inputFolder, "."):StrTools::xmlEscape(entry.screenshotFile)) + "</image>\n");
    }
    if(entry.marqueeFile.isEmpty()) {
      finalOutput.append("    <marquee />\n");
    } else {
      finalOutput.append("    <marquee>" + (config->relativePaths?StrTools::xmlEscape(entry.marqueeFile).replace(config->inputFolder, "."):StrTools::xmlEscape(entry.marqueeFile)) + "</marquee>\n");
    }
    if(entry.videoFormat.isEmpty() || !config->videos) {
      finalOutput.append("    <video />\n");
    } else {
      finalOutput.append("    <video>" + (config->relativePaths?StrTools::xmlEscape(entry.videoFile).replace(config->inputFolder, "."):StrTools::xmlEscape(entry.videoFile)) + "</video>\n");
    }
    if(entry.rating.isEmpty()) {
      finalOutput.append("    <rating />\n");
    } else {
      finalOutput.append("    <rating>" + StrTools::xmlEscape(entry.rating) + "</rating>\n");
    }
    if(entry.description.isEmpty()) {
      finalOutput.append("    <desc />\n");
    } else {
      finalOutput.append("    <desc>" + StrTools::xmlEscape(entry.description.left(config->maxLength)) + "</desc>\n");
    }
    if(entry.releaseDate.isEmpty()) {
      finalOutput.append("    <releasedate />\n");
    } else {
      finalOutput.append("    <releasedate>" + StrTools::xmlEscape(entry.releaseDate) + "T000000</releasedate>\n");
    }
    if(entry.developer.isEmpty()) {
      finalOutput.append("    <developer />\n");
    } else {
      finalOutput.append("    <developer>" + StrTools::xmlEscape(entry.developer) + "</developer>\n");
    }
    if(entry.publisher.isEmpty()) {
      finalOutput.append("    <publisher />\n");
    } else {
      finalOutput.append("    <publisher>" + StrTools::xmlEscape(entry.publisher) + "</publisher>\n");
    }
    if(entry.tags.isEmpty()) {
      finalOutput.append("    <genre />\n");
    } else {
      finalOutput.append("    <genre>" + StrTools::xmlEscape(entry.tags) + "</genre>\n");
    }
    if(entry.players.isEmpty()) {
      finalOutput.append("    <players />\n");
    } else {
      finalOutput.append("    <players>" + StrTools::xmlEscape(entry.players) + "</players>\n");
    }
    if(!entry.eSSortName.isEmpty()) {
      finalOutput.append("    <sortname>" + StrTools::xmlEscape(entry.eSSortName) + "</sortname>\n");
    }
    if(!entry.eSFavorite.isEmpty()) {
      finalOutput.append("    <favorite>" + StrTools::xmlEscape(entry.eSFavorite) + "</favorite>\n");
    }
    if(!entry.eSHidden.isEmpty()) {
      finalOutput.append("    <hidden>" + StrTools::xmlEscape(entry.eSHidden) + "</hidden>\n");
    }
    if(!entry.eSLastPlayed.isEmpty()) {
      finalOutput.append("    <lastplayed>" + StrTools::xmlEscape(entry.eSLastPlayed) + "</lastplayed>\n");
    }
    if(!entry.eSPlayCount.isEmpty()) {
      finalOutput.append("    <playcount>" + StrTools::xmlEscape(entry.eSPlayCount) + "</playcount>\n");
    }
    if(entry.eSKidGame.isEmpty()) {
      if(!entry.ages.isEmpty() && (entry.ages.toInt() >= 1 && entry.ages.toInt() <= 10)) {
	finalOutput.append("    <kidgame>true</kidgame>\n");
      }
    } else {
      finalOutput.append("    <kidgame>" + StrTools::xmlEscape(entry.eSKidGame) + "</kidgame>\n");
    }
    finalOutput.append("  </" + entryType + ">\n");
  }
  finalOutput.append("</gameList>");
}

bool EmulationStation::canSkip()
{
  return true;
}

QString EmulationStation::getGameListFileName()
{
  return QString("gamelist.xml");
}

QString EmulationStation::getInputFolder()
{
  return QString(QDir::homePath() + "/RetroPie/roms/" + config->platform);
}

QString EmulationStation::getGameListFolder()
{
  return config->inputFolder;
}

QString EmulationStation::getCoversFolder()
{
  return config->mediaFolder + "/covers";
}

QString EmulationStation::getScreenshotsFolder()
{
  return config->mediaFolder + "/screenshots";
}

QString EmulationStation::getWheelsFolder()
{
  return config->mediaFolder + "/wheels";
}

QString EmulationStation::getMarqueesFolder()
{
  return config->mediaFolder + "/marquees";
}

QString EmulationStation::getVideosFolder()
{
  return config->mediaFolder + "/videos";
}
