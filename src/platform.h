/***************************************************************************
 *            platform.h
 *
 *  Sat Dec 23 10:00:00 CEST 2017
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

#ifndef PLATFORM_H
#define PLATFORM_H

#include <QObject>
#include <QDir>

class Platform : public QObject
{
  Q_OBJECT

public:
  Platform();
  ~Platform();

  static QStringList getScrapers(QString platform);
  static QString getFormats(QString platform);
  static QString getDefaultScraper(QString platform);
  static QStringList getAliases(QString platform);
  static QStringList getPlatforms();

private:
  
};

#endif // PLATFORM_H
