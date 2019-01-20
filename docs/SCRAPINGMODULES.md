## Scraping module overview
Skyscraper supports several online and local sources when scraping data for your roms. This makes Skyscraper a hugely versatile tool since it also caches any resources that are gathered from any of the modules. The cached data can then be used to generate a game list and composite artwork later.

Choosing a scraping module is as simply as setting the `-s <MODULE>` option when running Skyscraper on the command line. It also requires a platform to be set with `-p <PLATFORM>`. If you leave out the `-s` option Skyscraper goes into *game list generation* mode and combines your cached data into a game list for the chosen platform and frontend. Read more about the resource cache [here](CACHE.md).

Below follows a description of all scraping modules.

#### ScreenScraper
* Shortname: `screenscraper`
* Website: *[www.screenscraper.fr](http://www.screenscraper.fr)*
* Type: *Rom checksum based, Exact file name based*
* User credential support: *yes (`userid:passwd`)*
* Thread limitation: *1 or more depending on user credentials*
* Example use: `Skyscraper -p snes -s screenscraper`
