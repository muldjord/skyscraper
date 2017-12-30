/***************************************************************************
 *            localdb.h
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

#ifndef LOCALDB_H
#define LOCALDB_H

#include <QObject>
#include <QString>
#include <QMutex>
#include <QDirIterator>
#include <QMap>

#include "gameentry.h"

struct Resource {
  QString sha1 = "";
  QString type = "";
  QString source = "";
  QString value = "";
  qint64 timestamp = 0;
};

class LocalDb : public QObject
{
  Q_OBJECT

public:
  LocalDb(const QString &dbFolder);
  bool createFolders(const QString &scraper);
  bool readDb();
  void readPriorities();
  bool writeDb();
  void cleanDb();
  void addResources(GameEntry &entry, const bool &update, const bool &noResize);
  void fillBlanks(GameEntry &entry);
  void printResources();
  bool hasSha1(const QString &sha1);
  void mergeDb(LocalDb &srcDb, bool overwrite, const QString &srcDbFolder);
  QList<Resource> getResources();

 private:
  QDir dbDir;
  QMutex dbMutex;

  QMap<QString, QList<QString> > prioMap;
  
  QList<Resource> resources;
  void addResource(const Resource &resource, GameEntry &entry, const QString &dbAbsolutePath, const bool &update, const bool &noResize);
  void verifyResources(QDirIterator &dirIt, int &deleted, int &noDelete, QString resType);
  bool fillType(QString &type, QList<Resource> &sha1Resources, QString &result);
  
};

#endif // LOCALDB_H
