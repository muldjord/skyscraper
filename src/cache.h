/***************************************************************************
 *            cache.h
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

#ifndef CACHE_H
#define CACHE_H

#include <QObject>
#include <QString>
#include <QMutex>
#include <QDirIterator>
#include <QMap>
#include <QSharedPointer>

#include "gameentry.h"
#include "queue.h"
#include "settings.h"

struct Resource {
  QString cacheId = "";
  int version = 1;
  QString type = "";
  QString source = "";
  QString value = "";
  qint64 timestamp = 0;
};

struct ResCounts {
  int titles;
  int platforms;
  int descriptions;
  int publishers;
  int developers;
  int players;
  int ages;
  int tags;
  int ratings;
  int releaseDates;
  int covers;
  int screenshots;
  int wheels;
  int marquees;
  int videos;
};

class Cache
{
public:
  Cache(const QString &cacheFolder);
  bool createFolders(const QString &scraper);
  bool read();
  void purgeResources(QString purgeStr);
  void printPriorities(QString cacheId);
  void editResources(QSharedPointer<Queue> queue);
  void purgeAll(const bool unattend = false);
  void vacuumResources(const QString inputFolder, const QString filters,
		       const int verbosity, const bool unattend = false);
  void assembleReport(const QString inputFolder, const QString filters, QString platform, bool subdirs = true, QString reportStr = "");
  void showStats(int verbosity);
  void readPriorities();
  bool write();
  void validate();
  void addResources(GameEntry &entry, const Settings &config);
  void fillBlanks(GameEntry &entry, const QString scraper = "");
  bool hasEntries(const QString &cacheId, const QString scraper = "");
  void addQuickId(const QFileInfo &info, const QString &cacheId);
  QString getQuickId(const QFileInfo &info);
  void merge(Cache &mergeCache, bool overwrite, const QString &mergeCacheFolder);
  QList<Resource> getResources();

 private:
  QDir cacheDir;
  QMutex cacheMutex;
  QMutex quickIdMutex;

  QMap<QString, QList<QString> > prioMap;

  QMap<QString, ResCounts> resCountsMap;
  
  QList<Resource> resources;
  QMap<QString, QPair<qint64, QString> > quickIds; // filePath, timestamp + cacheId for quick lookup
  
  QList<QFileInfo> getFileInfos(const QString &inputFolder, const QString &filter, const bool subdirs = true);
  QList<QString> getCacheIdList(const QList<QFileInfo> &fileInfos);

  void addToResCounts(const QString source, const QString type);
  void addResource(const Resource &resource, GameEntry &entry, const QString &cacheAbsolutePath,
		   const Settings &config);
  void verifyFiles(QDirIterator &dirIt, int &filesDeleted, int &noDelete, QString resType);
  void verifyResources(int &resourcesDeleted);
  bool fillType(QString &type, QList<Resource> &matchingResources,
		QString &result, QString &source);

  int resAtLoad = 0;
};

#endif // CACHE_H
