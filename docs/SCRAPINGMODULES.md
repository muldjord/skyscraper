## Scraping module overview
Skyscraper supports several online and local sources when scraping data for your roms. This makes Skyscraper a hugely versatile tool since it also caches any resources that are gathered from any of the modules. The cached data can then be used to generate a game list and composite artwork later.

Choosing a scraping module is as simply as setting the `-s <MODULE>` option when running Skyscraper on the command line. It also requires a platform to be set with `-p <PLATFORM>`. If you leave out the `-s` option Skyscraper goes into *game list generation* mode and combines your cached data into a game list for the chosen platform and frontend. Read more about the resource cache [here](CACHE.md).

Below follows a description of all scraping modules.

#### ScreenScraper
* Shortname: *`screenscraper`*
* Type: *Online*
* Website: *[www.screenscraper.fr](https://www.screenscraper.fr)*
* Type: *Rom checksum based, Exact file name based*
* User credential support: *Yes (`userid:passwd`), but not required*
* API request limit: *None*
* Thread limit: *1 or more depending on user credentials*
* Platform support: *[Check list under "Systémes"](https://www.screenscraper.fr)*
* Media support: *`cover`, `screenshot`, `wheel`, `marquee`, `video`*
* Example use: `Skyscraper -p snes -s screenscraper`

#### TheGamesDb (TGDB)
* Shortname: *`thegamesdb`*
* Type: *Online*
* Website: *[www.thegamesdb.net](http://www.thegamesdb.net)*
* Type: *File name search based*
* User credential support: *Not required*
* API request limit: *Limited to 2000 requests per IP per month*
* Thread limit: *None*
* Platform support: *[Link to list](https://thegamesdb.net/list_platforms.php)*
* Media support: *`cover`, `screenshot`*
* Example use: `Skyscraper -p snes -s thegamesdb`

#### ArcadeDB by motoschifo
* Shortname: *`arcadedb`*
* Type: *Online*
* Website: *[adb.arcadeitalia.net](http://adb.arcadeitalia.net), [youtube](https://www.youtube.com/c/ArcadeDatabase)*
* Contact: *arcadedatabase@gmail.com*
* Type: *Mame file name id based*
* User credential support: *None required*
* API request limit: *None*
* Thread limit: *1*
* Platform support: *Exclusively arcade platforms using official MAME files*
* Media support: *`cover`, `screenshot`, `marquee`, `video`*
* Example use: `Skyscraper -p fba -s arcadedb`

#### OpenRetro
* Shortname: *`openretro`*
* Type: *Online*
* Website: *[www.openretro.org](https://www.openretro.org)*
* Type: *WHDLoad uuid based, File name search based*
* User credential support: *None required*
* API request limit: *None*
* Thread limit: *1*
* Platform support: *Primarily Amiga, but supports others aswell. [Check list to the right](https://openretro.org/browse/amiga/a)*
* Media support: *`cover`, `screenshot`, `marquee`*
* Example use: `Skyscraper -p amiga -s openretro`

#### MobyGames
* Shortname: *`mobygames`*
* Type: *Online*
* Website: *[www.mobygames.com](https://www.mobygames.com)*
* Type: *File name search based*
* User credential support: *None required*
* API request limit: *1 request per 10 seconds*
* Rom limit per run: *25*
* Thread limit: *1*
* Platform support: *[List](https://www.mobygames.com/browse/games)*
* Media support: *`cover`, `screenshot`*
* Example use:
  * `Skyscraper -p fba -s mobygames <SINGLE FILE TO SCRAPE>`
  * `Skyscraper -p fba -s mobygames --startat <FILE TO START AT> --endat <FILE TO END AT>`

#### IGDB
* Shortname: *`igdb`*
* Type: *Online*
* Website: *[www.igdb.com](https://www.igdb.com)*
* Type: *File name search based*
* User credential support: *None required*
* API request limit: *10000 requests per month shared among ALL Skyscraper users*
* Rom limit per run: *5*
* Thread limit: *1*
* Platform support: *[List](https://www.igdb.com/platforms)*
* Media support: *None*
* Example use:
  * `Skyscraper -p fba -s igdb <SINGLE FILE TO SCRAPE>`
  * `Skyscraper -p fba -s igdb --startat <FILE TO START AT> --endat <FILE TO END AT>`

#### World of Spectrum
* Shortname: *`worldofspectrum`*
* Type: *Online*
* Website: *[www.worldofspectrum.org](http://www.worldofspectrum.org)*
* Type: *File name search based*
* User credential support: *None required*
* API request limit: *None*
* Thread limit: *None*
* Platform support: *Exclusively ZX Spectrum games*
* Media support: *`cover`, `screenshot`*
* Example use: `Skyscraper -p zxspectrum -s worldofspectrum`

#### EmulationStation game list
* Shortname: *`esgamelist`*
* Type: *Local*
* Website: *[https://emulationstation.org](https://emulationstation.org)*
* Type: *Exact file name match*
* User credential support: *None required*
* API request limit: *None*
* Thread limit: *None*
* Platform support: *All*
* Media support: *`cover`, `screenshot`, `marquee`*
* Example use: `Skyscraper -p snes -s esgamelist`

#### Custom resource import scraper
* Shortname: *`import`*
* Type: *Local*
* Website: *[Skyscraper@github](https://github.com/muldjord/skyscraper)*
* Type: *Exact file name match*
* User credential support: *None required*
* API request limit: *None*
* Thread limit: *None*
* Platform support: *All*
* Media support: *`cover`, `screenshot`, `wheel`, `marquee`, `video`*
* Example use: `Skyscraper -p snes -s import`

Read a thorough descriptio of this module [here](IMPORT.md).
