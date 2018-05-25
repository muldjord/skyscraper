/***************************************************************************
 *            abstractscraper.h
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

#ifndef ABSTRACTSCRAPER_H
#define ABSTRACTSCRAPER_H

#include <QImage>
#include <QEventLoop>
#include <QFileInfo>
#include <QSettings>

#include "netcomm.h"
#include "gameentry.h"
#include "settings.h"

class AbstractScraper : public QObject
{
  Q_OBJECT

public:
  AbstractScraper();
  virtual ~AbstractScraper();
  virtual void getGameData(GameEntry &game);
  virtual QString getSearchName(QFileInfo info);
  virtual QString getCompareTitle(QFileInfo info, QString &sqrNotes, QString &parNotes);
  virtual void runPasses(QList<GameEntry> &gameEntries, const QFileInfo &info, QString &output, QString &marking, QString &debug);

  void setConfig(Settings *config);
  void setRegionPrios();
  void setLangPrios();
  void loadMameMap();
  
protected:
  Settings *config;
  QList<QString> regionPrios;
  QList<QString> langPrios;
  
  virtual void getSearchResults(QList<GameEntry> &gameEntries, QString searchName,
				QString platform);
  virtual void getDescription(GameEntry &game);
  virtual void getDeveloper(GameEntry &game);
  virtual void getPublisher(GameEntry &game);
  virtual void getPlayers(GameEntry &game);
  virtual void getAges(GameEntry &game);
  virtual void getTags(GameEntry &game);
  virtual void getRating(GameEntry &game);
  virtual void getReleaseDate(GameEntry &game);
  virtual void getCover(GameEntry &game);
  virtual void getScreenshot(GameEntry &game);
  virtual void getWheel(GameEntry &game);
  virtual void getMarquee(GameEntry &game);
  virtual void getVideo(GameEntry &game);

  virtual void nomNom(const QString nom, bool including = true);

  virtual bool platformMatch(QString found, QString platform);

  bool checkNom(const QString nom);

  QList<int> fetchOrder;

  QByteArray data;

  QString baseUrl;
  QString searchUrlPre;
  QString searchUrlPost;

  QString searchResultPre;

  QList<QString> urlPre;
  QString urlPost;
  QList<QString> titlePre;
  QString titlePost;
  QList<QString> platformPre;
  QString platformPost;
  QList<QString> descriptionPre;
  QString descriptionPost;
  QList<QString> developerPre;
  QString developerPost;
  QList<QString> publisherPre;
  QString publisherPost;
  QList<QString> playersPre;
  QString playersPost;
  QList<QString> agesPre;
  QString agesPost;
  QList<QString> tagsPre;
  QString tagsPost;
  QList<QString> ratingPre;
  QString ratingPost;
  QList<QString> releaseDatePre;
  QString releaseDatePost;
  QList<QString> coverPre;
  QString coverPost;
  QList<QString> screenshotPre;
  QString screenshotPost;
  QString screenshotCounter;
  QList<QString> wheelPre;
  QString wheelPost;
  QList<QString> marqueePre;
  QString marqueePost;
  QList<QString> videoPre;
  QString videoPost;

  NetComm manager;
  QEventLoop q; // Event loop for use when waiting for data from NetComm.

private:
  QMap<QString, QString> mameMap;
  
};

#endif // ABSTRACTSCRAPER_H
