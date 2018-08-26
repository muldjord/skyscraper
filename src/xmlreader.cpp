/***************************************************************************
 *            xmlreader.cpp
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
#include <QFile>
#include <QFileInfo>

#include "xmlreader.h"
#include "strtools.h"

XmlReader::XmlReader()
{
}

XmlReader::~XmlReader()
{
}

bool XmlReader::setFile(QString filename)
{
  bool result = false;

  QFile f(filename);
  if(f.open(QIODevice::ReadOnly)) {
    if(setContent(f.readAll(), false)) {
      result = true;
    }
    f.close();
  }
  return result;
}

QList<GameEntry> XmlReader::getEntries()
{
  QList<GameEntry> gameEntries;

  QDomNodeList gameNodes = elementsByTagName("game");

  printf("Parsing existing xml entries...\n");
  for(int a = 0; a < gameNodes.length(); ++a) {
    GameEntry entry;
    entry.path = gameNodes.at(a).firstChildElement("path").text();
    QString baseName = QFileInfo(entry.path).completeBaseName();
    setNotes(entry, baseName);
    QString title = gameNodes.at(a).firstChildElement("name").text();
    entry.title = title.left(title.indexOf("(")).left(title.indexOf("[")).simplified();
    entry.coverFile = gameNodes.at(a).firstChildElement("cover").text();
    entry.screenshotFile = gameNodes.at(a).firstChildElement("image").text();
    entry.wheelFile = gameNodes.at(a).firstChildElement("wheel").text();
    entry.marqueeFile = gameNodes.at(a).firstChildElement("marquee").text();
    entry.videoFile = gameNodes.at(a).firstChildElement("video").text();
    if(!entry.videoFile.isEmpty()) {
      entry.videoFormat = "fromxml";
    }
    entry.description = gameNodes.at(a).firstChildElement("desc").text();
    entry.releaseDate = gameNodes.at(a).firstChildElement("releasedate").text();
    entry.developer = gameNodes.at(a).firstChildElement("developer").text();
    entry.publisher = gameNodes.at(a).firstChildElement("publisher").text();
    entry.tags = gameNodes.at(a).firstChildElement("genre").text();
    entry.rating = gameNodes.at(a).firstChildElement("rating").text();
    entry.players = gameNodes.at(a).firstChildElement("players").text();
    gameEntries.append(entry);
  }
  return gameEntries;
}

void XmlReader::setNotes(GameEntry &entry, QString baseName)
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
