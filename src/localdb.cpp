/***************************************************************************
 *            localdb.cpp
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

#include <QFile>
#include <QDir>
#include <QXmlStreamReader>
#include <QXmlStreamAttributes>
#include <QDateTime>
#include <QDomDocument>

#include "localdb.h"

LocalDb::LocalDb(const QString &dbFolder)
{
  dbDir = QDir(dbFolder);
}

bool LocalDb::createFolders(const QString &scraper)
{
  if(!dbDir.mkpath(dbDir.absolutePath() + "/covers/" + scraper)) {
    return false;
  }
  if(!dbDir.mkpath(dbDir.absolutePath() + "/screenshots/" + scraper)) {
    return false;
  }
  if(!dbDir.mkpath(dbDir.absolutePath() + "/videos/" + scraper)) {
    return false;
  }

  // Copy priorities.xml example file to db folder if it doesn't already exist
  if(!QFileInfo::exists(dbDir.absolutePath() + "/priorities.xml")) {
    QFile::copy("dbs/priorities.xml.example",
		dbDir.absolutePath() + "/priorities.xml");
  }
  
  return true;
}

bool LocalDb::readDb()
{
  bool result = false;

  QFile dbFile(dbDir.absolutePath() + "/db.xml");
  if(dbFile.open(QIODevice::ReadOnly)) {
    printf("Reading and parsing local database, please wait...\n");
    QXmlStreamReader xml(&dbFile);
    while(!xml.atEnd()) {
      if(xml.readNext() != QXmlStreamReader::StartElement) {
	continue;
      }
      if(xml.name() != "resource") {
	continue;
      }
      QXmlStreamAttributes attribs = xml.attributes();
      if(!attribs.hasAttribute("sha1")) {
	printf("Resource is missing 'sha1' attribute, skipping...\n");
	continue;
      }

      Resource resource;
      resource.sha1 = attribs.value("sha1").toString();

      if(attribs.hasAttribute("type")) {
	resource.type = attribs.value("type").toString();
      } else {
	printf("Resource with sha1 '%s' is missing 'type' attribute, skipping...\n",
	       resource.sha1.toStdString().c_str());
	continue;
      }
      if(attribs.hasAttribute("source")) {
	resource.source = attribs.value("source").toString();
      } else {
	resource.source = "generic";
      }
      if(attribs.hasAttribute("timestamp")) {
	resource.timestamp = attribs.value("timestamp").toULong();
      } else {
	printf("Resource with sha1 '%s' is missing 'timestamp' attribute, skipping...\n",
	       resource.sha1.toStdString().c_str());
	continue;
      }
      resource.value = xml.readElementText();
      if(resource.type == "cover" || resource.type == "screenshot" || resource.type == "video") {
	if(!QFileInfo::exists(dbDir.absolutePath() + "/" + resource.value)) {
	  printf("Data file is missing for %s resource with sha1 '%s', skipping...\n",
		 resource.type.toStdString().c_str(), resource.sha1.toStdString().c_str());
	  continue;
	}
      }

      resources.append(resource);
    }
    result = true;
    printf("Successfully parsed %d resources!\n\n", resources.length());
    dbFile.close();
  }
  return result;
}

void LocalDb::readPriorities()
{
  QDomDocument prioDoc;
  QFile prioFile(dbDir.absolutePath() + "/priorities.xml");
  printf("Looking for optional 'priorities.xml' file in local db folder... ");
  if(prioFile.open(QIODevice::ReadOnly)) {
    printf("Found!\n");
    if(!prioDoc.setContent(prioFile.readAll())) {
      printf("Document is not XML compliant, skipping...\n\n");
      return;
    }
  } else {
    printf("Not found, skipping...\n\n");
    return;
  }

  QDomNodeList orderNodes = prioDoc.elementsByTagName("order");

  int errors = 0;
  for(int a = 0; a < orderNodes.length(); ++a) {
    QDomElement orderElem = orderNodes.at(a).toElement();
    if(!orderElem.hasAttribute("type")) {
      printf("Priority 'order' node missing 'type' attribute, skipping...\n");
      errors++;
      continue;
    }
    QString type = orderElem.attribute("type");
    QList<QString> sources;
    QDomNodeList sourceNodes = orderNodes.at(a).childNodes();
    if(sourceNodes.isEmpty()) {
      printf("'source' node(s) missing for type '%s' in priorities.xml, skipping...\n",
	     type.toStdString().c_str());
      errors++;
      continue;
    }
    for(int b = 0; b < sourceNodes.length(); ++b) {
      sources.append(sourceNodes.at(b).toElement().text());
    }
    prioMap[type] = sources;
  }
  printf("Priorities loaded successfully");
  if(errors != 0) {
    printf(", but %d errors encountered, please check this", errors);
  }
  printf("!\n\n");
}

bool LocalDb::writeDb()
{
  bool result = false;

  QFile dbFile(dbDir.absolutePath() + "/db.xml");
  if(dbFile.open(QIODevice::WriteOnly)) {
    printf("Writing %d resources to local database, please wait... ", resources.length());
    QXmlStreamWriter xml(&dbFile);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement("resources");
    foreach(Resource resource, resources) {
      xml.writeStartElement("resource");
      xml.writeAttribute("sha1", resource.sha1);
      xml.writeAttribute("type", resource.type);
      xml.writeAttribute("source", resource.source);
      xml.writeAttribute("timestamp", QString::number(resource.timestamp));
      xml.writeCharacters(resource.value);
      xml.writeEndElement();
    }
    xml.writeEndDocument();
    result = true;
    printf("\033[1;32mSuccess!\033[0m\n");
    dbFile.close();
  }
  return result;
}

// This verifies all attached media files and deletes those that have no entry in the db
void LocalDb::cleanDb()
{
  // TODO: Add format checks for each resource type, and remove if deemed corrupt

  printf("Starting cleaning run on local database, please wait...\n");

  if(!QFileInfo::exists(dbDir.absolutePath() + "/db.xml")) {
    printf("'db.xml' not found, db cleaning cancelled...\n");
    return;
  }

  QDir coversDir(dbDir.absolutePath() + "/covers", "*.*", QDir::Name, QDir::Files);
  QDir screenshotsDir(dbDir.absolutePath() + "/screenshots", "*.*", QDir::Name, QDir::Files);
  QDir videosDir(dbDir.absolutePath() + "/videos", "*.*", QDir::Name, QDir::Files);

  QDirIterator coversDirIt(coversDir.absolutePath(),
			   QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks,
			   QDirIterator::Subdirectories);

  QDirIterator screenshotsDirIt(screenshotsDir.absolutePath(),
				QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks,
				QDirIterator::Subdirectories);

  QDirIterator videosDirIt(videosDir.absolutePath(),
			   QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks,
			   QDirIterator::Subdirectories);

  int deleted = 0;
  int noDelete = 0;

  verifyResources(coversDirIt, deleted, noDelete, "cover");
  verifyResources(screenshotsDirIt, deleted, noDelete, "screenshot");
  verifyResources(videosDirIt, deleted, noDelete, "video");
  
  if(deleted == 0 && noDelete == 0) {
    printf("No inconsistencies found in the database. :)\n\n");
  } else {
    printf("Successfully deleted %d files with no entry.\n", deleted);
    if(noDelete != 0) {
      printf("%d files couldn't be deleted, please check file permissions.\n", noDelete);
    }
    printf("\n");
  }
}

void LocalDb::verifyResources(QDirIterator &dirIt, int &deleted, int &noDelete, QString resType)
{
  while(dirIt.hasNext()) {
    QFileInfo fileInfo(dirIt.next());
    bool valid = false;
    foreach(Resource resource, resources) {
      if(resource.type == resType) {
	QFileInfo resInfo(dbDir.absolutePath() + "/" + resource.value);
	if(fileInfo.fileName() == resInfo.fileName()) {
	  valid = true;
	  break;
	}
      }
    }
    if(!valid) {
      printf("No resource entry for '%s', deleting... ",
	     fileInfo.fileName().toStdString().c_str());
      if(QFile::remove(fileInfo.absoluteFilePath())) {
	printf("OK!\n");
	deleted++;
      } else {
	printf("ERROR! File couldn't be deleted :/\n");
	noDelete++;
      }
    }
  }
}

void LocalDb::mergeDb(LocalDb &srcDb, bool overwrite, const QString &srcDbFolder)
{
  printf("Merging databases, please wait...\n");
  QList<Resource> srcResources = srcDb.getResources();

  QDir srcDbDir(srcDbFolder);
  
  int resUpdated = 0;
  int resMerged = 0;

  foreach(Resource srcResource, srcResources) {
    bool resExists = false;
    for(int a = 0; a < resources.length(); ++a) {
      if(resources.at(a).sha1 == srcResource.sha1 && resources.at(a).type == srcResource.type) {
	if(overwrite) {
	  resources.removeAt(a);
	} else {
	  resExists = true;
	}
      }
    }
    if(!resExists) {
      if(srcResource.type == "cover" || srcResource.type == "screenshot" ||
	 srcResource.type == "video") {
	if(!QFile::copy(srcDbDir.absolutePath() + "/" + srcResource.value,
			dbDir.absolutePath() + "/" + srcResource.value)) {
	  continue;
	}
      }
      if(overwrite) {
	resUpdated++;
      } else {
	resMerged++;
      }
      resources.append(srcResource);
    }
  }
  printf("Successfully updated %d resource(s) in local database!\n", resUpdated);
  printf("Successfully merged %d resource(s) into local database!\n\n", resMerged);
}

QList<Resource> LocalDb::getResources()
{
  return resources;
}
    
void LocalDb::addResources(GameEntry &entry, const bool &update)
{
  QString dbAbsolutePath = dbDir.absolutePath();

  if(entry.source.isEmpty()) {
    printf("Something is wrong, resource with sha1 '%s' has no source, exiting...\n",
	   entry.sha1.toStdString().c_str());
    exit(1);
  }
  
  if(entry.sha1 != "") {
    Resource resource;
    resource.sha1 = entry.sha1;
    resource.source = entry.source;
    resource.timestamp = QDateTime::currentDateTime().toMSecsSinceEpoch();
    if(entry.title != "") {
      resource.type = "title";
      resource.value = entry.title;
      addResource(resource, entry, dbAbsolutePath, update);
    }
    if(entry.platform != "") {
      resource.type = "platform";
      resource.value = entry.platform;
      addResource(resource, entry, dbAbsolutePath, update);
    }
    if(entry.description != "") {
      resource.type = "description";
      resource.value = entry.description;
      addResource(resource, entry, dbAbsolutePath, update);
    }
    if(entry.publisher != "") {
      resource.type = "publisher";
      resource.value = entry.publisher;
      addResource(resource, entry, dbAbsolutePath, update);
    }
    if(entry.developer != "") {
      resource.type = "developer";
      resource.value = entry.developer;
      addResource(resource, entry, dbAbsolutePath, update);
    }
    if(entry.players != "") {
      resource.type = "players";
      resource.value = entry.players;
      addResource(resource, entry, dbAbsolutePath, update);
    }
    if(entry.tags != "") {
      resource.type = "tags";
      resource.value = entry.tags;
      addResource(resource, entry, dbAbsolutePath, update);
    }
    if(entry.rating != "") {
      resource.type = "rating";
      resource.value = entry.rating;
      addResource(resource, entry, dbAbsolutePath, update);
    }
    if(entry.releaseDate != "") {
      resource.type = "releasedate";
      resource.value = entry.releaseDate;
      addResource(resource, entry, dbAbsolutePath, update);
    }
    if(entry.videoData != "" && entry.videoFormat != "") {
      resource.type = "video";
      resource.value = "videos/" + entry.source + "/"  + entry.sha1 + "." + entry.videoFormat;
      addResource(resource, entry, dbAbsolutePath, update);
    }
    if(!entry.coverData.isNull()) {
      resource.type = "cover";
      resource.value = "covers/" + entry.source + "/" + entry.sha1 + ".png";
      addResource(resource, entry, dbAbsolutePath, update);
    }
    if(!entry.screenshotData.isNull()) {
      resource.type = "screenshot";
      resource.value = "screenshots/" + entry.source + "/"  + entry.sha1 + ".png";
      addResource(resource, entry, dbAbsolutePath, update);
    }
  }
}

void LocalDb::addResource(const Resource &resource, GameEntry &entry,
			  const QString &dbAbsolutePath, const bool &update)
{
  QMutexLocker locker(&dbMutex);

  bool notFound = true;
  for(int a = 0; a < resources.length(); ++a) {
    if(resources.at(a).sha1 == resource.sha1 &&
       resources.at(a).type == resource.type &&
       resources.at(a).source == resource.source) {
      if(update) {
	resources.removeAt(a);
      } else {
	notFound = false;
      }
      break;
    }
  }
  if(notFound) {
    bool okToAppend = true;
    if(resource.type == "cover") {
      // Restrict size of cover to save space
      if(entry.coverData.height() >= 512) {
	entry.coverData = entry.coverData.scaledToHeight(512, Qt::SmoothTransformation);
      }
      if(!entry.coverData.save(dbAbsolutePath + "/" + resource.value)) {
	okToAppend = false;
      }
    } else if(resource.type == "screenshot") {
      // Restrict size of screenshot to save space
      if(entry.screenshotData.width() >= 640) {
	entry.screenshotData = entry.screenshotData.scaledToWidth(640, Qt::SmoothTransformation);
      }
      if(!entry.screenshotData.save(dbAbsolutePath + "/" + resource.value)) {
	okToAppend = false;
      }
    } else if(resource.type == "video") {
      QFile videoFile(dbAbsolutePath + "/" + resource.value);
      if(videoFile.open(QIODevice::WriteOnly)) {
	videoFile.write(entry.videoData);
	videoFile.close();
      } else {
	okToAppend = false;
      }
    }

    if(okToAppend) {
      resources.append(resource);
    }
    
  }
}

bool LocalDb::hasSha1(const QString &sha1)
{
  // Copy data before iterating to make thread safe
  QList<Resource> tmpResources = resources;
  foreach(Resource res, tmpResources) {
    if(res.sha1 == sha1) {
      return true;
    }
  }
  return false;
}

void LocalDb::fillBlanks(GameEntry &entry)
{
  // Copy data before iterating to make thread safe
  QList<Resource> tmpResources = resources;
  QList<Resource> sha1Resources;
  // Find all resources related to this particular rom
  foreach(Resource resource, tmpResources) {
    if(entry.sha1 == resource.sha1) {
      sha1Resources.append(resource);
    }
  }

  {
    QString type = "title";
    QString result = "";
    if(fillType(type, sha1Resources, result)) {
      entry.title = result;
    }
  }
  {
    QString type = "platform";
    QString result = "";
    if(fillType(type, sha1Resources, result)) {
      entry.platform = result;
    }
  }
  {
    QString type = "description";
    QString result = "";
    if(fillType(type, sha1Resources, result)) {
      entry.description = result;
    }
  }
  {
    QString type = "publisher";
    QString result = "";
    if(fillType(type, sha1Resources, result)) {
      entry.publisher = result;
    }
  }
  {
    QString type = "developer";
    QString result = "";
    if(fillType(type, sha1Resources, result)) {
      entry.developer = result;
    }
  }
  {
    QString type = "players";
    QString result = "";
    if(fillType(type, sha1Resources, result)) {
      entry.players = result;
    }
  }
  {
    QString type = "tags";
    QString result = "";
    if(fillType(type, sha1Resources, result)) {
      entry.tags = result;
    }
  }
  {
    QString type = "rating";
    QString result = "";
    if(fillType(type, sha1Resources, result)) {
      entry.rating = result;
    }
  }
  {
    QString type = "releasedate";
    QString result = "";
    if(fillType(type, sha1Resources, result)) {
      entry.releaseDate = result;
    }
  }
  {
    QString type = "cover";
    QString result = "";
    if(fillType(type, sha1Resources, result)) {
      entry.coverData = QImage(dbDir.absolutePath() + "/" + result);
    }
  }
  {
    QString type = "screenshot";
    QString result = "";
    if(fillType(type, sha1Resources, result)) {
      entry.screenshotData = QImage(dbDir.absolutePath() + "/" + result);
    }
  }
  {
    QString type = "video";
    QString result = "";
    if(fillType(type, sha1Resources, result)) {
      QFileInfo info(dbDir.absolutePath() + "/" + result);
      QFile videoFile(info.absoluteFilePath());
      if(videoFile.open(QIODevice::ReadOnly)) {
	entry.videoData = videoFile.readAll();
	entry.videoFormat = info.suffix();
	videoFile.close();
      }
    }
  }
}

bool LocalDb::fillType(QString &type, QList<Resource> &sha1Resources, QString &result)
{
  QList<Resource> typeResources;
  foreach(Resource resource, sha1Resources) {
    if(resource.type == type) {
      typeResources.append(resource);
    }
  }
  if(typeResources.isEmpty()) {
    return false;
  }
  if(prioMap.contains(type)) {
    for(int a = 0; a < prioMap.value(type).length(); ++a) {
      foreach(Resource resource, typeResources) {
	if(resource.source == prioMap.value(type).at(a)) {
	  result = resource.value;
	  return true;
	}
      }
    }
  }
  qint64 newest = 0;
  foreach(Resource resource, typeResources) {
    if(resource.timestamp >= newest) {
      newest = resource.timestamp;
      result = resource.value;
    }
  }  
  
  return true;
}


void LocalDb::printResources()
{
  foreach(Resource resource, resources) {
    printf("--- sha1: '%s' ---\ntype: '%s'\nsource: '%s'\ntimestamp: '%s'\nvalue: '%s'\n", resource.sha1.toStdString().c_str(), resource.type.toStdString().c_str(), resource.source.toStdString().c_str(), QString::number(resource.timestamp).toStdString().c_str(), resource.value.toStdString().c_str());
  }
}
