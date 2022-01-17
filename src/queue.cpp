/***************************************************************************
 *            queue.cpp
 *
 *  Fri Mar 9 12:00:00 CEST 2018
 *  Copyright 2018 Lars Muldjord
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

#include "queue.h"

#include <QRegularExpression>

Queue::Queue()
{
}

bool Queue::hasEntry()
{
  queueMutex.lock();
  if(isEmpty()) {
    queueMutex.unlock();
    return false;
  }
  return true;
}

QFileInfo Queue::takeEntry()
{
  QFileInfo info = first();
  removeFirst();
  queueMutex.unlock();
  return info;
}

void Queue::clearAll()
{
  queueMutex.lock();
  clear();
  queueMutex.unlock();
}

void Queue::filterFiles(const QString &patterns, const bool &include)
{
  QList<QString> regExpPatterns = getRegExpPatterns(patterns);

  queueMutex.lock();
  QMutableListIterator<QFileInfo> it(*this);
  while(it.hasNext()) {
    QFileInfo info = it.next();
    bool match = false;
    for(const auto &regExpPattern: regExpPatterns) {
      if(QRegularExpression(regExpPattern).match(info.fileName()).hasMatch()) {
	match = true;
      }
    }
    if(match && !include) {
      it.remove();
    } else if(!match && include) {
      it.remove();
    }
  }
  queueMutex.unlock();
}

void Queue::removeFiles(const QList<QString> &files)
{
  queueMutex.lock();
  QMutableListIterator<QFileInfo> it(*this);
  while(it.hasNext()) {
    QFileInfo info = it.next();
    for(const auto &file: files) {
      if(info.absoluteFilePath() == file) {
	it.remove();
	break;
      }
    }
  }
  queueMutex.unlock();
}

QList<QString> Queue::getRegExpPatterns(QString patterns)
{
  patterns.replace("\\,", "###COMMA###");
  patterns.replace(",", ";");

  QList<QString> regExpPatterns;

  for(auto regExpPattern: patterns.split(";")) {
    regExpPattern.replace("###COMMA###", ",");
    regExpPattern = QRegularExpression::escape(regExpPattern);
    regExpPattern.replace("\\*", ".*");
    regExpPattern.prepend("^");
    regExpPattern.append("$");
    regExpPatterns.append(regExpPattern);
  }

  return regExpPatterns;
}
