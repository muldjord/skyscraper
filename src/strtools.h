/***************************************************************************
 *            strtools.h
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

#ifndef STRTOOLS_H
#define STRTOOLS_H

#include <QObject>

class StrTools : public QObject
{
public:
  static QString xmlUnescape(QString str);
  static QString xmlEscape(QString str);
  static QByteArray magic(const QByteArray str);
  static QByteArray unMagic(const QByteArray str);
  static QString conformPlayers(const QString str);
  static QString conformAges(QString str);
  static QString conformReleaseDate(QString str);
  static QString conformTags(const QString str);
  static QString getVersionHeader();
  static QString stripBrackets(const QString str);
  static QString stripHtmlTags(QString str);
  static QString getMd5Sum(const QByteArray &data);
  
};

#endif // STRTOOLS_H
