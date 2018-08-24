/***************************************************************************
 *            strtools.cpp
 *
 *  Wed Jun 7 12:00:00 CEST 2017
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

#include <QDate>
#include <QRegularExpression>

#include "strtools.h"

QString StrTools::xmlUnescape(QString str)
{
  return str.replace("&amp;", "&").
    replace("&lt;", "<").
    replace("&gt;", ">").
    replace("&quot;", "\"").
    replace("&apos;", "'").
    replace("&copy;", "(c)").
    replace("&#32;", " ").
    replace("&#33;", "!").
    replace("&#34;", "\"").
    replace("&#35;", "#").
    replace("&#36;", "$").
    replace("&#37;", "%").
    replace("&#38;", "&").
    replace("&#39;", "'").
    replace("&#40;", "(").
    replace("&#41;", ")").
    replace("&#42;", "*").
    replace("&#43;", "+").
    replace("&#44;", ",").
    replace("&#45;", "-").
    replace("&#46;", ".").
    replace("&#47;", "/").
    replace("&#032;", " ").
    replace("&#033;", "!").
    replace("&#034;", "\"").
    replace("&#035;", "#").
    replace("&#036;", "$").
    replace("&#037;", "%").
    replace("&#038;", "&").
    replace("&#039;", "'").
    replace("&#040;", "(").
    replace("&#041;", ")").
    replace("&#042;", "*").
    replace("&#043;", "+").
    replace("&#044;", ",").
    replace("&#045;", "-").
    replace("&#046;", ".").
    replace("&#047;", "/").
    replace("&#160;", " ").
    replace("&#8211;", "-").
    replace("&#8217;", "'").
    replace("&#xF4;", "o").
    replace("&#xE3;", "a").
    replace("&#xE4;", "ae").
    replace("&#xE1;", "a").
    replace("&#xE9;", "e").
    replace("&#xED;", "i").
    replace("&#x16B;", "uu").
    replace("&#x22;", "\"").
    replace("&#x26;", "&").
    replace("&#x27;", "'").
    replace("&#xB3;", "3").
    replace("&#x14D;", "o");
}

QString StrTools::xmlEscape(QString str)
{
  str = xmlUnescape(str);
  return str.
    replace("&", "&amp;").
    replace("<", "&lt;").
    replace(">", "&gt;").
    replace("\"", "&quot;").
    replace("'", "&apos;");
}

QByteArray StrTools::magic(QByteArray str)
{
  QByteArray magicStr("WowMuchEncryptedVeryImpressIGuessThisHasToBeQuiteLongToAlsoSupportSomeVeryLongKeys");

  int strChars[str.length()];
  int magicChars[str.length()];

  for(int a = 0; a < str.length(); ++a) {
    strChars[a] = str.at(a);
  }

  for(int a = 0; a < str.length(); ++a) {
    magicChars[a] = magicStr.at(a);
  }

  QByteArray thingie;
  for(int a = 0; a < str.length(); ++a) {
    thingie.append(QString::number(strChars[a] += magicChars[a]) + ";");
  }

  thingie = thingie.left(thingie.length() - 1);
  
  return thingie; 
}

QByteArray StrTools::unMagic(QByteArray str)
{
  int length = str.split(';').length();

  QByteArray magicStr("WowMuchEncryptedVeryImpressIGuessThisHasToBeQuiteLongToAlsoSupportSomeVeryLongKeys");

  int strChars[length];
  int magicChars[length];

  for(int a = 0; a < length; ++a) {
    strChars[a] = str.split(';').at(a).toInt();
  }
  for(int a = 0; a < length; ++a) {
    magicChars[a] = magicStr.at(a);
  }
  QByteArray thingie;
  for(int a = 0; a < length; ++a) {
    thingie.append(QChar(strChars[a] -= magicChars[a]));
  }

  return thingie; 
}

QString StrTools::conformPlayers(QString str)
{
  if(QRegularExpression("^1 Player").match(str).hasMatch())
    return "1";

  if(QRegularExpression("^1 Only").match(str).hasMatch())
    return "1";

  if(QRegularExpression("^single player").match(str).hasMatch())
    return "1";
  
  if(QRegularExpression("^1 or 2").match(str).hasMatch())
    return "2";

  if(QRegularExpression("^\\d-\\d\\d").match(str).hasMatch())
    return str.mid(2, 2);

  if(QRegularExpression("^\\d-\\d").match(str).hasMatch())
    return str.mid(2, 1);

  if(QRegularExpression("^\\d - \\d\\d").match(str).hasMatch())
    return str.mid(4, 2);

  if(QRegularExpression("^\\d - \\d").match(str).hasMatch())
    return str.mid(4, 1);

  // A faulty Openretro entry is necessary as it marks "1 - 6" as "1 -6"
  if(QRegularExpression("^\\d -\\d\\d").match(str).hasMatch())
    return str.mid(3, 2);

  if(QRegularExpression("^\\d -\\d").match(str).hasMatch())
    return str.mid(3, 1);

  if(QRegularExpression("^\\d to \\d\\d").match(str).hasMatch())
    return str.mid(5, 2);

  if(QRegularExpression("^\\d to \\d").match(str).hasMatch())
    return str.mid(5, 1);

  if(QRegularExpression("^\\d\\+").match(str).hasMatch())
    return str.mid(0, 1);
  
  return str;
}

QString StrTools::conformAges(QString str)
{
  if(str == "0 (ohne Altersbeschränkung)") {
    str = "1";
  } else if(str == "U") {
    str = "1";
  } else if(str == "E") {
    str = "1";
  } else if(str == "E - Everyone") {
    str = "1";
  } else if(str == "Everyone") {
    str = "1";
  } else if(str == "GA") {
    str = "1";
  } else if(str == "EC") {
    str = "3";
  } else if(str == "Early Childhood") {
    str = "3";
  } else if(str == "3+") {
    str = "3";
  } else if(str == "G") {
    str = "3";
  } else if(str == "KA") {
    str = "6";
  } else if(str == "Kids to Adults") {
    str = "6";
  } else if(str == "G8+") {
    str = "8";
  } else if(str == "E10+") {
    str = "10";
  } else if(str == "E10+ - Everyone 10+") {
    str = "10";
  } else if(str == "Everyone 10+") {
    str = "10";
  } else if(str == "11+") {
    str = "11";
  } else if(str == "12+") {
    str = "11";
  } else if(str == "MA-13") {
    str = "13";
  } else if(str == "T") {
    str = "13";
  } else if(str == "T - Teen") {
    str = "13";
  } else if(str == "Teen") {
    str = "13";
  } else if(str == "M") {
    str = "15";
  } else if(str == "M15+") {
    str = "15";
  } else if(str == "MA 15+") {
    str = "15";
  } else if(str == "MA15+") {
    str = "15";
  } else if(str == "PG") {
    str = "15";
  } else if(str == "15+") {
    str = "15";
  } else if(str == "MA-17") {
    str = "17";
  } else if(str == "M") {
    str = "17";
  } else if(str == "18+") {
    str = "18";
  } else if(str == "R18+") {
    str = "18";
  } else if(str == "18 (keine Jugendfreigabe)") {
    str = "18";
  } else if(str == "A") {
    str = "18";
  } else if(str == "AO") {
    str = "18";
  } else if(str == "AO - Adults Only") {
    str = "18";
  } else if(str == "Adults Only") {
    str = "18";
  } else if(str == "M - Mature") {
    str = "18";
  } else if(str == "Mature") {
    str = "18";
  }

  return str;
}

QString StrTools::conformReleaseDate(QString str)
{
  if(QRegularExpression("^\\d{4}$").match(str).hasMatch()) {
    str = QDate::fromString(str, "yyyy").toString("yyyyMMdd");
  } else if(str.length() == 7 &&
	    str.left(4).toInt() != 0 &&
	    str.left(4).toInt() < 3000 &&
	    str.mid(5, 2).toInt() != 0 &&
	    str.mid(5, 2).toInt() <= 12) {
    str = QDate::fromString(str, "yyyy-MM").toString("yyyyMMdd");
  } else if(str.length() == 10 &&
	    str.left(4).toInt() != 0 &&
	    str.left(4).toInt() < 3000 &&
	    str.mid(5, 2).toInt() != 0 &&
	    str.mid(5, 2).toInt() <= 12 &&
	    str.mid(8, 2).toInt() != 0 &&
	    str.mid(8, 2).toInt() <= 31) {
    str = QDate::fromString(str, "yyyy-MM-dd").toString("yyyyMMdd");
  } else if(str.length() == 10 &&
	    str.left(2).toInt() != 0 &&
	    str.left(2).toInt() <= 12 &&
	    str.mid(3, 2).toInt() != 0 &&
	    str.mid(3, 2).toInt() <= 31 &&
	    str.mid(6, 4).toInt() != 0 &&
	    str.mid(6, 4).toInt() < 3000) {
    str = QDate::fromString(str, "MM/dd/yyyy").toString("yyyyMMdd");
  } else if(str.length() == 9 &&
	    (str.left(3) == "Jan" ||
	     str.left(3) == "Feb" ||
	     str.left(3) == "Mar" ||
	     str.left(3) == "Apr" ||
	     str.left(3) == "May" ||
	     str.left(3) == "Jun" ||
	     str.left(3) == "Jul" ||
	     str.left(3) == "Aug" ||
	     str.left(3) == "Sep" ||
	     str.left(3) == "Oct" ||
	     str.left(3) == "Nov" ||
	     str.left(3) == "Dec") &&
	    str.mid(5, 4).toInt() != 0 &&
	    str.mid(5, 4).toInt() < 3000) {
    str = QDate::fromString(str, "MMM, yyyy").toString("yyyyMMdd");
  } else if(str.length() == 12 &&
	    (str.left(3) == "Jan" ||
	     str.left(3) == "Feb" ||
	     str.left(3) == "Mar" ||
	     str.left(3) == "Apr" ||
	     str.left(3) == "May" ||
	     str.left(3) == "Jun" ||
	     str.left(3) == "Jul" ||
	     str.left(3) == "Aug" ||
	     str.left(3) == "Sep" ||
	     str.left(3) == "Oct" ||
	     str.left(3) == "Nov" ||
	     str.left(3) == "Dec") &&
	    str.mid(4, 2).toInt() != 0 &&
	    str.mid(4, 2).toInt() <= 31 &&
	    str.mid(8, 4).toInt() != 0 &&
	    str.mid(8, 4).toInt() < 3000) {
    str = QDate::fromString(str, "MMM dd, yyyy").toString("yyyyMMdd");
  }
  return str;
}

QString StrTools::conformTags(QString str)
{
  QString tags = "";
  QList<QString> tagList = str.split(',', QString::SkipEmptyParts);
  foreach(QString tag, tagList) {
    tag = tag.simplified();
    tag = tag.left(1).toUpper() + tag.mid(1, tag.length() - 1);
    tags += tag.simplified() + ", ";
  }
  tags = tags.left(tags.length() - 2);
  return tags;
}

int StrTools::getNumeral(const QString &name)
{
  QRegularExpressionMatch match;
  int numeral = 1;
  
  // Check for roman numerals
  match = QRegularExpression(" [IVX]{1,5}([: ]+|$)").match(name);
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
  match = QRegularExpression("\\d+([: ]+|$)").match(name);
  if(match.hasMatch()) {
    QString intStr = match.captured(0).replace(":", "").simplified();
    if(intStr.toInt() != 0) {
      numeral = intStr.toInt();
    }
  }

  return numeral;
}

QString StrTools::getVersionHeader()
{
  QString headerString = "Running Skyscraper v" VERSION " by Lars Muldjord";
  QString dashesString = "";
  for(int a = 0; a < headerString.length(); ++a) {
    dashesString += "-";
  }

  return QString("\033[1;34m" + dashesString + "\033[0m\n\033[1;33m" + headerString + "\033[0m\n\033[1;34m" + dashesString + "\033[0m\n");
}
