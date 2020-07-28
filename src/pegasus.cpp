/***************************************************************************
 *            pegasus.cpp
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

#include "pegasus.h"
#include "strtools.h"

#include <QDir>
#include <QDate>

Pegasus::Pegasus()
{
}

bool Pegasus::loadOldGameList(const QString &gameListFileString)
{
  // Load old game list entries so we can preserve metadata later when assembling new game list
  QFile gameListFile(gameListFileString);
  if(gameListFile.exists() && gameListFile.open(QIODevice::ReadOnly)) {
    QByteArray line = "";
    // Parse header value pairs
    while(!gameListFile.atEnd()) {
      line = gameListFile.readLine();
      if(line.contains(':')) {
	QPair<QString, QString> valuePair;
	valuePair.first = QString::fromUtf8(line.left(line.indexOf(':')).simplified());
	if(valuePair.first == "game") {
	  GameEntry oldEntry;
	  line = line.remove(0, line.indexOf(':') + 1).simplified();
	  oldEntry.title = StrTools::stripBrackets(line);
	  oldEntries.append(oldEntry);
	  break;
	}
	valuePair.second = line.remove(0, line.indexOf(':') + 1).simplified();
	if(!valuePair.first.isEmpty()) {
	  headerPairs.append(valuePair);
	}
      } else if(line.left(1) == " " || line.left(1) == "\t") {
	headerPairs.last().second.append("\n" + line.simplified());
      }
    }
    QString *currentPairValue = nullptr;
    // Parse games
    while(!gameListFile.atEnd()) {
      line = gameListFile.readLine();
      if(line.contains(':')) {
	QString header = QString::fromUtf8(line.left(line.indexOf(":")));
	if(header == "game") {
	  GameEntry oldEntry;
	  line = line.remove(0, line.indexOf(':') + 1).simplified();
	  oldEntry.title = StrTools::stripBrackets(line);
	  oldEntries.append(oldEntry);
	} else if(header == "file" || header == "files") {
	  oldEntries.last().path = QString::fromUtf8(line.right(line.length() - line.indexOf(":") - 1).simplified());
	  currentPairValue = &oldEntries.last().path;
	} else if(header == "developer") {
	  oldEntries.last().developer = QString::fromUtf8(line.right(line.length() - line.indexOf(":") - 1).simplified());
	  currentPairValue = &oldEntries.last().developer;
	} else if(header == "publisher") {
	  oldEntries.last().publisher = QString::fromUtf8(line.right(line.length() - line.indexOf(":") - 1).simplified());
	  currentPairValue = &oldEntries.last().publisher;
	} else if(header == "release") {
	  oldEntries.last().releaseDate = QString::fromUtf8(line.right(line.length() - line.indexOf(":") - 1).simplified());
	  currentPairValue = &oldEntries.last().releaseDate;
	} else if(header == "genre") {
	  oldEntries.last().tags = QString::fromUtf8(line.right(line.length() - line.indexOf(":") - 1).simplified());
	  currentPairValue = &oldEntries.last().tags;
	} else if(header == "players") {
	  oldEntries.last().players = QString::fromUtf8(line.right(line.length() - line.indexOf(":") - 1).simplified());
	  currentPairValue = &oldEntries.last().players;
	} else if(header == "assets.boxFront") {
	  oldEntries.last().coverFile = makeAbsolute(QString::fromUtf8(line.right(line.length() - line.indexOf(":") - 1).simplified()), config->inputFolder);
	  currentPairValue = &oldEntries.last().coverFile;
	} else if(header == "assets.marquee") {
	  oldEntries.last().marqueeFile = makeAbsolute(QString::fromUtf8(line.right(line.length() - line.indexOf(":") - 1).simplified()), config->inputFolder);
	  currentPairValue = &oldEntries.last().marqueeFile;
	} else if(header == "assets.wheel") {
	  oldEntries.last().wheelFile = makeAbsolute(QString::fromUtf8(line.right(line.length() - line.indexOf(":") - 1).simplified()), config->inputFolder);
	  currentPairValue = &oldEntries.last().wheelFile;
	} else if(header == "assets.video") {
	  oldEntries.last().videoFile = makeAbsolute(QString::fromUtf8(line.right(line.length() - line.indexOf(":") - 1).simplified()), config->inputFolder);
	  oldEntries.last().videoFormat = "fromgamelist"; // Irrelevant, just set to something
	  currentPairValue = &oldEntries.last().videoFile;
	} else if(header == "description") {
	  oldEntries.last().description = QString::fromUtf8(line.right(line.length() - line.indexOf(":") - 1).simplified());
	  currentPairValue = &oldEntries.last().description;
	} else {
	  QPair<QString, QString> pSValuePair;
	  pSValuePair.first = header;
	  pSValuePair.second = QString::fromUtf8(line.right(line.length() - line.indexOf(":") - 1).simplified());
	  oldEntries.last().pSValuePairs.append(pSValuePair);
	  currentPairValue = &pSValuePair.second;
	}
      } else if(currentPairValue != nullptr && (line.left(1) == " " || line.left(1) == "\t")) {
	currentPairValue->append("\n" + line.simplified());
      }
    }
    gameListFile.close();
    return true;
  }

  return false;
}

QString Pegasus::makeAbsolute(QString filePath, const QString &inputFolder)
{
  if(filePath.left(1) == ".") {
    filePath.remove(0, 1);
    filePath.prepend(inputFolder);
  }
  return filePath;
}


bool Pegasus::skipExisting(QList<GameEntry> &gameEntries, QSharedPointer<Queue> queue) 
{
  gameEntries = oldEntries;

  printf("Resolving missing entries...");
  int dots = 0;
  for(int a = 0; a < gameEntries.length(); ++a) {
    dots++;
    if(dots % 100 == 0) {
      printf(".");
      fflush(stdout);
    }
    QFileInfo current(gameEntries.at(a).path);
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

void Pegasus::preserveFromOld(GameEntry &entry)
{
  for(const auto &oldEntry: oldEntries) {
    QString oldFileName = oldEntry.path.mid(oldEntry.path.lastIndexOf("/"), oldEntry.path.length());
    QString fileName = entry.path.mid(entry.path.lastIndexOf("/"), entry.path.length());
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

void Pegasus::assembleList(QString &finalOutput, QList<GameEntry> &gameEntries)
{
  QList<QString> extensionsList;
  for(const auto &entry: gameEntries) {
    QString entryExtension = entry.path.split(".").last();
    bool extFound = false;
    for(const auto &extension: extensionsList) {
      if(entryExtension == extension) {
	extFound = true;
	break;
      }
    }
    if(!extFound) {
      extensionsList.append(entryExtension);
    }
  }
  QString extensions = "";
  for(const auto &extension: extensionsList) {
    extensions.append(extension + ", ");
  }
  extensions = extensions.left(extensions.length() - 2);

  if(!gameEntries.isEmpty()) {
    finalOutput.append("collection: " + gameEntries.first().platform + "\n");
    finalOutput.append("shortname: " + config->platform + "\n");
    finalOutput.append("extensions: " + extensions + "\n");
    if(config->frontendExtra.isEmpty()) {
      finalOutput.append("command: /opt/retropie/supplementary/runcommand/runcommand.sh 0 _SYS_ " + config->platform + " \"{file.path}\"\n");
    } else {
      finalOutput.append("command: " + config->frontendExtra + "\n");
    }
    finalOutput.append("\n\n");
  }
  int dots = 0;
  // Always make dotMod at least 1 or it will give "floating point exception" when modulo
  int dotMod = gameEntries.length() * 0.1 + 1;
  if(dotMod == 0)
    dotMod = 1;
  for(auto &entry: gameEntries) {
    if(dots % dotMod == 0) {
      printf(".");
      fflush(stdout);
    }
    dots++;

    if(config->relativePaths) {
      entry.path.replace(config->inputFolder, ".");
    }
    
    finalOutput.append("game: " + entry.title + "\n");
    finalOutput.append("file: " + entry.path + "\n");
    // The replace here IS supposed to be 'inputFolder' and not 'mediaFolder' because we only want the path to be relative if '-o' hasn't been set. So this will only make it relative if the path is equal to inputFolder which is what we want.
    if(!entry.rating.isEmpty()) {
      finalOutput.append("rating: " + QString::number((int)(entry.rating.toDouble() * 100)) + "%\n");
    }
    if(!entry.description.isEmpty()) {
      QString remainingDesc = "description: " + entry.description.left(config->maxLength);
      QString finalDesc = "";
      while(!remainingDesc.isEmpty()) {
	QString line = "";
	if(line.contains("\n")) {
	  line = remainingDesc.remove(0, remainingDesc.indexOf("\n") + 1).trimmed();
	} else {
	  line = remainingDesc.trimmed();
	  remainingDesc = "";
	}
	if(line.isEmpty()) {
	  line = "  .\n";
	} else {
	  line = "  " + line + "\n";
	}
	printf("HERE!\n");
	finalDesc.append(line);
      }
      finalOutput.append(finalDesc.trimmed() + "\n");
    }
    if(!entry.releaseDate.isEmpty()) {
      finalOutput.append("release: " + QDate::fromString(entry.releaseDate, "yyyyMMdd").toString("yyyy-MM-dd") + "\n");
    }
    if(!entry.developer.isEmpty()) {
      finalOutput.append("developer: " + entry.developer + "\n");
    }
    if(!entry.publisher.isEmpty()) {
      finalOutput.append("publisher: " + entry.publisher + "\n");
    }
    if(!entry.tags.isEmpty()) {
      finalOutput.append("genre: " + entry.tags + "\n");
    }
    if(!entry.players.isEmpty()) {
      finalOutput.append("players: " + entry.players + "\n");
    }
    if(!entry.screenshotFile.isEmpty()) {
      finalOutput.append("assets.screenshots: " + (config->relativePaths?entry.screenshotFile.replace(config->inputFolder, "."):entry.screenshotFile) + "\n");
    }
    if(!entry.coverFile.isEmpty()) {
      finalOutput.append("assets.boxFront: " + (config->relativePaths?entry.coverFile.replace(config->inputFolder, "."):entry.coverFile) + "\n");
    }
    if(!entry.marqueeFile.isEmpty()) {
      finalOutput.append("assets.marquee: " + (config->relativePaths?entry.marqueeFile.replace(config->inputFolder, "."):entry.marqueeFile) + "\n");
    }
    if(!entry.wheelFile.isEmpty()) {
      finalOutput.append("assets.wheel: " + (config->relativePaths?entry.wheelFile.replace(config->inputFolder, "."):entry.wheelFile) + "\n");
    }
    if(!entry.videoFormat.isEmpty() && config->videos) {
      finalOutput.append("assets.video: " + (config->relativePaths?entry.videoFile.replace(config->inputFolder, "."):entry.videoFile) + "\n");
    }
    finalOutput.append("\n\n");
  }
}

bool Pegasus::canSkip()
{
  return true;
}

QString Pegasus::getGameListFileName()
{
  return QString("metadata.pegasus.txt");
}

QString Pegasus::getInputFolder()
{
  return QString(QDir::homePath() + "/RetroPie/roms/" + config->platform);
}

QString Pegasus::getGameListFolder()
{
  return config->inputFolder;
}

QString Pegasus::getCoversFolder()
{
  return config->mediaFolder + "/covers";
}

QString Pegasus::getScreenshotsFolder()
{
  return config->mediaFolder + "/screenshots";
}

QString Pegasus::getWheelsFolder()
{
  return config->mediaFolder + "/wheels";
}

QString Pegasus::getMarqueesFolder()
{
  return config->mediaFolder + "/marquees";
}

QString Pegasus::getVideosFolder()
{
  return config->mediaFolder + "/videos";
}
