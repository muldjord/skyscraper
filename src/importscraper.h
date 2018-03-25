/***************************************************************************
 *            importscraper.h
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

#ifndef IMPORTSCRAPER_H
#define IMPORTSCRAPER_H

#include "abstractscraper.h"

class ImportScraper : public AbstractScraper
{
  Q_OBJECT

public:
  ImportScraper();
  void runPasses(QList<GameEntry> &gameEntries, const QFileInfo &info, QString &, QString &, QString &);
  void getGameData(GameEntry &game);
  QString getCompareTitle(QFileInfo info, QString &, QString &);
  void getCover(GameEntry &game);
  void getScreenshot(GameEntry &game);
  void getWheel(GameEntry &game);
  void getMarquee(GameEntry &game);
  void getVideo(GameEntry &game);

private:
  bool checkType(QString baseName, QList<QFileInfo> &infos, QString &inputFile);
  bool loadDefinitions();
  void loadData();
  void checkForTag(QList<QString> &pre, QString &post, QString &tag, QString &line);

  QString descriptionTag = "###DESCRIPTION###";
  QString developerTag = "###DEVELOPER###";
  QString publisherTag = "###PUBLISHER###";
  QString playersTag = "###PLAYERS###";
  QString ratingTag = "###RATING###";
  QString tagsTag = "###TAGS###";
  QString releaseDateTag = "###RELEASEDATE###";
  
  QList<QFileInfo> textual;
  QList<QFileInfo> covers;
  QList<QFileInfo> screenshots;
  QList<QFileInfo> wheels;
  QList<QFileInfo> marquees;
  QList<QFileInfo> videos;
  QString textualFile = "";
  QString coverFile = "";
  QString screenshotFile = "";
  QString wheelFile = "";
  QString marqueeFile = "";
  QString videoFile = "";
};

#endif // IMPORTSCRAPER_H
