/***************************************************************************
 *            cache.cpp
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

#include <iostream>

#include <QFile>
#include <QDir>
#include <QXmlStreamReader>
#include <QXmlStreamAttributes>
#include <QDateTime>
#include <QDomDocument>

#include "cache.h"
#include "nametools.h"

Cache::Cache(const QString &cacheFolder)
{
  cacheDir = QDir(cacheFolder);
}

bool Cache::createFolders(const QString &scraper)
{
  if(scraper != "cache") {
    if(!cacheDir.mkpath(cacheDir.absolutePath() + "/covers/" + scraper)) {
      return false;
    }
    if(!cacheDir.mkpath(cacheDir.absolutePath() + "/screenshots/" + scraper)) {
      return false;
    }
    if(!cacheDir.mkpath(cacheDir.absolutePath() + "/wheels/" + scraper)) {
      return false;
    }
    if(!cacheDir.mkpath(cacheDir.absolutePath() + "/marquees/" + scraper)) {
      return false;
    }
    if(!cacheDir.mkpath(cacheDir.absolutePath() + "/videos/" + scraper)) {
      return false;
    }
  }

  // Copy priorities.xml example file to cache folder if it doesn't already exist
  if(!QFileInfo::exists(cacheDir.absolutePath() + "/priorities.xml")) {
    QFile::copy("cache/priorities.xml.example",
		cacheDir.absolutePath() + "/priorities.xml");
  }
  
  return true;
}

bool Cache::read()
{
  QFile cacheFile(cacheDir.absolutePath() + "/db.xml");
  if(cacheFile.open(QIODevice::ReadOnly)) {
    printf("Reading and parsing resource cache, please wait...\n");
    QXmlStreamReader xml(&cacheFile);
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

      if(attribs.hasAttribute("source")) {
	resource.source = attribs.value("source").toString();
      } else {
	resource.source = "generic";
      }
      if(attribs.hasAttribute("type")) {
	resource.type = attribs.value("type").toString();
	addToResCounts(resource.source, resource.type);
      } else {
	printf("Resource with sha1 '%s' is missing 'type' attribute, skipping...\n",
	       resource.sha1.toStdString().c_str());
	continue;
      }
      if(attribs.hasAttribute("timestamp")) {
	resource.timestamp = attribs.value("timestamp").toULongLong();
      } else {
	printf("Resource with sha1 '%s' is missing 'timestamp' attribute, skipping...\n",
	       resource.sha1.toStdString().c_str());
	continue;
      }
      resource.value = xml.readElementText();
      if(resource.type == "cover" || resource.type == "screenshot" ||
	 resource.type == "wheel" || resource.type == "marquee" ||
	 resource.type == "video") {
	if(!QFileInfo::exists(cacheDir.absolutePath() + "/" + resource.value)) {
	  printf("Source file '%s' missing, skipping entry...\n",
		 resource.value.toStdString().c_str());
	  continue;
	}
      }

      resources.append(resource);
    }
    cacheFile.close();
    resAtLoad = resources.length();
    printf("Successfully parsed %d resources!\n\n", resources.length());
    return true;
  }
  return false;
}

void Cache::purgeResources(QString purgeStr)
{
  purgeStr.replace("purge:", "");
  printf("Purging requested resources from cache, please wait...\n");

  QString module = "";
  QString type = "";

  QList<QString> definitions = purgeStr.split(",");
  foreach(QString definition, definitions) {
    if(definition.left(2) == "m=") {
      module = definition.remove(0,2);
      printf("Module: '%s'\n", module.toStdString().c_str());
    }
	if(definition.left(2) == "t=") {
      type = definition.remove(0,2);
      printf("Type: '%s'\n", type.toStdString().c_str());
    }
  }

  int purged = 0;

  QMutableListIterator<Resource> it(resources);
  while(it.hasNext()) {
    Resource res = it.next();
    bool remove = false;
    if(module.isEmpty() && res.type == type) {
      remove = true;
    } else if(type.isEmpty() && res.source == module) {
      remove = true;
    } else if(res.source == module && res.type == type) {
      remove = true;
    }
    if(remove) {
      if(res.type == "cover" || res.type == "screenshot" ||
	 res.type == "wheel" || res.type == "marquee" ||
	 res.type == "video") {
	if(!QFile::remove(cacheDir.absolutePath() + "/" + res.value)) {
	  printf("Couldn't purge media file '%s', skipping...\n", res.value.toStdString().c_str());
	  continue;
	}
      }
      it.remove();
      purged++;
    }
  }
  printf("Successfully purged %d resources from the cache.\n", purged);
}

void Cache::purgeAll(const bool unattend)
{
  if(!unattend) {
    printf("\033[1;31mWARNING!!! You are about to purge / remove ALL resources from the Skyscaper cache connected to the currently selected platform. THIS CANNOT BE UNDONE!\033[0m\n\n");
    printf("\033[1;34mDo you wish to continue\033[0m (y/N)? ");
    std::string userInput = "";
    getline(std::cin, userInput);
    if(userInput != "y") {
      printf("User chose not to continue, cancelling purge...\n\n");
      return;
    }
  }

  printf("Purging ALL resources for the selected platform, please wait...");

  int purged = 0;
  int dots = 0;
  // Always make dotMod at least 1 or it will give "floating point exception" when modulo
  int dotMod = resources.size() * 0.1 + 1;
  
  QMutableListIterator<Resource> it(resources);
  while(it.hasNext()) {
    if(dots % dotMod == 0) {
      printf(".");
      fflush(stdout);
    }
    dots++;
    Resource res = it.next();
    if(res.type == "cover" || res.type == "screenshot" ||
       res.type == "wheel" || res.type == "marquee" ||
       res.type == "video") {
      if(!QFile::remove(cacheDir.absolutePath() + "/" + res.value)) {
	printf("Couldn't purge media file '%s', skipping...\n", res.value.toStdString().c_str());
	continue;
      }
    }
    it.remove();
    purged++;
  }
  printf("\033[1;32m Done!\033[0m\n");
  if(purged == 0) {
    printf("No resources for the current platform found in the resource cache.\n");
  } else {
    printf("Successfully purged %d resources from the resource cache.\n", purged);
  }
  printf("\n");
}

void Cache::vacuumResources(const QString inputFolder, const QString filter, const bool unattend)
{
  if(!unattend) {
    std::string userInput = "";
    printf("\033[1;31mWARNING!!! Vacuuming your Skyscraper cache removes all resources that don't match your current romset (files located at '%s' or any of its subdirectories matching the suffixes supported by the platform and any extension(s) you might have added manually). Please consider making a backup of your Skyscraper cache before performing this action. The cache for this platform is listed under 'Cache folder' further up and is usually located under '~/.skyscraper/' unless you've set it manually.\033[0m\n\n", inputFolder.toStdString().c_str());
    printf("\033[1;34mDo you wish to continue\033[0m (y/N)? ");
    getline(std::cin, userInput);
    if(userInput != "y") {
      printf("User chose not to continue, cancelling vacuum...\n\n");
      return;
    }
  }
  
  QList<QString> sha1List;
  QStringList filters = filter.split(" ");
  if(filter.size() < 2) {
    printf("Found less than 2 suffix filters. Something is wrong, cancelling vacuum...\n");
    return;
  }
  printf("Vacuuming resources from cache, this can take several minutes, please wait...");
  QList<QFileInfo> fileInfos;
  QDirIterator dirIt(inputFolder,
		     filters,
		     QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks,
		     QDirIterator::Subdirectories);
  while(dirIt.hasNext()) {
    dirIt.next();
    fileInfos.append(dirIt.fileInfo());
  }
  if(fileInfos.isEmpty()) {
    printf("\nInput folder returned no entries, cancelling vacuum...\n\n");
    return;
  }
  {
    int dots = 0;
    // Always make dotMod at least 1 or it will give "floating point exception" when modulo
    int dotMod = fileInfos.size() * 0.1 + 1;
    foreach(QFileInfo info, fileInfos) {
      if(dots % dotMod == 0) {
	printf(".");
	fflush(stdout);
      }
      dots++;
      sha1List.append(NameTools::getSha1(info));
    }
  }
  
  int vacuumed = 0;
  {
    int dots = 0;
    // Always make dotMod at least 1 or it will give "floating point exception" when modulo
    int dotMod = resources.size() * 0.1 + 1;

    QMutableListIterator<Resource> it(resources);
    while(it.hasNext()) {
      if(dots % dotMod == 0) {
	printf(".");
	fflush(stdout);
      }
      dots++;
      Resource res = it.next();
      bool remove = true;
      foreach(QString sha1, sha1List) {
	if(res.sha1 == sha1) {
	  remove = false;
	  break;
	}
      }
      if(remove) {
	if(res.type == "cover" || res.type == "screenshot" ||
	   res.type == "wheel" || res.type == "marquee" ||
	   res.type == "video") {
	  if(!QFile::remove(cacheDir.absolutePath() + "/" + res.value)) {
	    printf("Couldn't purge media file '%s', skipping...\n", res.value.toStdString().c_str());
	    continue;
	  }
	}
	it.remove();
	vacuumed++;
      }
    }
  }
  printf("\033[1;32m Done!\033[0m\n");
  if(vacuumed == 0) {
    printf("All resources match a file in your romset. No resources vacuumed.\n");
  } else {
    printf("Successfully vacuumed %d resources from the resource cache.\n", vacuumed);
  }
  printf("\n");
}

void Cache::showStats(int verbosity)
{
  printf("Resource cache stats for selected platform:\n");
  if(verbosity == 1) {
    int titles = 0;
    int platforms = 0;
    int descriptions = 0;
    int publishers = 0;
    int developers = 0;
    int players = 0;
    int ages = 0;
    int tags = 0;
    int ratings = 0;
    int releaseDates = 0;
    int covers = 0;
    int screenshots = 0;
    int wheels = 0;
    int marquees = 0;
    int videos = 0;
    for(QMap<QString, ResCounts>::iterator it = resCountsMap.begin();
	it != resCountsMap.end(); ++it) {
      titles += it.value().titles;
      platforms += it.value().platforms;
      descriptions += it.value().descriptions;
      publishers += it.value().publishers;
      developers += it.value().developers;
      players += it.value().players;
      ages += it.value().ages;
      tags += it.value().tags;
      ratings += it.value().ratings;
      releaseDates += it.value().releaseDates;
      covers += it.value().covers;
      screenshots += it.value().screenshots;
      wheels += it.value().wheels;
      marquees += it.value().marquees;
      videos += it.value().videos;
    }
    printf("  Titles       : %d\n", titles);
    printf("  Platforms    : %d\n", platforms);
    printf("  Descriptions : %d\n", descriptions);
    printf("  Publishers   : %d\n", publishers);
    printf("  Developers   : %d\n", developers);
    printf("  Players      : %d\n", players);
    printf("  Ages         : %d\n", ages);
    printf("  Tags         : %d\n", tags);
    printf("  Ratings      : %d\n", ratings);
    printf("  ReleaseDates : %d\n", releaseDates);
    printf("  Covers       : %d\n", covers);
    printf("  Screenshots  : %d\n", screenshots);
    printf("  Wheels       : %d\n", wheels);
    printf("  Marquees     : %d\n", marquees);
    printf("  Videos       : %d\n", videos);
  } else if(verbosity > 1) {
    for(QMap<QString, ResCounts>::iterator it = resCountsMap.begin();
	it != resCountsMap.end(); ++it) {
      printf("'%s' module\n", it.key().toStdString().c_str());
      printf("  Titles       : %d\n", it.value().titles);
      printf("  Platforms    : %d\n", it.value().platforms);
      printf("  Descriptions : %d\n", it.value().descriptions);
      printf("  Publishers   : %d\n", it.value().publishers);
      printf("  Developers   : %d\n", it.value().developers);
      printf("  Ages         : %d\n", it.value().ages);
      printf("  Tags         : %d\n", it.value().tags);
      printf("  Ratings      : %d\n", it.value().ratings);
      printf("  ReleaseDates : %d\n", it.value().releaseDates);
      printf("  Covers       : %d\n", it.value().covers);
      printf("  Screenshots  : %d\n", it.value().screenshots);
      printf("  Wheels       : %d\n", it.value().wheels);
      printf("  Marquees     : %d\n", it.value().marquees);
      printf("  Videos       : %d\n", it.value().videos);
    }
  }
  printf("\n");
}

void Cache::addToResCounts(const QString source, const QString type)
{
  if(type == "title") {
    resCountsMap[source].titles++;
  } else if(type == "platform") {
    resCountsMap[source].platforms++;
  } else if(type == "description") {
    resCountsMap[source].descriptions++;
  } else if(type == "publisher") {
    resCountsMap[source].publishers++;
  } else if(type == "developer") {
    resCountsMap[source].developers++;
  } else if(type == "players") {
    resCountsMap[source].players++;
  } else if(type == "ages") {
    resCountsMap[source].ages++;
  } else if(type == "tags") {
    resCountsMap[source].tags++;
  } else if(type == "rating") {
    resCountsMap[source].ratings++;
  } else if(type == "releasedate") {
    resCountsMap[source].releaseDates++;
  } else if(type == "cover") {
    resCountsMap[source].covers++;
  } else if(type == "screenshot") {
    resCountsMap[source].screenshots++;
  } else if(type == "wheel") {
    resCountsMap[source].wheels++;
  } else if(type == "marquee") {
    resCountsMap[source].marquees++;
  } else if(type == "video") {
    resCountsMap[source].videos++;
  }
}

void Cache::readPriorities()
{
  QDomDocument prioDoc;
  QFile prioFile(cacheDir.absolutePath() + "/priorities.xml");
  printf("Looking for optional 'priorities.xml' file in cache folder... ");
  if(prioFile.open(QIODevice::ReadOnly)) {
    printf("\033[1;32mFound!\033[0m\n");
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

bool Cache::write()
{
  bool result = false;

  QFile cacheFile(cacheDir.absolutePath() + "/db.xml");
  if(cacheFile.open(QIODevice::WriteOnly)) {
    printf("Writing %d (%d new) resources to cache, please wait... ",
	   resources.length(), resources.length() - resAtLoad);
    fflush(stdout);
    QXmlStreamWriter xml(&cacheFile);
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
    printf("\033[1;32mSuccess!\033[0m\n\n");
    cacheFile.close();
  }
  return result;
}

// This verifies all attached media files and deletes those that have no entry in the cache
void Cache::validate()
{
  // TODO: Add format checks for each resource type, and remove if deemed corrupt

  printf("Starting resource cache validation run, please wait...\n");

  if(!QFileInfo::exists(cacheDir.absolutePath() + "/db.xml")) {
    printf("'db.xml' not found, cache cleaning cancelled...\n");
    return;
  }

  QDir coversDir(cacheDir.absolutePath() + "/covers", "*.*", QDir::Name, QDir::Files);
  QDir screenshotsDir(cacheDir.absolutePath() + "/screenshots", "*.*", QDir::Name, QDir::Files);
  QDir wheelsDir(cacheDir.absolutePath() + "/wheels", "*.*", QDir::Name, QDir::Files);
  QDir marqueesDir(cacheDir.absolutePath() + "/marquees", "*.*", QDir::Name, QDir::Files);
  QDir videosDir(cacheDir.absolutePath() + "/videos", "*.*", QDir::Name, QDir::Files);

  QDirIterator coversDirIt(coversDir.absolutePath(),
			   QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks,
			   QDirIterator::Subdirectories);

  QDirIterator screenshotsDirIt(screenshotsDir.absolutePath(),
				QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks,
				QDirIterator::Subdirectories);

  QDirIterator wheelsDirIt(wheelsDir.absolutePath(),
			   QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks,
			   QDirIterator::Subdirectories);

  QDirIterator marqueesDirIt(marqueesDir.absolutePath(),
			     QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks,
			     QDirIterator::Subdirectories);
  
  QDirIterator videosDirIt(videosDir.absolutePath(),
			   QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks,
			   QDirIterator::Subdirectories);

  int filesDeleted = 0;
  int filesNoDelete = 0;

  verifyFiles(coversDirIt, filesDeleted, filesNoDelete, "cover");
  verifyFiles(screenshotsDirIt, filesDeleted, filesNoDelete, "screenshot");
  verifyFiles(wheelsDirIt, filesDeleted, filesNoDelete, "wheel");
  verifyFiles(marqueesDirIt, filesDeleted, filesNoDelete, "marquee");
  verifyFiles(videosDirIt, filesDeleted, filesNoDelete, "video");

  if(filesDeleted == 0 && filesNoDelete == 0) {
    printf("No inconsistencies found in the database. :)\n\n");
  } else {
    printf("Successfully deleted %d files with no resource entry.\n", filesDeleted);
    if(filesNoDelete != 0) {
      printf("%d files couldn't be deleted, please check file permissions and re-run with '--cache validate'.\n", filesNoDelete);
    }
    printf("\n");
  }
}

void Cache::verifyFiles(QDirIterator &dirIt, int &filesDeleted, int &filesNoDelete, QString resType)
{
  QList<QString> resFileNames;
  foreach(Resource resource, resources) {
    if(resource.type == resType) {
      QFileInfo resInfo(cacheDir.absolutePath() + "/" + resource.value);
      resFileNames.append(resInfo.absoluteFilePath());
    }
  }

  while(dirIt.hasNext()) {
    QFileInfo fileInfo(dirIt.next());
    if(!resFileNames.contains(fileInfo.absoluteFilePath())) {
      printf("No resource entry for file '%s', deleting... ",
	     fileInfo.absoluteFilePath().toStdString().c_str());
      if(QFile::remove(fileInfo.absoluteFilePath())) {
	printf("OK!\n");
	filesDeleted++;
      } else {
	printf("ERROR! File couldn't be deleted :/\n");
	filesNoDelete++;
      }
    }
  }
}

void Cache::merge(Cache &mergeCache, bool overwrite, const QString &mergeCacheFolder)
{
  printf("Merging databases, please wait...\n");
  QList<Resource> mergeResources = mergeCache.getResources();

  QDir mergeCacheDir(mergeCacheFolder);
  
  int resUpdated = 0;
  int resMerged = 0;

  foreach(Resource mergeResource, mergeResources) {
    bool resExists = false;
    // This type of iterator ensures we can delete items while iterating
    QMutableListIterator<Resource> it(resources);
    while(it.hasNext()) {
      Resource res = it.next();
      if(res.sha1 == mergeResource.sha1 &&
	 res.type == mergeResource.type &&
	 res.source == mergeResource.source) {
	if(overwrite) {
	  if(res.type == "cover" || res.type == "screenshot" ||
	     res.type == "wheel" || res.type == "marquee" ||
	     res.type == "video") {
	    if(!QFile::remove(cacheDir.absolutePath() + "/" + res.value)) {
	      printf("Couldn't remove media file '%s' for updating, skipping...\n", res.value.toStdString().c_str());
	      continue;
	    }
	    
	  }
	  it.remove();
	} else {
	  resExists = true;
	  break;
	}
      }
    }
    if(!resExists) {
      if(mergeResource.type == "cover" || mergeResource.type == "screenshot" ||
	 mergeResource.type == "wheel" || mergeResource.type == "marquee" ||
	 mergeResource.type == "video") {
	cacheDir.mkpath(QFileInfo(cacheDir.absolutePath() + "/" + mergeResource.value).absolutePath());
	if(!QFile::copy(mergeCacheDir.absolutePath() + "/" + mergeResource.value,
			cacheDir.absolutePath() + "/" + mergeResource.value)) {
	  printf("Couldn't copy media file '%s', skipping...\n",  mergeResource.value.toStdString().c_str());
	  continue;
	}
      }
      if(overwrite) {
	resUpdated++;
      } else {
	resMerged++;
      }
      resources.append(mergeResource);
    }
  }
  printf("Successfully updated %d resource(s) in cache!\n", resUpdated);
  printf("Successfully merged %d new resource(s) into cache!\n\n", resMerged);
}

QList<Resource> Cache::getResources()
{
  return resources;
}
    
void Cache::addResources(GameEntry &entry, const Settings &config)
{
  QString cacheAbsolutePath = cacheDir.absolutePath();

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
      addResource(resource, entry, cacheAbsolutePath, config);
    }
    if(entry.platform != "") {
      resource.type = "platform";
      resource.value = entry.platform;
      addResource(resource, entry, cacheAbsolutePath, config);
    }
    if(entry.description != "") {
      resource.type = "description";
      resource.value = entry.description;
      addResource(resource, entry, cacheAbsolutePath, config);
    }
    if(entry.publisher != "") {
      resource.type = "publisher";
      resource.value = entry.publisher;
      addResource(resource, entry, cacheAbsolutePath, config);
    }
    if(entry.developer != "") {
      resource.type = "developer";
      resource.value = entry.developer;
      addResource(resource, entry, cacheAbsolutePath, config);
    }
    if(entry.players != "") {
      resource.type = "players";
      resource.value = entry.players;
      addResource(resource, entry, cacheAbsolutePath, config);
    }
    if(entry.ages != "") {
      resource.type = "ages";
      resource.value = entry.ages;
      addResource(resource, entry, cacheAbsolutePath, config);
    }
    if(entry.tags != "") {
      resource.type = "tags";
      resource.value = entry.tags;
      addResource(resource, entry, cacheAbsolutePath, config);
    }
    if(entry.rating != "") {
      resource.type = "rating";
      resource.value = entry.rating;
      addResource(resource, entry, cacheAbsolutePath, config);
    }
    if(entry.releaseDate != "") {
      resource.type = "releasedate";
      resource.value = entry.releaseDate;
      addResource(resource, entry, cacheAbsolutePath, config);
    }
    if(entry.videoData != "" && entry.videoFormat != "") {
      resource.type = "video";
      resource.value = "videos/" + entry.source + "/"  + entry.sha1 + "." + entry.videoFormat;
      addResource(resource, entry, cacheAbsolutePath, config);
    }
    if(!entry.coverData.isNull() && config.cacheCovers) {
      resource.type = "cover";
      resource.value = "covers/" + entry.source + "/" + entry.sha1 + ".png";
      addResource(resource, entry, cacheAbsolutePath, config);
    }
    if(!entry.screenshotData.isNull() && config.cacheScreenshots) {
      resource.type = "screenshot";
      resource.value = "screenshots/" + entry.source + "/"  + entry.sha1 + ".png";
      addResource(resource, entry, cacheAbsolutePath, config);
    }
    if(!entry.wheelData.isNull() && config.cacheWheels) {
      resource.type = "wheel";
      resource.value = "wheels/" + entry.source + "/"  + entry.sha1 + ".png";
      addResource(resource, entry, cacheAbsolutePath, config);
    }
    if(!entry.marqueeData.isNull() && config.cacheMarquees) {
      resource.type = "marquee";
      resource.value = "marquees/" + entry.source + "/"  + entry.sha1 + ".png";
      addResource(resource, entry, cacheAbsolutePath, config);
    }
  }
}

void Cache::addResource(const Resource &resource, GameEntry &entry,
			  const QString &cacheAbsolutePath, const Settings &config)
{
  QMutexLocker locker(&cacheMutex);
  bool notFound = true;
  // This type of iterator ensures we can delete items while iterating
  QMutableListIterator<Resource> it(resources);
  while(it.hasNext()) {
    Resource res = it.next();
    if(res.sha1 == resource.sha1 &&
       res.type == resource.type &&
       res.source == resource.source) {
      if(config.refresh) {
	it.remove();
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
      if(entry.coverData.height() >= 512 && !config.noResize) {
	entry.coverData = entry.coverData.scaledToHeight(512, Qt::SmoothTransformation);
      }
      if(!entry.coverData.convertToFormat(QImage::Format_ARGB6666_Premultiplied).save(cacheAbsolutePath + "/" + resource.value)) {
	okToAppend = false;
      }
    } else if(resource.type == "screenshot") {
      // Restrict size of screenshot to save space
      if(entry.screenshotData.width() >= 640 && !config.noResize) {
	entry.screenshotData = entry.screenshotData.scaledToWidth(640, Qt::SmoothTransformation);
      }
      if(!entry.screenshotData.convertToFormat(QImage::Format_ARGB6666_Premultiplied).save(cacheAbsolutePath + "/" + resource.value)) {
	okToAppend = false;
      }
    } else if(resource.type == "wheel") {
      // Restrict size of wheel to save space
      if(entry.wheelData.width() >= 640 && !config.noResize) {
	entry.wheelData = entry.wheelData.scaledToWidth(640, Qt::SmoothTransformation);
      }
      if(!entry.wheelData.convertToFormat(QImage::Format_ARGB6666_Premultiplied).save(cacheAbsolutePath + "/" + resource.value)) {
	okToAppend = false;
      }
    } else if(resource.type == "marquee") {
      // Restrict size of marquee to save space
      if(entry.marqueeData.width() >= 640 && !config.noResize) {
	entry.marqueeData = entry.marqueeData.scaledToWidth(640, Qt::SmoothTransformation);
      }
      if(!entry.marqueeData.convertToFormat(QImage::Format_ARGB6666_Premultiplied).save(cacheAbsolutePath + "/" + resource.value)) {
	okToAppend = false;
      }
    } else if(resource.type == "video") {
      QFile videoFile(cacheAbsolutePath + "/" + resource.value);
      if(videoFile.open(QIODevice::WriteOnly)) {
	videoFile.write(entry.videoData);
	videoFile.close();
      } else {
	okToAppend = false;
      }
    }

    if(okToAppend) {
      resources.append(resource);
    } else {
      printf("\033[1;33mWarning! Couldn't add resource to cache. Have you run out of disk space?\n\033[0m");
    }
    
  }
}

bool Cache::hasEntries(const QString &sha1, const QString scraper)
{
  QMutexLocker locker(&cacheMutex);
  foreach(Resource res, resources) {
    if(scraper.isEmpty()) {
      if(res.sha1 == sha1) {
	return true;
      }
    } else {
      if(res.sha1 == sha1 && res.source == scraper) {
	return true;
      }
    }
  }
  return false;
}

void Cache::fillBlanks(GameEntry &entry, const QString scraper)
{
  QMutexLocker locker(&cacheMutex);
  QList<Resource> matchingResources;
  // Find all resources related to this particular rom
  foreach(Resource resource, resources) {
    if(scraper.isEmpty()) {
      if(entry.sha1 == resource.sha1) {
	matchingResources.append(resource);
      }
    } else {
      if(entry.sha1 == resource.sha1 && resource.source == scraper) {
	matchingResources.append(resource);
      }
    }
  }

  {
    QString type = "title";
    QString result = "";
    QString source = "";
    if(fillType(type, matchingResources, result, source)) {
      entry.title = result;
      entry.titleSrc = source;
    }
  }
  {
    QString type = "platform";
    QString result = "";
    QString source = "";
    if(fillType(type, matchingResources, result, source)) {
      entry.platform = result;
      entry.platformSrc = source;
    }
  }
  {
    QString type = "description";
    QString result = "";
    QString source = "";
    if(fillType(type, matchingResources, result, source)) {
      entry.description = result;
      entry.descriptionSrc = source;
    }
  }
  {
    QString type = "publisher";
    QString result = "";
    QString source = "";
    if(fillType(type, matchingResources, result, source)) {
      entry.publisher = result;
      entry.publisherSrc = source;
    }
  }
  {
    QString type = "developer";
    QString result = "";
    QString source = "";
    if(fillType(type, matchingResources, result, source)) {
      entry.developer = result;
      entry.developerSrc = source;
    }
  }
  {
    QString type = "players";
    QString result = "";
    QString source = "";
    if(fillType(type, matchingResources, result, source)) {
      entry.players = result;
      entry.playersSrc = source;
    }
  }
  {
    QString type = "ages";
    QString result = "";
    QString source = "";
    if(fillType(type, matchingResources, result, source)) {
      entry.ages = result;
      entry.agesSrc = source;
    }
  }
  {
    QString type = "tags";
    QString result = "";
    QString source = "";
    if(fillType(type, matchingResources, result, source)) {
      entry.tags = result;
      entry.tagsSrc = source;
    }
  }
  {
    QString type = "rating";
    QString result = "";
    QString source = "";
    if(fillType(type, matchingResources, result, source)) {
      entry.rating = result;
      entry.ratingSrc = source;
    }
  }
  {
    QString type = "releasedate";
    QString result = "";
    QString source = "";
    if(fillType(type, matchingResources, result, source)) {
      entry.releaseDate = result;
      entry.releaseDateSrc = source;
    }
  }
  {
    QString type = "cover";
    QString result = "";
    QString source = "";
    if(fillType(type, matchingResources, result, source)) {
      entry.coverData = QImage(cacheDir.absolutePath() + "/" + result);
      entry.coverSrc = source;
    }
  }
  {
    QString type = "screenshot";
    QString result = "";
    QString source = "";
    if(fillType(type, matchingResources, result, source)) {
      entry.screenshotData = QImage(cacheDir.absolutePath() + "/" + result);
      entry.screenshotSrc = source;
    }
  }
  {
    QString type = "wheel";
    QString result = "";
    QString source = "";
    if(fillType(type, matchingResources, result, source)) {
      entry.wheelData = QImage(cacheDir.absolutePath() + "/" + result);
      entry.wheelSrc = source;
    }
  }
  {
    QString type = "marquee";
    QString result = "";
    QString source = "";
    if(fillType(type, matchingResources, result, source)) {
      entry.marqueeData = QImage(cacheDir.absolutePath() + "/" + result);
      entry.marqueeSrc = source;
    }
  }
  {
    QString type = "video";
    QString result = "";
    QString source = "";
    if(fillType(type, matchingResources, result, source)) {
      QFileInfo info(cacheDir.absolutePath() + "/" + result);
      QFile videoFile(info.absoluteFilePath());
      if(videoFile.open(QIODevice::ReadOnly)) {
	entry.videoData = videoFile.readAll();
	entry.videoFormat = info.suffix();
	entry.videoFile = info.absoluteFilePath();
	entry.videoSrc = source;
	videoFile.close();
      }
    }
  }
}

bool Cache::fillType(QString &type, QList<Resource> &matchingResources,
		       QString &result, QString &source)
{
  QList<Resource> typeResources;
  foreach(Resource resource, matchingResources) {
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
	  source = resource.source;
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
      source = resource.source;
    }
  }  
  return true;
}
