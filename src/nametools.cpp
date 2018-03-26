/***************************************************************************
 *            nametools.cpp
 *
 *  Tue Feb 20 12:00:00 CEST 2018
 *  Copyright 2018 Lars Muldjord
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

#include <QFileInfo>
#include <QDir>
#include <QSettings>

#include "nametools.h"

QString NameTools::getMameName(const QString &baseName, const QMap<QString, QString> &mameMap)
{
  if(!mameMap[baseName].isEmpty()) {
    return mameMap[baseName];
  }
  return baseName;
}

QString NameTools::getScummName(const QString &baseName)
{
  // Set to global for RetroPie
  QString scummIniStr = "/opt/retropie/configs/scummvm/scummvm.ini";

  // If local exists, use that one instead
  if(QFileInfo::exists(QDir::homePath() + "/.scummvmrc")) {
    scummIniStr = QDir::homePath() + "/.scummvmrc";
  }

  QSettings scummIni(scummIniStr, QSettings::IniFormat);
  if(scummIni.contains(baseName + "/description")) {
    return scummIni.value(baseName + "/description").toString();
  }
  return baseName;
}

QString NameTools::getNameWithSpaces(const QString &baseName)
{
  // Only perform if name contains no spaces
  if(baseName.indexOf(" ") != -1) {
    return baseName;
  }

  QString withSpaces = "";
  QChar previous;
  for(int a = 0; a < baseName.length(); ++a) {
    QChar current = baseName.at(a);
    if(current == '_' ||
       (a > 4 && baseName.mid(a, 4) == "Demo") ||
       baseName.mid(a, 5) == "-WHDL") {
      break;
    }
    if(a > 0) {
      if(current.isDigit() && (!previous.isDigit() && previous != 'x')) {
	withSpaces.append(" ");
      } else if(current == '&') {
	withSpaces.append(" ");
      } else if(current == 'D') {
	if(baseName.mid(a, 6) == "Deluxe") {
	  withSpaces.append(" ");
	} else if(previous != '3' && previous != '4') {
	  withSpaces.append(" ");
	}
      } else if(current.isUpper()) {
	if(previous.isLetter() && !previous.isUpper()) {
	  withSpaces.append(" ");
	} else if(previous == '&') {
	  withSpaces.append(" ");
	} else if(previous == 'D') {
	  withSpaces.append(" ");
	} else if(previous == 'C') {
	  withSpaces.append(" ");
	} else if(previous == 'O') {
	  withSpaces.append(" ");
	} else if(previous.isDigit()) {
	  withSpaces.append(" ");
	}
      }
    }
    withSpaces.append(current);
    previous = current;
  }
  //printf("withSpaces: '%s'\n", withSpaces.toStdString().c_str());
  return withSpaces;
}
