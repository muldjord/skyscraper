/***************************************************************************
 *            abstractfrontend.cpp
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

#include "abstractfrontend.h"

AbstractFrontend::AbstractFrontend()
{
}

AbstractFrontend::~AbstractFrontend()
{
}

void AbstractFrontend::setConfig(Settings *config)
{
  this->config = config;
}

bool AbstractFrontend::canSkip()
{
  return false;
}

bool AbstractFrontend::loadOldGameList(const QString &)
{
  return false;
}

QString AbstractFrontend::getGameListFileName()
{
  return QString();
}

QString AbstractFrontend::getInputFolder()
{
  return QString();
}

QString AbstractFrontend::getGameListFolder()
{
  return QString();
}

QString AbstractFrontend::getCoversFolder()
{
  return QString();
}

QString AbstractFrontend::getScreenshotsFolder()
{
  return QString();
}

QString AbstractFrontend::getWheelsFolder()
{
  return QString();
}

QString AbstractFrontend::getMarqueesFolder()
{
  return QString();
}

QString AbstractFrontend::getVideosFolder()
{
  return QString();
}

void AbstractFrontend::sortEntries(QList<GameEntry> &gameEntries)
{
  qSort(gameEntries.begin(), gameEntries.end(), 
	[](const GameEntry a, const GameEntry b) -> bool { return a.title.toLower() < b.title.toLower(); });
}
