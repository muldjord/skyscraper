/***************************************************************************
 *            scraperworker.h
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

#ifndef SCRAPERWORKER_H
#define SCRAPERWORKER_H

#include <QImage>
#include <QDir>
#include <QThread>

#include "abstractscraper.h"
#include "settings.h"
#include "localdb.h"

class ScraperWorker : public QObject
{
  Q_OBJECT

public:
  ScraperWorker(QList<QFileInfo> inputFiles, int filesPerThreads, int beginIdx,
		Settings config, QSharedPointer<LocalDb> localDb);
  ~ScraperWorker();
  void run();
  
signals:
  void allDone();
  void entryReady(const GameEntry &entry);
  void outputToTerminal(const QString &text);

private:
  QSharedPointer<LocalDb> localDb;
  
  Settings config;
  QString platformOrig;
  QString output;
  QList<QFileInfo> inputFiles;
  int filesPerThread;
  int beginIdx;
  
  unsigned int editDistance(const std::string& s1, const std::string& s2);
  void nomNom(QByteArray &data, const QString nom, bool including = true);

  void getSearchResults(QList<GameEntry> &gameEntries, QString searchName, QString platform);
  void getGameData(GameEntry &game);
  QString assembleTags(QByteArray &data);
  QString getSha1(const QFileInfo &info);
  GameEntry getBestEntry(const QList<GameEntry> &gameEntries, const QString &compareName,
			 int &lowestDistance);
  int getSearchMatch(const QString &title, const QString &compareName, const int &lowestDistance);
};

#endif // SCRAPERWORKER_H
