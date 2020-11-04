/***************************************************************************
 *            esgamelist.h
 *
 *  Mon Dec 17 08:00:00 CEST 2018
 *  Copyright 2018 Lars Muldjord & Martin Gerhardy
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

#ifndef ESGAMELIST_H
#define ESGAMELIST_H

#include "abstractscraper.h"
#include <QDomDocument>

class ESGameList: public AbstractScraper {
Q_OBJECT

public:
  ESGameList(Settings *config, QSharedPointer<NetManager> manager);

private:
  QList<QString> getSearchNames(const QFileInfo &info) override;
  void getSearchResults(QList<GameEntry> &gameEntries, QString searchName,
			QString platform) override;
  void getGameData(GameEntry &game) override;
  QByteArray loadImageData(const QString fileName);
  void loadVideoData(GameEntry &game, const QString fileName);
  QString getAbsoluteFileName(const QString fileName);

  QDomNodeList games;
  QDomNode gameNode;
};

#endif // ESGAMELIST_H
