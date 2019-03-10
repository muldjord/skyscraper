/***************************************************************************
 *            thegamesdb.cpp
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

#include <QJsonArray>

#include "thegamesdb.h"
#include "strtools.h"

TheGamesDb::TheGamesDb(Settings *config) : AbstractScraper(config)
{
  connect(&manager, &NetComm::dataReady, &q, &QEventLoop::quit);

  loadMaps();
  
  baseUrl = "https://api.thegamesdb.net";

  searchUrlPre = "https://api.thegamesdb.net/Games/ByGameName?apikey=";
  
  fetchOrder.append(RELEASEDATE);
  fetchOrder.append(DESCRIPTION);
  fetchOrder.append(TAGS);
  fetchOrder.append(PLAYERS);
  fetchOrder.append(AGES);
  fetchOrder.append(PUBLISHER);
  fetchOrder.append(DEVELOPER);
  //fetchOrder.append(RATING);
  fetchOrder.append(COVER);
  fetchOrder.append(SCREENSHOT);
  //fetchOrder.append(WHEEL);
}

void TheGamesDb::getSearchResults(QList<GameEntry> &gameEntries,
				  QString searchName, QString platform)
{
  manager.request(searchUrlPre + StrTools::unMagic("187;161;217;126;172;149;202;122;163;197;163;219;162;171;203;197;139;151;215;173;122;206;161;162;200;216;217;123;124;215;200;170;171;132;158;155;215;120;149;169;140;164;122;154;178;174;160;172;157;131;210;161;203;137;159;117;205;166;162;139;171;169;210;163") + "&name="+ searchName);
  q.exec();
  data = manager.getData();
  
  jsonDoc = QJsonDocument::fromJson(data);
  if(jsonDoc.isEmpty()) {
    return;
  }

  reqRemaining = jsonDoc.object().value("remaining_monthly_allowance").toInt();

  if(jsonDoc.object().value("status").toString() != "Success") {
    return;
  }
  if(jsonDoc.object().value("data").toObject().value("count").toInt() < 1) {
    return;
  }

  QJsonArray jsonGames = jsonDoc.object().value("data").toObject().value("games").toArray();

  while(!jsonGames.isEmpty()) {
    QJsonObject jsonGame = jsonGames.first().toObject();
    
    GameEntry game;
    // https://api.thegamesdb.net/Games/ByGameID?id=88&apikey=XXX&fields=game_title,players,release_date,developer,publisher,genres,overview,rating,platform
    game.id = QString::number(jsonGame.value("id").toInt());
    game.url = "https://api.thegamesdb.net/Games/ByGameID?id=" + game.id + "&apikey=" + StrTools::unMagic("187;161;217;126;172;149;202;122;163;197;163;219;162;171;203;197;139;151;215;173;122;206;161;162;200;216;217;123;124;215;200;170;171;132;158;155;215;120;149;169;140;164;122;154;178;174;160;172;157;131;210;161;203;137;159;117;205;166;162;139;171;169;210;163") + "&fields=game_title,players,release_date,developers,publishers,genres,overview,rating";
    game.title = jsonGame.value("game_title").toString();
    // Remove anything at the end with a parentheses. 'thegamesdb' has a habit of adding
    // for instance '(1993)' to the name.
    game.title = game.title.left(game.title.indexOf("(")).simplified();
    game.platform = platformMap[jsonGame.value("platform").toInt()];
    if(platformMatch(game.platform, platform)) {
      gameEntries.append(game);
    }
    jsonGames.removeFirst();
  }
}

void TheGamesDb::getGameData(GameEntry &game)
{
  manager.request(game.url);
  q.exec();
  data = manager.getData();
  jsonDoc = QJsonDocument::fromJson(data);
  if(jsonDoc.isEmpty()) {
    printf("No returned json data, is 'thegamesdb' down?\n");
    reqRemaining = 0;
  }

  reqRemaining = jsonDoc.object().value("remaining_monthly_allowance").toInt();

  if(jsonDoc.object().value("data").toObject().value("count").toInt() < 1) {
    printf("No returned json game document, is 'thegamesdb' down?\n");
    reqRemaining = 0;
  }
  
  jsonObj = jsonDoc.object().value("data").toObject().value("games").toArray().first().toObject();

  for(int a = 0; a < fetchOrder.length(); ++a) {
    switch(fetchOrder.at(a)) {
    case DESCRIPTION:
      getDescription(game);
      break;
    case DEVELOPER:
      getDeveloper(game);
      break;
    case PUBLISHER:
      getPublisher(game);
      break;
    case PLAYERS:
      getPlayers(game);
      break;
    case AGES:
      getAges(game);
      break;
    case RATING:
      getRating(game);
      break;
    case TAGS:
      getTags(game);
      break;
    case RELEASEDATE:
      getReleaseDate(game);
      break;
    case COVER:
      getCover(game);
      break;
    case SCREENSHOT:
      getScreenshot(game);
      break;
    case WHEEL:
      getWheel(game);
      break;
    case MARQUEE:
      getMarquee(game);
      break;
    case VIDEO:
      if(config->videos) {
	getVideo(game);
      }
      break;
    default:
      ;
    }
  }
}

void TheGamesDb::getReleaseDate(GameEntry &game)
{
  if(jsonObj.value("release_date") != QJsonValue::Undefined)
    game.releaseDate = jsonObj.value("release_date").toString();
}

void TheGamesDb::getDeveloper(GameEntry &game)
{
  QJsonArray developers = jsonObj.value("developers").toArray();
  if(developers.count() != 0)
    game.developer = developerMap[developers.first().toInt()];
}

void TheGamesDb::getPublisher(GameEntry &game)
{
  QJsonArray publishers = jsonObj.value("publishers").toArray();
  if(publishers.count() != 0)
    game.publisher = publisherMap[publishers.first().toInt()];
}

void TheGamesDb::getDescription(GameEntry &game)
{
  game.description = jsonObj.value("overview").toString();
}

void TheGamesDb::getPlayers(GameEntry &game)
{
  int players = jsonObj.value("players").toInt();
  if(players != 0)
    game.players = QString::number(players);
}

void TheGamesDb::getAges(GameEntry &game)
{
  if(jsonObj.value("rating") != QJsonValue::Undefined)
    game.ages = jsonObj.value("rating").toString();
}

void TheGamesDb::getTags(GameEntry &game)
{
  QJsonArray genres = jsonObj.value("genres").toArray();
  if(genres.count() != 0) {
    while(!genres.isEmpty()) {
      game.tags.append(genreMap[genres.first().toInt()] + ", ");
      genres.removeFirst();
    }
    game.tags = game.tags.left(game.tags.length() - 2);
  }
}

void TheGamesDb::getCover(GameEntry &game)
{
  // https://api.thegamesdb.net/Games/Boxart?games_id=88&apikey=XXX&filter=boxart,screenshot
  // https://cdn.thegamesdb.net/images/original/boxart/front/[gameid]-1.jpg
  manager.request("https://cdn.thegamesdb.net/images/original/boxart/front/" + game.id + "-1.jpg");
  q.exec();
  QImage image;
  if(image.loadFromData(manager.getData())) {
    game.coverData = image;
  }
}

void TheGamesDb::getScreenshot(GameEntry &game)
{
  // https://api.thegamesdb.net/Games/Boxart?games_id=88&apikey=XXX&filter=boxart,screenshot
  // https://cdn.thegamesdb.net/images/original/screenshots/[gameid]-1.jpg
  manager.request("https://cdn.thegamesdb.net/images/original/screenshots/" + game.id + "-1.jpg");
  q.exec();
  QImage image;
  if(image.loadFromData(manager.getData())) {
    game.screenshotData = image;
  }
}

void TheGamesDb::loadMaps()
{
  genreMap[1] = "Action";
  genreMap[2] = "Adventure";
  genreMap[3] = "Construction and Management Simulation";
  genreMap[4] = "Role-Playing";
  genreMap[5] = "Puzzle";
  genreMap[6] = "Strategy";
  genreMap[7] = "Racing";
  genreMap[8] = "Shooter";
  genreMap[9] = "Life Simulation";
  genreMap[10] = "Fighting";
  genreMap[11] = "Sports";
  genreMap[12] = "Sandbox";
  genreMap[13] = "Flight Simulator";
  genreMap[14] = "MMO";
  genreMap[15] = "Platform";
  genreMap[16] = "Stealth";
  genreMap[17] = "Music";
  genreMap[18] = "Horror";
  genreMap[19] = "Vehicle Simulation";

  platformMap[25] = "3DO";
  platformMap[4944] = "Acorn Archimedes";
  platformMap[4954] = "Acorn Electron";
  platformMap[4976] = "Action Max";
  platformMap[4911] = "Amiga";
  platformMap[4947] = "Amiga CD32";
  platformMap[4914] = "Amstrad CPC";
  platformMap[4916] = "Android";
  platformMap[4969] = "APF MP-1000";
  platformMap[4942] = "Apple II";
  platformMap[23] = "Arcade";
  platformMap[22] = "Atari 2600";
  platformMap[26] = "Atari 5200";
  platformMap[27] = "Atari 7800";
  platformMap[4943] = "Atari 800";
  platformMap[28] = "Atari Jaguar";
  platformMap[29] = "Atari Jaguar CD";
  platformMap[4924] = "Atari Lynx";
  platformMap[4937] = "Atari ST";
  platformMap[30] = "Atari XE";
  platformMap[4968] = "Bally Astrocade";
  platformMap[4964] = "Casio PV-1000";
  platformMap[4970] = "Coleco Telstar Arcade";
  platformMap[31] = "Colecovision";
  platformMap[4946] = "Commodore 128";
  platformMap[40] = "Commodore 64";
  platformMap[4945] = "Commodore VIC-20";
  platformMap[4952] = "Dragon 32/64";
  platformMap[4963] = "Emerson Arcadia 2001";
  platformMap[4974] = "Entex Adventure Vision";
  platformMap[4973] = "Entex Select-a-Game";
  platformMap[4965] = "Epoch Cassette Vision";
  platformMap[4966] = "Epoch Super Cassette Vision";
  platformMap[4928] = "Fairchild Channel F";
  platformMap[4936] = "Famicom Disk System";
  platformMap[4932] = "FM Towns Marty";
  platformMap[4978] = "Fujitsu FM-7";
  platformMap[4962] = "Gakken Compact Vision";
  platformMap[4950] = "Game & Watch";
  platformMap[4940] = "Game.com";
  platformMap[4951] = "Handheld Electronic Games (LCD)";
  platformMap[32] = "Intellivision";
  platformMap[4915] = "iOS";
  platformMap[37] = "Mac OS";
  platformMap[4961] = "Magnavox Odyssey 1";
  platformMap[4927] = "Magnavox Odyssey 2";
  platformMap[4948] = "Mega Duck";
  platformMap[14] = "Microsoft Xbox";
  platformMap[15] = "Microsoft Xbox 360";
  platformMap[4920] = "Microsoft Xbox One";
  platformMap[4972] = "Milton Bradley Microvision";
  platformMap[4929] = "MSX";
  platformMap[4938] = "N-Gage";
  platformMap[24] = "Neo Geo";
  platformMap[4956] = "Neo Geo CD";
  platformMap[4922] = "Neo Geo Pocket";
  platformMap[4923] = "Neo Geo Pocket Color";
  platformMap[4912] = "Nintendo 3DS";
  platformMap[3] = "Nintendo 64";
  platformMap[8] = "Nintendo DS";
  platformMap[7] = "Nintendo Entertainment System (NES)";
  platformMap[4] = "Nintendo Game Boy";
  platformMap[5] = "Nintendo Game Boy Advance";
  platformMap[41] = "Nintendo Game Boy Color";
  platformMap[2] = "Nintendo GameCube";
  platformMap[4957] = "Nintendo Pok\u00e9mon Mini";
  platformMap[4971] = "Nintendo Switch";
  platformMap[4918] = "Nintendo Virtual Boy";
  platformMap[9] = "Nintendo Wii";
  platformMap[38] = "Nintendo Wii U";
  platformMap[4935] = "Nuon";
  platformMap[4921] = "Ouya";
  platformMap[1] = "PC";
  platformMap[4933] = "PC-88";
  platformMap[4934] = "PC-98";
  platformMap[4930] = "PC-FX";
  platformMap[4917] = "Philips CD-i";
  platformMap[4975] = "Pioneer LaserActive";
  platformMap[4967] = "RCA Studio II";
  platformMap[4979] = "SAM Coup\u00e9";
  platformMap[33] = "Sega 32X";
  platformMap[21] = "Sega CD";
  platformMap[16] = "Sega Dreamcast";
  platformMap[20] = "Sega Game Gear";
  platformMap[18] = "Sega Genesis";
  platformMap[35] = "Sega Master System";
  platformMap[36] = "Sega Mega Drive";
  platformMap[4958] = "Sega Pico";
  platformMap[17] = "Sega Saturn";
  platformMap[4949] = "SEGA SG-1000";
  platformMap[4977] = "Sharp X1";
  platformMap[4931] = "Sharp X68000";
  platformMap[4913] = "Sinclair ZX Spectrum";
  platformMap[10] = "Sony Playstation";
  platformMap[11] = "Sony Playstation 2";
  platformMap[12] = "Sony Playstation 3";
  platformMap[4919] = "Sony Playstation 4";
  platformMap[13] = "Sony Playstation Portable";
  platformMap[39] = "Sony Playstation Vita";
  platformMap[6] = "Super Nintendo (SNES)";
  platformMap[4953] = "Texas Instruments TI-99/4A";
  platformMap[4960] = "Tomy Tutor";
  platformMap[4941] = "TRS-80 Color Computer";
  platformMap[34] = "TurboGrafx 16";
  platformMap[4955] = "TurboGrafx CD";
  platformMap[4939] = "Vectrex";
  platformMap[4959] = "Watara Supervision";
  platformMap[4925] = "WonderSwan";
  platformMap[4926] = "WonderSwan Color";

  {
    QFile jsonFile("tgdb_developers.json");
    if(jsonFile.open(QIODevice::ReadOnly)) {
      QJsonObject jsonDevs = QJsonDocument::fromJson(jsonFile.readAll()).object().value("data").toObject().value("developers").toObject();
      for(QJsonObject::iterator it = jsonDevs.begin(); it != jsonDevs.end(); ++it) {
	developerMap[it.value().toObject().value("id").toInt()] = it.value().toObject().value("name").toString();
      }
      jsonFile.close();
    }
  }
  {
    QFile jsonFile("tgdb_publishers.json");
    if(jsonFile.open(QIODevice::ReadOnly)) {
      QJsonObject jsonPubs = QJsonDocument::fromJson(jsonFile.readAll()).object().value("data").toObject().value("publishers").toObject();
      for(QJsonObject::iterator it = jsonPubs.begin(); it != jsonPubs.end(); ++it) {
	publisherMap[it.value().toObject().value("id").toInt()] = it.value().toObject().value("name").toString();
      }
      jsonFile.close();
    }
  }
}
