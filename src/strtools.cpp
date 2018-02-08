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

QByteArray StrTools::unMagic(QByteArray str)
{
  QByteArray magicStr("WowMuchEncrypted");

  int strChars[str.length()];
  int magicChars[str.length()];

  for(int a = 0; a < magicStr.length(); ++a) {
    strChars[a] = str.split(';').at(a).toInt();
  }
  for(int a = 0; a < magicStr.length(); ++a) {
    magicChars[a] = magicStr.at(a);
  }
  QByteArray thingie;
  for(int a = 0; a < magicStr.length(); ++a) {
    thingie.append(QChar(strChars[a] -= magicChars[a]));
  }

  return thingie; 
}

QString StrTools::conformPlayers(QString str)
{
  if(QRegularExpression("^\\d-\\d\\d").match(str).hasMatch())
    return str.mid(2, 2);

  if(QRegularExpression("^\\d-\\d").match(str).hasMatch())
    return str.mid(2, 1);

  if(QRegularExpression("^\\d - \\d\\d").match(str).hasMatch())
    return str.mid(4, 2);

  if(QRegularExpression("^\\d - \\d").match(str).hasMatch())
    return str.mid(4, 1);

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
