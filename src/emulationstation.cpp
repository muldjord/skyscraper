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

EmulationStation::EmulationStation()
{
}

void EmulationStation::skipExisting(const QString &gameListFileString,
				    QList<GameEntry> &gameEntries, QList<QFileInfo> &inputFiles)
{
  XmlReader gameListReader;
  if(gameListReader.setFile(gameListFileString)) {
    gameEntries = gameListReader.getEntries();
  } else {
    qDebug("Error while trying to load existing gamelist.xml!\n");
    qDebug("Can't resolve existing entries... :(\n");
    exit(1);
  }
  qDebug("Resolving missing entries...\n");
  for(int a = 0; a < gameEntries.length(); ++a) {
    QFileInfo current(gameEntries.at(a).path);
    for(int b = 0; b < inputFiles.length(); ++b) {
      if(current.fileName() == inputFiles.at(b).fileName()) {
	inputFiles.removeAt(b);
	// We assume filename is unique, so break after getting first hit
	break;
      }
    }
  }
}

void EmulationStation::assembleList(QString &finalOutput, const QList<GameEntry> &gameEntries,
				    int maxLength)
{
  finalOutput.append("<?xml version=\"1.0\"?>\n<gameList>\n");
  foreach(GameEntry entry, gameEntries) {
    finalOutput.append("  <game>\n");
    finalOutput.append("    <path>" + StrTools::xmlEscape(entry.path) + "</path>\n");
    if(config->brackets) {
      finalOutput.append("    <name>" + StrTools::xmlEscape(entry.title + (entry.parNotes != ""?" " + entry.parNotes:"") + (entry.sqrNotes != ""?" " + entry.sqrNotes:"")) + "</name>\n");
    } else {
      finalOutput.append("    <name>" + StrTools::xmlEscape(entry.title) + "</name>\n");
    }
    if(entry.imageFile.isEmpty()) {
      finalOutput.append("    <image />\n");
    } else {
      finalOutput.append("    <image>" + StrTools::xmlEscape(entry.imageFile) + "</image>\n");
    }
    if(entry.videoFormat.isEmpty()) {
      finalOutput.append("    <video />\n");
    } else {
      finalOutput.append("    <video>" + StrTools::xmlEscape(entry.videoFile) + "</video>\n");
    }
    if(entry.rating.isEmpty()) {
      finalOutput.append("    <rating />\n");
    } else {
      finalOutput.append("    <rating>" + StrTools::xmlEscape(entry.rating) + "</rating>\n");
    }
    if(entry.description.isEmpty()) {
      finalOutput.append("    <desc />\n");
    } else {
      finalOutput.append("    <desc>" + StrTools::xmlEscape(entry.description.left(maxLength)) + "</desc>\n");
    }
    if(entry.releaseDate.isEmpty()) {
      finalOutput.append("    <releasedate />\n");
    } else {
      finalOutput.append("    <releasedate>" + StrTools::xmlEscape(entry.releaseDate) + "</releasedate>\n");
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
    finalOutput.append("  </game>\n");
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
  return QString("/home/pi/RetroPie/roms/" + config->platform);
}

QString EmulationStation::getGameListFolder()
{
  return config->inputFolder;
}

QString EmulationStation::getImagesFolder()
{
  return config->gameListFolder + "/images";
}

QString EmulationStation::getVideosFolder()
{
  return config->gameListFolder + "/videos";
}
