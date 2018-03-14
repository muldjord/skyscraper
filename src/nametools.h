/***************************************************************************
 *            nametools.h
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

#ifndef NAMETOOLS_H
#define NAMETOOLS_H

#include <QObject>
#include <QMap>

class NameTools : public QObject
{
public:
  static QString getMameName(const QString &baseName, const QMap<QString, QString> &mameMap);
  static QString getScummName(const QString &baseName);
  static QString getNameWithSpaces(const QString &baseName);
  
};

#endif // NAMETOOLS_H
