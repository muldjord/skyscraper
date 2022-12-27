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

#include "nametools.h"
#include "strtools.h"

#include <QFileInfo>
#include <QDir>
#include <QSettings>
#include <QRegularExpression>
#include <QCryptographicHash>

QString NameTools::getScummName(const QString baseName, const QString scummIni)
{
  // Set to global for RetroPie
  QString scummIniStr = "/opt/retropie/configs/scummvm/scummvm.ini";

  // If local exists, use that one instead
  if(QFileInfo::exists(QDir::homePath() + "/.scummvmrc")) {
    scummIniStr = QDir::homePath() + "/.scummvmrc";
  }

  // If set in config, use that one instead
  if(!scummIni.isEmpty()) {
    scummIniStr = scummIni;
  }


  QFile scummIniFile(scummIniStr);
  if(scummIniFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    int state = 0;
    while(!scummIniFile.atEnd()) {
      QByteArray line = scummIniFile.readLine();
      if(line.contains("[")) {
	state = 0; // Always reset if traversing into a new game
      }
      if(state == 0 && line.contains("[" + baseName.toUtf8() + "]")) {
	state = 1;
      }
      if(state == 1 && line.contains("description=")) {
	return line.split('=').last();
      }
    }
    scummIniFile.close();
  }
  return baseName;
}

QString NameTools::getNameWithSpaces(const QString baseName)
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

QString NameTools::getUrlQueryName(const QString baseName, const int words, const QString spaceChar)
{
  QString newName = baseName;
  // Remove everything in brackets
  newName = newName.left(newName.indexOf("(")).simplified();
  newName = newName.left(newName.indexOf("[")).simplified();
  // The following is mostly, if not only, used when getting the name from mameMap
  newName = newName.left(newName.indexOf(" / ")).simplified();
  // Always remove everything after a ':' since it's always a subtitle
  //newName = newName.left(newName.indexOf(":")).simplified();
  // Always remove everything after a ' - ' since it's always a subtitle
  //newName = newName.left(newName.indexOf(" - ")).simplified();

  QRegularExpressionMatch match;
  // Remove " rev.X" instances
  match = QRegularExpression(" rev[.]{0,1}([0-9]{1}[0-9]{0,2}[.]{0,1}[0-9]{1,4}|[IVX]{1,5})$").match(newName);
  if(match.hasMatch() && match.capturedStart(0) != -1) {
    newName = newName.left(match.capturedStart(0)).simplified();
  }
  // Remove versioning instances
  match = QRegularExpression(" v[.]{0,1}([0-9]{1}[0-9]{0,2}[.]{0,1}[0-9]{1,4}|[IVX]{1,5})$").match(newName);
  if(match.hasMatch() && match.capturedStart(0) != -1) {
    newName = newName.left(match.capturedStart(0)).simplified();
  }

  // If we have the first game in a series, remove the ' I' for more search results
  if(newName.right(2) == " I") {
    newName = newName.left(newName.length() - 2);
  }

  newName = newName.toLower();

  // Always remove 'the' from beginning or end if equal to or longer than 10 chars.
  // If it's shorter the 'the' is of more significance and shouldn't be removed.
  if(newName.length() >= 10) {
    if(newName.simplified().left(4) == "the ") {
      newName = newName.simplified().remove(0, 4);
    }
    QRegularExpressionMatch match;
    match = QRegularExpression(", the( - |:|$)").match(newName);
    if(match.hasMatch()) {
      newName = newName.replace(match.captured(0), " ");
    }
  }
  newName = newName.replace("_", " ");
  newName = newName.replace(" - ", " ");
  newName = newName.replace(",", " ");
  newName = newName.replace("&", "%26");
  newName = newName.replace("+", "");
  // A few game names have faulty "s's". Fix them to "s'"
  newName = newName.replace("s's", "s'");
  newName = newName.replace("'", "%27");
  // Finally change all spaces to requested char. Default is '+' since that's what most search engines seem to understand
  newName = newName.simplified().replace(" ", spaceChar);

  // Implement special cases here
  if(newName == "ik") {
    newName = "international+karate";
  }
  if(newName == "arkanoid+revenge+of+doh") {
    newName = "arkanoid%3A+revenge+of+doh";
  }
  if(newName == "lemmings+3") {
    newName = "all+new+world+of+lemmings";
  }

  if(words != -1) {
    QList<QString> wordList = newName.split(spaceChar);
    if(wordList.size() > words) {
      newName.clear();
      for(int a = 0; a < words; ++a) {
	newName.append(wordList.at(a) + spaceChar);
      }
      newName = newName.left(newName.length() - spaceChar.length());
    }
  }
  return newName;
}

bool NameTools::hasIntegerNumeral(const QString baseName)
{
  if(QRegularExpression(" [0-9]{1,2}([: ]+|$)").match(baseName).hasMatch())
    return true;
  return false;
}

bool NameTools::hasRomanNumeral(const QString baseName)
{
  if(QRegularExpression(" [IVX]{1,5}([: ]+|$)").match(baseName).hasMatch())
    return true;
  return false;
}

QString NameTools::convertToRomanNumeral(const QString baseName)
{
  QRegularExpressionMatch match;
  QString newName = baseName;

  match = QRegularExpression(" [0-9]{1,2}([: ]+|$)").match(baseName);
  // Match is either " 2" or " 2: yada yada"
  if(match.hasMatch()) {
    QString integer = match.captured(0);
    if(integer.contains(":")) {
      integer = integer.left(integer.indexOf(":")).simplified();
    } else {
      integer = integer.simplified();
    }
    if(integer == "1") {
      return newName.replace(match.captured(0), match.captured(0).replace(integer, "I"));
    } else if(integer == "2") {
      return newName.replace(match.captured(0), match.captured(0).replace(integer, "II"));
    } else if(integer == "3") {
      return newName.replace(match.captured(0), match.captured(0).replace(integer, "III"));
    } else if(integer == "4") {
      return newName.replace(match.captured(0), match.captured(0).replace(integer, "IV"));
    } else if(integer == "5") {
      return newName.replace(match.captured(0), match.captured(0).replace(integer, "V"));
    } else if(integer == "6") {
      return newName.replace(match.captured(0), match.captured(0).replace(integer, "VI"));
    } else if(integer == "7") {
      return newName.replace(match.captured(0), match.captured(0).replace(integer, "VII"));
    } else if(integer == "8") {
      return newName.replace(match.captured(0), match.captured(0).replace(integer, "VIII"));
    } else if(integer == "9") {
      return newName.replace(match.captured(0), match.captured(0).replace(integer, "IX"));
    } else if(integer == "10") {
      return newName.replace(match.captured(0), match.captured(0).replace(integer, "X"));
    } else if(integer == "11") {
      return newName.replace(match.captured(0), match.captured(0).replace(integer, "XI"));
    } else if(integer == "12") {
      return newName.replace(match.captured(0), match.captured(0).replace(integer, "XII"));
    } else if(integer == "13") {
      return newName.replace(match.captured(0), match.captured(0).replace(integer, "XIII"));
    } else if(integer == "14") {
      return newName.replace(match.captured(0), match.captured(0).replace(integer, "XIV"));
    } else if(integer == "15") {
      return newName.replace(match.captured(0), match.captured(0).replace(integer, "XV"));
    } else if(integer == "16") {
      return newName.replace(match.captured(0), match.captured(0).replace(integer, "XVI"));
    } else if(integer == "17") {
      return newName.replace(match.captured(0), match.captured(0).replace(integer, "XVII"));
    } else if(integer == "18") {
      return newName.replace(match.captured(0), match.captured(0).replace(integer, "XVIII"));
    } else if(integer == "19") {
      return newName.replace(match.captured(0), match.captured(0).replace(integer, "XIX"));
    } else if(integer == "20") {
      return newName.replace(match.captured(0), match.captured(0).replace(integer, "XX"));
    }
  }
  return newName;
}

QString NameTools::convertToIntegerNumeral(const QString baseName)
{
  QRegularExpressionMatch match;
  QString newName = baseName;

  match = QRegularExpression(" [IVX]{1,5}([: ]+|$)").match(baseName);
  // Match is either " X" or " X: yada yada"
  if(match.hasMatch()) {
    QString roman = match.captured(0);
    if(roman.contains(":")) {
      roman = roman.left(roman.indexOf(":")).simplified();
    } else {
      roman = roman.simplified();
    }
    if(roman == "I") {
      return newName.replace(match.captured(0), match.captured(0).replace(roman, "1"));
    } else if(roman == "II") {
      return newName.replace(match.captured(0), match.captured(0).replace(roman, "2"));
    } else if(roman == "III") {
      return newName.replace(match.captured(0), match.captured(0).replace(roman, "3"));
    } else if(roman == "IV") {
      return newName.replace(match.captured(0), match.captured(0).replace(roman, "4"));
    } else if(roman == "V") {
      return newName.replace(match.captured(0), match.captured(0).replace(roman, "5"));
    } else if(roman == "VI") {
      return newName.replace(match.captured(0), match.captured(0).replace(roman, "6"));
    } else if(roman == "VII") {
      return newName.replace(match.captured(0), match.captured(0).replace(roman, "7"));
    } else if(roman == "VIII") {
      return newName.replace(match.captured(0), match.captured(0).replace(roman, "8"));
    } else if(roman == "IX") {
      return newName.replace(match.captured(0), match.captured(0).replace(roman, "9"));
    } else if(roman == "X") {
      return newName.replace(match.captured(0), match.captured(0).replace(roman, "10"));
    } else if(roman == "XI") {
      return newName.replace(match.captured(0), match.captured(0).replace(roman, "11"));
    } else if(roman == "XII") {
      return newName.replace(match.captured(0), match.captured(0).replace(roman, "12"));
    } else if(roman == "XIII") {
      return newName.replace(match.captured(0), match.captured(0).replace(roman, "13"));
    } else if(roman == "XIV") {
      return newName.replace(match.captured(0), match.captured(0).replace(roman, "14"));
    } else if(roman == "XV") {
      return newName.replace(match.captured(0), match.captured(0).replace(roman, "15"));
    } else if(roman == "XVI") {
      return newName.replace(match.captured(0), match.captured(0).replace(roman, "16"));
    } else if(roman == "XVII") {
      return newName.replace(match.captured(0), match.captured(0).replace(roman, "17"));
    } else if(roman == "XVIII") {
      return newName.replace(match.captured(0), match.captured(0).replace(roman, "18"));
    } else if(roman == "XIX") {
      return newName.replace(match.captured(0), match.captured(0).replace(roman, "19"));
    } else if(roman == "XX") {
      return newName.replace(match.captured(0), match.captured(0).replace(roman, "20"));
    }
  }
  return newName;
}

int NameTools::getNumeral(const QString baseName)
{
  QRegularExpressionMatch match;
  int numeral = 1;

  // Check for roman numerals
  match = QRegularExpression(" [IVX]{1,5}([: ]+|$)").match(baseName);
  if(match.hasMatch()) {
    QString roman = match.captured(0).replace(":", "").replace(" ", "").simplified();
    if(roman == "I") {
      numeral = 1;
    } else if(roman == "II") {
      numeral = 2;
    } else if(roman == "III") {
      numeral = 3;
    } else if(roman == "IV") {
      numeral = 4;
    } else if(roman == "V") {
      numeral = 5;
    } else if(roman == "VI") {
      numeral = 6;
    } else if(roman == "VII") {
      numeral = 7;
    } else if(roman == "VIII") {
      numeral = 8;
    } else if(roman == "IX") {
      numeral = 9;
    } else if(roman == "X") {
      numeral = 10;
    } else if(roman == "XI") {
      numeral = 11;
    } else if(roman == "XII") {
      numeral = 12;
    } else if(roman == "XIII") {
      numeral = 13;
    } else if(roman == "XIV") {
      numeral = 14;
    } else if(roman == "XV") {
      numeral = 15;
    } else if(roman == "XVI") {
      numeral = 16;
    } else if(roman == "XVII") {
      numeral = 17;
    } else if(roman == "XVIII") {
      numeral = 18;
    } else if(roman == "XIX") {
      numeral = 19;
    } else if(roman == "XX") {
      numeral = 20;
    }
  }

  // Check for digit numerals
  match = QRegularExpression("\\d+([: ]+|$)").match(baseName);
  if(match.hasMatch()) {
    QString intStr = match.captured(0).replace(":", "").simplified();
    if(intStr.toInt() != 0) {
      numeral = intStr.toInt();
    }
  }

  return numeral;
}

QString NameTools::getSqrNotes(QString baseName)
{
  QString sqrNotes = "";

  // Get square notes
  while(baseName.contains("[") && baseName.contains("]") &&
	baseName.indexOf("[") < baseName.indexOf("]")) {
    if(baseName.indexOf("[") != -1 && baseName.indexOf("]") != -1) {
      sqrNotes.append(baseName.mid(baseName.indexOf("["),
			      baseName.indexOf("]") - baseName.indexOf("[") + 1));
    }
    baseName.remove(baseName.indexOf("["),
	       baseName.indexOf("]") - baseName.indexOf("[") + 1);
  }

  // Look for '_tag_' or '[tag]' with the last char optional
  if(QRegularExpression("[_[]{1}(Aga|AGA)[_\\]]{0,1}").match(baseName).hasMatch())
    sqrNotes.append("[AGA]");
  if(QRegularExpression("[_[]{1}(Cd32|cd32|CD32)[_\\]]{0,1}").match(baseName).hasMatch())
    sqrNotes.append("[CD32]");
  if(QRegularExpression("[_[]{1}(Cdtv|cdtv|CDTV)[_\\]]{0,1}").match(baseName).hasMatch())
    sqrNotes.append("[CDTV]");
  if(QRegularExpression("[_[]{1}(Ntsc|ntsc|NTSC)[_\\]]{0,1}").match(baseName).hasMatch())
    sqrNotes.append("[NTSC]");
  if(QRegularExpression("(Demo|demo|DEMO)[_\\]]{1}").match(baseName).hasMatch())
    sqrNotes.append("[Demo]");
  // Don't add PAL detection as it will also match with "_Palace" and such
  sqrNotes = sqrNotes.simplified();

  return sqrNotes;
}

QString NameTools::getParNotes(QString baseName)
{
  QString parNotes = "";

  // Get parentheses notes
  while(baseName.contains("(") && baseName.contains(")") &&
	baseName.indexOf("(") < baseName.indexOf(")")) {
    if(baseName.indexOf("(") != -1 && baseName.indexOf(")") != -1) {
      parNotes.append(baseName.mid(baseName.indexOf("("),
			      baseName.indexOf(")") - baseName.indexOf("(") + 1));
    }
    baseName.remove(baseName.indexOf("("),
	       baseName.indexOf(")") - baseName.indexOf("(") + 1);
  }

  QRegularExpressionMatch match;

  // Add "nDisk" detection
  match = QRegularExpression("[0-9]{1,2}[ ]{0,1}Disk").match(baseName);
  if(match.hasMatch()) {
    parNotes.append("(" + match.captured(0).left(match.captured(0).indexOf("Disk")).trimmed() + " Disk)");
  }
  // Add "CD" detection that DON'T match CD32 and CDTV
  if(QRegularExpression("[_[]{1}CD(?!32|TV)").match(baseName).hasMatch())
    parNotes.append("(CD)");
  // Look for language and add it
  match = QRegularExpression("[_[]{1}(De|It|Pl|Fr|Es|Fi|Dk|Gr|Cz){1,10}[_\\]]{0,1}").match(baseName);
  if(match.hasMatch()) {
    parNotes.append("(" +
		    match.captured(0).replace("_", "").
		    replace("[", "").
		    replace("]", "") +
		    ")");
  }

  parNotes = parNotes.simplified();

  return parNotes;
}

QString NameTools::getUniqueNotes(const QString &notes, QChar delim)
{
#if QT_VERSION >= 0x050e00
  QList<QString> notesList = notes.split(delim, Qt::SkipEmptyParts);
#else
  QList<QString> notesList = notes.split(delim, QString::SkipEmptyParts);
#endif
  QList<QString> uniqueList;
  for(const auto &note: notesList) {
    bool found = false;
    for(const auto &unique: uniqueList) {
      if(note.toLower() == unique.toLower()) {
	found = true;
      }
    }
    if(!found) {
      uniqueList.append(delim + note);
    }
  }
  QString uniqueNotes;
  for(const auto &note: uniqueList) {
    uniqueNotes.append(note);
  }
  return uniqueNotes;
}

QString NameTools::getCacheId(const QFileInfo &info)
{
  QCryptographicHash cacheId(QCryptographicHash::Sha1);

  // Use checksum of filename if file is a script or an "unstable" compressed filetype
  bool cacheIdFromData = true;
  if(info.suffix() == "uae" || info.suffix() == "cue" ||
     info.suffix() == "sh" || info.suffix() == "svm" ||
     info.suffix() == "scummvm" || info.suffix() == "mds" ||
     info.suffix() == "zip" || info.suffix() == "7z" ||
     info.suffix() == "gdi" || info.suffix() == "ml" ||
     info.suffix() == "bat" || info.suffix() == "au3" ||
     info.suffix() == "po" || info.suffix() == "dsk" ||
     info.suffix() == "nib") {
    cacheIdFromData = false;
  }
  // If file is larger than 50 MBs, use filename checksum for cache id for optimization reasons
  if(info.size() > 52428800) {
    cacheIdFromData = false;
  }
  // If file is empty always do checksum on filename
  if(info.size() == 0) {
    cacheIdFromData = false;
  }
  if(cacheIdFromData) {
    QFile romFile(info.absoluteFilePath());
    if(romFile.open(QIODevice::ReadOnly)) {
      while(!romFile.atEnd()) {
	cacheId.addData(romFile.read(1024));
      }
      romFile.close();
    } else {
      printf("Couldn't calculate cache id of rom file '%s', please check permissions and try again, now exiting...\n", info.fileName().toStdString().c_str());
      exit(1);
    }
  } else {
    cacheId.addData(info.fileName().toUtf8());
  }

  return cacheId.result().toHex();
}

QString NameTools::getNameFromTemplate(const GameEntry &game, const QString &nameTemplate)
{
  QList<QString> templateGroups = nameTemplate.split(";");
  QString finalName;
  for(auto &templateGroup: templateGroups) {
    bool include = false;
    if(templateGroup.contains("%t") && !game.title.isEmpty()) {
      include = true;
    }
    if(templateGroup.contains("%f") && !game.baseName.isEmpty()) {
      include = true;
    }
    if(templateGroup.contains("%b") && !game.parNotes.isEmpty()) {
      include = true;
    }
    if(templateGroup.contains("%B") && !game.sqrNotes.isEmpty()) {
      include = true;
    }
    if(templateGroup.contains("%a") && !game.ages.isEmpty()) {
      include = true;
    }
    if(templateGroup.contains("%d") && !game.developer.isEmpty()) {
      include = true;
    }
    if(templateGroup.contains("%p") && !game.publisher.isEmpty()) {
      include = true;
    }
    if(templateGroup.contains("%P") && !game.players.isEmpty()) {
      include = true;
    }
    if(templateGroup.contains("%D") && !game.releaseDate.isEmpty()) {
      include = true;
    }
    if(include) {
      templateGroup.replace("%t", game.title);
      templateGroup.replace("%f", StrTools::stripBrackets(game.baseName));
      templateGroup.replace("%b", game.parNotes);
      templateGroup.replace("%B", game.sqrNotes);
      templateGroup.replace("%a", game.ages);
      templateGroup.replace("%d", game.developer);
      templateGroup.replace("%p", game.publisher);
      templateGroup.replace("%P", game.players);
      templateGroup.replace("%D", game.releaseDate);
      finalName.append(templateGroup);
    }
  }

  return finalName;
}
