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

void AbstractFrontend::sortEntries(QList<GameEntry> &gameEntries)
{
  printf("Sorting entries...");
  int dots = 0;
  std::sort(gameEntries.begin(), gameEntries.end(),
	[&dots](const GameEntry a, const GameEntry b) -> bool {
	  if(dots % 1000 == 0) {
	    printf(".");
	    fflush(stdout);
	  }
	  dots++;
	  QString firstTitle = a.title.toLower();
	  QString secondTitle = b.title.toLower();
	  if(firstTitle.left(4) == "the ") {
	    firstTitle.remove(0, 4);
	  }
	  if(secondTitle.left(4) == "the ") {
	    secondTitle.remove(0, 4);
	  }

	  return firstTitle < secondTitle;
	});
  printf(" \033[1;32mDone!\033[0m\n");
}
