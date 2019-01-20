## Scraping module overview
Skyscraper supports several online and local sources when scraping data for your roms. This makes Skyscraper a hugely versatile tool since it also caches any resources that are gathered from any of the modules. The cached data can then be used to generate a game list and composite artwork later.

Choosing a scraping module is as simply as setting the `-s <MODULE>` option when running Skyscraper on the command line. It also requires a platform to be set with `-p <PLATFORM>`. If you leave out the `-s` option Skyscraper goes into *game list generation* mode and combines your cached data into a game list for the chosen platform and frontend. Read more about the resource cache [here](CACHE.md).

Below follows a description of all scraping modules.

#### ScreenScraper
* Shortname: `screenscraper`
* Website: *[www.screenscraper.fr](https://www.screenscraper.fr)*
* Type: *Rom checksum based, Exact file name based*
* User credential support: *Yes (`userid:passwd`), but not required*
* API request limit: *None*
* Thread limit: *1 or more depending on user credentials*
* Platform support: *[Check list under "Systémes"](https://www.screenscraper.fr)*
* Media support: `cover`, `screenshot`, `wheel`, `marquee`, `video`
* Example use: `Skyscraper -p snes -s screenscraper`

#### TheGamesDb (TGDB)
* Shortname: `thegamesdb`
* Website: *[www.thegamesdb.net](http://www.thegamesdb.net)*
* Type: *File name search based*
* User credential support: *Not required*
* API request limit: *Limited to 2000 requests per IP per month*
* Thread limit: *None*
* Platform support: *[Link to list](https://thegamesdb.net/list_platforms.php)*
* Media support: `cover`, `screenshot`
* Example use: `Skyscraper -p snes -s thegamesdb`

#### ArcadeDB by motoschifo
* Shortname: `arcadedb`
* Website: *[adb.arcadeitalia.net](http://adb.arcadeitalia.net), [youtube](https://www.youtube.com/c/ArcadeDatabase)*
* Contact: *arcadedatabase@gmail.com*
* Type: *Mame file name id based*
* User credential support: *None required*
* API request limit: *None*
* Thread limit: *1*
* Platform support: *Exclusively arcade platforms using official MAME files*
* Media support: `cover`, `screenshot`, `marquee`, `video`
* Example use: `Skyscraper -p fba -s arcadedb`

#### OpenRetro
* Shortname: `openretro`
* Website: *[www.openretro.org](https://www.openretro.org/)*
* Type: *WHDLoad uuid based, File name search based*
* User credential support: *None required*
* API request limit: *None*
* Thread limit: *1*
* Platform support: *Primarily Amiga, but supports others aswell. [Check list to the right](https://openretro.org/browse/amiga/a)*
* Media support: `cover`, `screenshot`, `marquee`
* Example use: `Skyscraper -p amiga -s openretro`

#### MobyGames
* Shortname: `mobygames`
* Website: *[www.mobygames.com](https://www.mobygames.com)*
* Type: *File name search based*
* User credential support: *None required*
* API request limit: *1 request per 10 seconds*
* Rom limit per run: *25*
* Thread limit: *1*
* Platform support: *[List](https://www.mobygames.com/browse/games)*
* Media support: `cover`, `screenshot`
* Example use:
  * `Skyscraper -p fba -s mobygames <FILE TO SCRAPE>`
  * `Skyscraper -p fba -s mobygames --startat <FILE TO START AT> --endat <FILE TO END AT>`
