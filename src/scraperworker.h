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
#include "cache.h"
#include "queue.h"

class ScraperWorker : public QObject
{
  Q_OBJECT

public:
  ScraperWorker(QSharedPointer<Queue> queue, QSharedPointer<Cache> cache,
		Settings config, QString threadId);
  ~ScraperWorker();
  void run();
  bool forceEnd = false;
  
signals:
  void allDone();
  void entryReady(GameEntry entry, QString output, QString debug);
  
private:
  AbstractScraper *scraper;

  QSharedPointer<Cache> cache;
  QSharedPointer<Queue> queue;
  
  Settings config;
  QString platformOrig;
  QString threadId;

  unsigned int editDistance(const std::string& s1, const std::string& s2);

  GameEntry getBestEntry(const QList<GameEntry> &gameEntries, QString compareTitle,
			 int &lowestDistance);
  GameEntry getEntryFromUser(const QList<GameEntry> &gameEntries, const GameEntry &suggestedGame,
			     const QString &compareTitle, int &lowestDistance);
  int getSearchMatch(const QString &title, const QString &compareTitle, const int &lowestDistance);

  bool limitReached(QString &output);
};

#endif // SCRAPERWORKER_H
