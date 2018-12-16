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

#pragma once

#include "abstractscraper.h"
#include <QDomDocument>

class ESGameList: public AbstractScraper {
Q_OBJECT

public:
	ESGameList(Settings *config);

private:
	QList<QString> getSearchNames(const QFileInfo &info) override;
	void getSearchResults(QList<GameEntry> &gameEntries, QString searchName,
			QString platform) override;
	void getGameData(GameEntry &game) override;
	QImage loadImageData(const QDomNode& gamenode, QString tag);
	QString loadVideoData(const QDomNode& gamenode);
	QString checkName(QString value);

	QDomDocument doc;
	QString basePath;
	QString gamelistXml;
};
