/***************************************************************************
 *            igdb.cpp
 *
 *  Sun Aug 26 12:00:00 CEST 2018
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

#include <QJsonArray>

#include "igdb.h"
#include "strtools.h"
#include "nametools.h"

Igdb::Igdb(Settings *config) : AbstractScraper(config)
{
  connect(&manager, &NetComm::dataReady, &q, &QEventLoop::quit);

  baseUrl = "https://api-v3.igdb.com";

  searchUrlPre = "https://api-v3.igdb.com";
  
  fetchOrder.append(RELEASEDATE);
  fetchOrder.append(RATING);
  fetchOrder.append(PUBLISHER);
  fetchOrder.append(DEVELOPER);
  fetchOrder.append(DESCRIPTION);
  fetchOrder.append(PLAYERS);
  fetchOrder.append(TAGS);
  fetchOrder.append(AGES);
  //fetchOrder.append(COVER);
  //fetchOrder.append(SCREENSHOT);
}

void Igdb::getSearchResults(QList<GameEntry> &gameEntries,
				QString searchName, QString platform)
{
  // Request list of games but don't allow re-releases ("game.version_parent = null")

  manager.request(baseUrl + "/search/", "fields game.name,game.platforms.name; search \"" + searchName + "\"; where game != null & game.version_parent = null;", "user-key", StrTools::unMagic("136;213;169;133;171;147;206;117;211;152;214;221;209;213;157;197;136;158;212;220;171;211;160;215;202;172;216;125;172;174;151;171"));
  q.exec();
  data = manager.getData();
  
  if(data.contains("Limits exceeded")) {
    printf("\033[1;31mYour monthly request limit for the 'igdb' scraping module has been reached. You can upgrade the limit at 'https://api.igdb.com/' under 'API credentials'.\033[0m\n");
    reqRemaining = 0;
  }

  if(data.contains("Authentication failed")) {
    printf("\033[1;31mThe key you provided with either the '-u [key]' command line option or the 'userCreds=\"[key]\" variable in config.ini does not seem to work. Now quitting...\033[0m\n\n");
    reqRemaining = 0;
  }

  jsonDoc = QJsonDocument::fromJson(data);
  if(jsonDoc.isEmpty()) {
    return;
  }

  if(jsonDoc.object().value("status").toInt() == 403) {
    printf("\033[1;31mYour monthly request limit for the 'igdb' scraping module has been reached. You can upgrade the limit at 'https://api.igdb.com/' under 'API credentials'.\033[0m\n");
    reqRemaining = 0;
  }

  QJsonArray jsonGames = jsonDoc.array();

  foreach(const QJsonValue &jsonGame, jsonGames) {
    GameEntry game;
    
    game.title = jsonGame.toObject().value("game").toObject().value("name").toString();
    game.id = QString::number(jsonGame.toObject().value("game").toObject().value("id").toInt());

    QJsonArray jsonPlatforms = jsonGame.toObject().value("game").toObject().value("platforms").toArray();
    foreach(const QJsonValue &jsonPlatform, jsonPlatforms) {
      game.id.append(";" + QString::number(jsonPlatform.toObject().value("id").toInt()));
      game.platform = jsonPlatform.toObject().value("name").toString();
      if(platformMatch(game.platform, platform)) {
	gameEntries.append(game);
      }
    }
  }
}

void Igdb::getGameData(GameEntry &game)
{
  manager.request(baseUrl + "/games/", "fields age_ratings.rating,age_ratings.category,total_rating,cover.url,game_modes.name,genres.name,screenshots.url,summary,release_dates.human,release_dates.platform,involved_companies.company.name,involved_companies.developer,involved_companies.publisher; where id = " + game.id.split(";").first() + ";", "user-key", StrTools::unMagic("136;213;169;133;171;147;206;117;211;152;214;221;209;213;157;197;136;158;212;220;171;211;160;215;202;172;216;125;172;174;151;171"));
  q.exec();
  data = manager.getData();

  printf("DATA:\n%s\n", data.data());

  jsonDoc = QJsonDocument::fromJson(data);
  if(jsonDoc.isEmpty()) {
    return;
  }

  jsonObj = jsonDoc.array().first().toObject();
  
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
    case RATING:
      getRating(game);
      break;
    case AGES:
      getAges(game);
      break;
    case TAGS:
      getTags(game);
      break;
    case RELEASEDATE:
      getReleaseDate(game);
      break;
    case COVER:
      if(config->cacheCovers) {
	getCover(game);
      }
      break;
    case SCREENSHOT:
      if(config->cacheScreenshots) {
	getScreenshot(game);
      }
      break;
    default:
      ;
    }
  }
}

void Igdb::getReleaseDate(GameEntry &game)
{
  QJsonArray jsonDates = jsonObj.value("release_dates").toArray();
  foreach(const QJsonValue &jsonDate, jsonDates) {
    if(QString::number(jsonDate.toObject().value("platform").toInt()) == game.id.split(";").last()) {
      game.releaseDate = jsonDate.toObject().value("human").toString();
      break;
    }
  }
}

void Igdb::getPlayers(GameEntry &game)
{
  // This is a bit of a hack. The unique identifiers are as follows:
  // 1 = Single Player
  // 2 = Multiplayer
  // 3 = Cooperative
  // 4 = Split screen
  // 5 = MMO
  // So basically if != 1 it's at least 2 players. That's all we can gather from this
  game.players = "1";
  QJsonArray jsonPlayers = jsonObj.value("game_modes").toArray();
  foreach(const QJsonValue &jsonPlayer, jsonPlayers) {
    if(jsonPlayer.toInt() != 1) {
      game.players = "2";
      break;
    }
  }
}

void Igdb::getTags(GameEntry &game)
{
  QJsonArray jsonGenres = jsonObj.value("genres").toArray();
  foreach(const QJsonValue &jsonGenre, jsonGenres) {
    game.tags.append(jsonGenre.toObject().value("name").toString() + ", ");
  }
  game.tags = game.tags.left(game.tags.length() - 2);
}

void Igdb::getAges(GameEntry &game)
{
  int agesEnum = jsonObj.value("age_ratings").toArray().first().toObject().value("rating").toInt();
  if(agesEnum == 1) {
    game.ages = "3";
  } else if(agesEnum == 2) {
    game.ages = "7";
  } else if(agesEnum == 3) {
    game.ages = "12";
  } else if(agesEnum == 4) {
    game.ages = "16";
  } else if(agesEnum == 5) {
    game.ages = "18";
  } else if(agesEnum == 6) {
    // Rating pending
  } else if(agesEnum == 7) {
    game.ages = "EC";
  } else if(agesEnum == 8) {
    game.ages = "E";
  } else if(agesEnum == 9) {
    game.ages = "E10";
  } else if(agesEnum == 10) {
    game.ages = "T";
  } else if(agesEnum == 11) {
    game.ages = "M";
  } else if(agesEnum == 12) {
    game.ages = "AO";
  }
}

void Igdb::getPublisher(GameEntry &game)
{
  QJsonArray jsonCompanies = jsonObj.value("involved_companies").toArray();
  foreach(const QJsonValue &jsonCompany, jsonCompanies) {
    if(jsonCompany.toObject().value("publisher").toBool() == true) {
      game.publisher = jsonCompany.toObject().value("company").toObject().value("name").toString();
      return;
    }
  }  
}

void Igdb::getDeveloper(GameEntry &game)
{
  QJsonArray jsonCompanies = jsonObj.value("involved_companies").toArray();
  foreach(const QJsonValue &jsonCompany, jsonCompanies) {
    if(jsonCompany.toObject().value("developer").toBool() == true) {
      game.developer = jsonCompany.toObject().value("company").toObject().value("name").toString();
      return;
    }
  }  
}

void Igdb::getDescription(GameEntry &game)
{
  QJsonValue jsonValue = jsonObj.value("summary");
  if(jsonValue != QJsonValue::Undefined) {
    game.description = StrTools::stripHtmlTags(jsonValue.toString());
  }
}

void Igdb::getRating(GameEntry &game)
{
  QJsonValue jsonValue = jsonObj.value("total_rating");
  if(jsonValue != QJsonValue::Undefined) {
    double rating = jsonValue.toDouble();
    if(rating != 0.0) {
      game.rating = QString::number(rating / 100.0);
    }
  }
}

QList<QString> Igdb::getSearchNames(const QFileInfo &info)
{
  QString baseName = info.completeBaseName();

  if(aliasMap.contains(baseName)) {
    baseName = aliasMap[baseName];
  } else if(info.suffix() == "lha") {
    QString nameWithSpaces = whdLoadMap[baseName].first;
    if(nameWithSpaces.isEmpty()) {
      baseName = NameTools::getNameWithSpaces(baseName);
    } else {
      baseName = nameWithSpaces;
    }
  } else if(config->platform == "scummvm") {
    baseName = NameTools::getScummName(baseName);
  } else if((config->platform == "neogeo" ||
	     config->platform == "arcade" ||
	     config->platform == "mame-advmame" ||
	     config->platform == "mame-libretro" ||
	     config->platform == "mame-mame4all" ||
	     config->platform == "fba") && mameMap.contains(baseName)) {
    baseName = mameMap[baseName];
  }
  baseName = StrTools::stripBrackets(baseName);
  QList<QString> searchNames;
  searchNames.append(baseName);
  return searchNames;
}

/*
JSON SEARCH RESULT:
[
  {
    "id": 105940,
    "game": {
      "id": 1073,
      "name": "Super Mario World 2: Yoshi\u0027s Island",
      "platforms": [
        {
          "id": 19,
          "name": "Super Nintendo Entertainment System (SNES)"
        },
        {
          "id": 24,
          "name": "Game Boy Advance"
        },
        {
          "id": 47,
          "name": "Virtual Console (Nintendo)"
        }
      ]
    }
  }
]

[
  {
    "id": 1070,
    "age_ratings": [
      19985,
      19986
    ],
    "aggregated_rating": 100.0,
    "aggregated_rating_count": 1,
    "alternative_names": [
      3781,
      3782
    ],
    "category": 0,
    "cover": 69388,
    "created_at": 1339200000,
    "external_games": [
      188859,
      245175
    ],
    "first_release_date": 659145600,
    "game_modes": [
      1,
      2
    ],
    "genres": [
      8
    ],
    "involved_companies": [
      22190,
      22191,
      22192
    ],
    "name": "Super Mario World",
    "platforms": [
      5,
      19,
      41,
      137
    ],
    "player_perspectives": [
      4
    ],
    "popularity": 17.52125071670721,
    "rating": 92.53825753171901,
    "rating_count": 516,
    "release_dates": [
      132781,
      132782,
      132783,
      132784,
      132785,
      132786,
      143838,
      143839,
      143840,
      143841,
      143842
    ],
    "screenshots": [
      9506,
      9507,
      9508,
      9509,
      174828,
      174829,
      174830,
      174831,
      174832,
      174833,
      174834,
      174835
    ],
    "slug": "super-mario-world",
    "summary": "Super Mario World (known in Japan as Super Mario World: Super Mario Bros. 4) is a side-scrolling platformer developed by Nintendo EAD and published by Nintendo for the Super Nintendo Entertainment System on November 21, 1990 (in Japan), August 31, 1991 (in North America), and April 11, 1992 (in Europe). \n \nOne of the launch titles of the SNES (and bundled with early systems in North America), Super Mario World is the fifth main game in the Super Mario series. (starring Nintendo\u0027s mascot, Mario, and his brother, Luigi). The game follows both Mario brothers as they explore Dinosaur Land (known for its large amount of dinosaurs) to find and defeat the evil Koopa king Bowser (and his seven underlings, the Koopalings) while rescuing Princess Toadstool. \n \nAlong with new abilities (such as the \"Spin Jump\"), a new power-up (the \"Cape Feather\") and more obstacles, the game introduces dinosaur companions (known as Yoshi) that Mario and Luigi can ride. Yoshi, known for using their long tongues to snare and eat enemies, have become a fan-favorite among the series (giving them their own games and spin-offs, most notably this game\u0027s prequel). \n \nSpecial bundles of the SNES in 1994 included a compilation cartridge mixing Super Mario World with Super Mario All-Stars. The only difference in this version is a new sprite set for Luigi. The original game was later ported to the Game Boy Advance on February 11, 2002 as Super Mario World: Super Mario Advance 2. Along with a special version of the original Mario Bros., the port includes a variety of differences, including Luigi as a selectable character (who now has unique features, such as his floating jump from Super Mario Bros. 2), new voice acting, and the ability to save anywhere. The original game was also digitally re-released in Nintendo\u0027s Virtual Console platform for the Wii (on February 5, 2007) and Wii U (on April 26, 2013).",
    "tags": [
      1,
      268435464
    ],
    "themes": [
      1
    ],
    "total_rating": 96.2691287658595,
    "total_rating_count": 517,
    "updated_at": 1546214400,
    "url": "https://www.igdb.com/games/super-mario-world",
    "videos": [
      17409
    ],
    "websites": [
      11549
    ]
  }
]

[
  {
    "id": 1070,
    "age_ratings": [
      {
        "id": 19985,
        "category": 1,
        "rating": 3
      },
      {
        "id": 19986,
        "category": 2,
        "rating": 2
      }
    ],
    "cover": {
      "id": 69388,
      "url": "//images.igdb.com/igdb/image/upload/t_thumb/co1hjg.jpg"
    },
    "game_modes": [
      {
        "id": 1,
        "name": "Single player"
      },
      {
        "id": 2,
        "name": "Multiplayer"
      }
    ],
    "genres": [
      {
        "id": 8,
        "name": "Platform"
      }
    ],
    "involved_companies": [
      {
        "id": 22190,
        "company": {
          "id": 421,
          "name": "Nintendo EAD"
        },
        "developer": true,
        "publisher": false
      },
      {
        "id": 22191,
        "company": {
          "id": 70,
          "name": "Nintendo"
        },
        "developer": false,
        "publisher": true
      },
      {
        "id": 22192,
        "company": {
          "id": 2889,
          "name": "Mattel"
        },
        "developer": false,
        "publisher": true
      }
    ],
    "release_dates": [
      {
        "id": 132781,
        "date": 659145600,
        "platform": 19
      },
      {
        "id": 132782,
        "date": 682041600,
        "platform": 19
      },
      {
        "id": 132783,
        "date": 702950400,
        "platform": 19
      },
      {
        "id": 132784,
        "date": 1456963200,
        "platform": 137
      },
      {
        "id": 132785,
        "date": 1366934400,
        "platform": 41
      },
      {
        "id": 132786,
        "platform": 5
      },
      {
        "id": 143838,
        "date": 1170633600,
        "platform": 5
      },
      {
        "id": 143839,
        "date": 1170979200,
        "platform": 5
      },
      {
        "id": 143840,
        "date": 1165017600,
        "platform": 5
      },
      {
        "id": 143841,
        "date": 1367020800,
        "platform": 41
      },
      {
        "id": 143842,
        "date": 1367020800,
        "platform": 41
      }
    ],
    "screenshots": [
      {
        "id": 9506,
        "url": "//images.igdb.com/igdb/image/upload/t_thumb/i9sgtqwdmmdjzox51rgu.jpg"
      },
      {
        "id": 9507,
        "url": "//images.igdb.com/igdb/image/upload/t_thumb/pdkvjmhh5urjnrwng5cg.jpg"
      },
      {
        "id": 9508,
        "url": "//images.igdb.com/igdb/image/upload/t_thumb/bbkmycy8wacpka1hroy5.jpg"
      },
      {
        "id": 9509,
        "url": "//images.igdb.com/igdb/image/upload/t_thumb/xqj7kfjsgqrboda05dnp.jpg"
      },
      {
        "id": 174828,
        "url": "//images.igdb.com/igdb/image/upload/t_thumb/swouq6zc6c5jjq6exjcx.jpg"
      },
      {
        "id": 174829,
        "url": "//images.igdb.com/igdb/image/upload/t_thumb/lzyxgb64aw2a28ufdkhj.jpg"
      },
      {
        "id": 174830,
        "url": "//images.igdb.com/igdb/image/upload/t_thumb/gfngetbkukbrjztd5u1b.jpg"
      },
      {
        "id": 174831,
        "url": "//images.igdb.com/igdb/image/upload/t_thumb/pnqm1e1nczs9zkyukd5b.jpg"
      },
      {
        "id": 174832,
        "url": "//images.igdb.com/igdb/image/upload/t_thumb/xqpfrjf6tclfjmr4gkhi.jpg"
      },
      {
        "id": 174833,
        "url": "//images.igdb.com/igdb/image/upload/t_thumb/oxfxrwtxbswrrqf4dc9x.jpg"
      },
      {
        "id": 174834,
        "url": "//images.igdb.com/igdb/image/upload/t_thumb/nwdmxtvfj7tro1iqwgbn.jpg"
      },
      {
        "id": 174835,
        "url": "//images.igdb.com/igdb/image/upload/t_thumb/j58abebg08zrh6vjdvkk.jpg"
      }
    ],
    "summary": "Super Mario World (known in Japan as Super Mario World: Super Mario Bros. 4) is a side-scrolling platformer developed by Nintendo EAD and published by Nintendo for the Super Nintendo Entertainment System on November 21, 1990 (in Japan), August 31, 1991 (in North America), and April 11, 1992 (in Europe). \n \nOne of the launch titles of the SNES (and bundled with early systems in North America), Super Mario World is the fifth main game in the Super Mario series. (starring Nintendo\u0027s mascot, Mario, and his brother, Luigi). The game follows both Mario brothers as they explore Dinosaur Land (known for its large amount of dinosaurs) to find and defeat the evil Koopa king Bowser (and his seven underlings, the Koopalings) while rescuing Princess Toadstool. \n \nAlong with new abilities (such as the \"Spin Jump\"), a new power-up (the \"Cape Feather\") and more obstacles, the game introduces dinosaur companions (known as Yoshi) that Mario and Luigi can ride. Yoshi, known for using their long tongues to snare and eat enemies, have become a fan-favorite among the series (giving them their own games and spin-offs, most notably this game\u0027s prequel). \n \nSpecial bundles of the SNES in 1994 included a compilation cartridge mixing Super Mario World with Super Mario All-Stars. The only difference in this version is a new sprite set for Luigi. The original game was later ported to the Game Boy Advance on February 11, 2002 as Super Mario World: Super Mario Advance 2. Along with a special version of the original Mario Bros., the port includes a variety of differences, including Luigi as a selectable character (who now has unique features, such as his floating jump from Super Mario Bros. 2), new voice acting, and the ability to save anywhere. The original game was also digitally re-released in Nintendo\u0027s Virtual Console platform for the Wii (on February 5, 2007) and Wii U (on April 26, 2013).",
    "total_rating": 96.2689865175066
  }
]

 */
