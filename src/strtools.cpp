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
