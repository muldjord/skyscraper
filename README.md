# Skyscraper by Lars Muldjord
A powerful and versatile yet easy to use game scraper written in C++ for use with multiple frontends running on a Linux system (macOS and Windows too, but not officially supported). It scrapes and caches various game resources from various scraping sources, including media such as screenshot, cover and video. It then gives you the option to generate a game list and artwork for the chosen frontend by combining all of the cached resources.

All Skyscraper features are [well-documented](https://github.com/muldjord/skyscraper/tree/master/docs) and there's also a [F.A.Q](https://github.com/muldjord/skyscraper/blob/master/docs/FAQ.md). If you still have questions after reading the documentation, please consider asking them on the [RetroPie subreddit](https://www.reddit.com/r/RetroPie/) or in the official [RetroPie forums](https://retropie.org.uk/forum). The `Issues` page here on Github is for bug reports and feature requests only. Thanks!

#### Supported platforms (set with '-p'):
Check the full list of platforms [here](docs/PLATFORMS.md).

#### Supported scraping modules (set with '-s')
Skyscraper supports a variety of different scraping sources called *scraping modules*. Use these to gather game data into the Skyscraper resource cache. Check the full list of scraping modules [here](docs/SCRAPINGMODULES.md) and read more about the resource cache [here](docs/CACHE.md).

#### Supported frontends (set with '-f'):
* EmulationStation
* AttractMode
* Pegasus

## How to install Skyscraper
Follow the steps below to install the latest version of Skyscraper. Lines beginning with `$` signifies a command you need run in a terminal on the machine you wish to install it on.

NOTE! If you are using the RetroPie distribution, you have the option to install Skyscraper directly from the RetroPie-Setup script (*you need to update the script before installing it!*). Read more about all of that [here](https://retropie.org.uk/docs/Scraper/#lars-muldjords-skyscraper). If not, read on.

### Install prerequisites
#### Linux
Skyscraper needs Qt5.3 or later to compile. For a Retropie, Ubuntu or other Debian derived distro, you can install Qt5 using the following commands:
```
$ sudo apt-get update
$ sudo apt-get install build-essential qt5-default
```
You might be asked for your sudo password. On RetroPie the default password is `raspberry`. To install Qt5 on other Linux distributions, please refer to their documentation.

#### macOS
Skyscraper works perfectly on macOS as well but is not officially supported as I don't own a Mac. But with the help of HoraceAndTheSpider and abritinthebay here's the commands needed to install the Qt5 and other prerequisites:
```
$ /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
$ brew install gnu-tar --with-default-names
$ brew install wget
$ brew install qt5
$ brew link qt5 --force
```
If that went well, proceed to the default installation instructions below. It should work and give you a working installation of Skyscraper.

NOTE! User 'davidmgrantham' reports that the `--with-default-names` above might be deprecated for some macOS installations. If you remove it, you also need to download the `update_skyscraper.sh` and edit the `tar` commands to be `gtar` before running it.

### Download, compile and install
When you've installed the prerequisites as described above, you can install Skyscraper by typing in the following commands:
```
$ cd
$ mkdir skysource
$ cd skysource
$ wget -q -O - https://raw.githubusercontent.com/muldjord/skyscraper/master/update_skyscraper.sh | bash
```
The last command will download and run the latest update script from Github. During the installation you might be asked for your sudo password. On RetroPie the default password is `raspberry`.

When the script has completed you are ready to run Skyscraper!

### Updating Skyscraper
From Skyscraper 2.3.2 and newer you can update to the latest version simply by running the following commands:
```
$ cd
$ cd skysource
$ ./update_skyscraper.sh
```
You might be asked for your sudo password during the update. On RetroPie the default password is `raspberry`. If your version is older than 2.3.2 (check with `--help`) you need to follow the [installation instructions](#download-compile-and-install) instead.

### How to uninstall Skyscraper
If you've installed Skyscraper using the instructions in this readme, you can uninstall it using the following commands:
```
$ cd
$ cd skysource
$ sudo make uninstall
$ cd
$ rm -Rf skysource
$ rm -Rf .skyscraper
```
You might be asked for your sudo password during the processs. On RetroPie the default password is `raspberry`.

### Windows
Windows is not officially supported at this time! But I do semi-regularly compile and release an unsupported Windows 64-bit version that works just fine. Be sure to read the included README from the downloaded file before using it! And just to be clear: You are on your own if you use this version - please don't ask me questions about it. Get the Windows version [here](http://www.muldjord.com/downloads/Skyscraper_3.4.6_unsupported_win_version.zip).

## How to use Skyscraper
IMPORTANT!!! In order for Skyscraper to work properly, it is necessary to quit your frontend before running it! If you're running EmulationStation, you can quit it by pressing F4.

Remember, you can completely customize the artwork Skyscraper exports. Check out the documentation [here](docs/ARTWORK.md). If you just want to use the default (pretty cool looking) artwork Skyscraper provides, read on.

### A simple use case
For first-time users I recommend reading the short and to-the-point [use case](docs/USECASE.md). Please read it and get back here when you got the gist of it.

### A quick run-down of Skyscraper
Skyscraper is a command line tool, and has many, many options for you to fiddle around with. I recommend taking a look at all of them to familiarize yourself with the possibilites:
```
$ Skyscraper --help
```
This will give you a description of everything Skyscraper can do if you feel adventurous! For a thorough description of all available options, check [here](docs/CLIHELP.md).

The most important ones are probably:
* `-p <PLATFORM>`
* `-s <SCRAPING MODULE>`
* `-u <USER:PASS>` or `-u <KEY>`
* `--cache refresh`
* `--videos`
* `--unattend`

If you have your roms in a non-default location (default for RetroPie users is `/home/<USER>/RetroPie/roms/<PLATFORM>`) or wish to export the game list or artwork to non-default locations, you will also need these:
* `-i <PATH>`
* `-g <PATH>`
* `-o <PATH>`

For almost any command line option, consider setting them in the `/home/USER/.skyscraper/config.ini` file as described [here](docs/CONFIGINI.md). This will make the options permanent so you don't need to type them in all the time.

#### Gathering data for a subset of roms
Skyscraper offers several ways of gathering data for a subset of roms. If you just want to scrape the roms that have no data in the cache whatsoever, you can do so with the `--onlymissing` command-line option. You can also check out the `--startat FILENAME` and `--endat FILENAME` options. If you just want to gather data for a couple of roms you can simply add the filename(s) to the end of the command-line (eg. `Skyscraper -p amiga -s openretro "/path/to/rom name 1.lha" "/path/to/rom name 2.lha"`). And probably the most advanced (and quite handy) way to gather data for a subset of roms is to make use of the `--cache report:missing=RESOURCE` option. This can generate a report containing the filenames that are missing a certain resource. You can then feed the report back into Skyscraper with the `--fromfile REPORTFILE` afterwards. Skyscraper will then only scrape the files contained in the report.

### config.ini
A lesser known, but extremely useful, feature of Skyscraper is to add your desired config variables to `/home/USER/.skyscraper/config.ini`. Any options set in this file will be used by default by Skyscraper. So if you always use, for example, `-i <SOME FOLDER>` on command line, you can set the matching option `inputFolder="<SOME FOLDER>"` in the config.

For a full description of all availabe config options, check [here](docs/CONFIGINI.md).

### Resource cache
One of Skyscraper's most powerful features is the resource cache. It's important to understand how this works in order to use Skyscraper to its full potential. Read more about it [here](docs/CACHE.md).

### Custom data
I addition to allowing scraping from locally cached resources, Skyscraper also allows you to import your own data into the resource cache with the `-s import` scraping module. You can also edit existing resources in the cache or add individual `user` resources with the `--cache edit` command. Lastly, you also have the option of importing existing EmulationStation game list data into the Skyscraper resource cache if you need it. You can do this with the `-s esgamelist` scraping module.

To read more about any of the features described above, please check out all of the documentation [here](https://github.com/muldjord/skyscraper/tree/master/docs).

### Artwork look and effects
Check the full artwork documentation [here](docs/ARTWORK.md)

### Simple mode
Skyscraper also includes a *Simple mode* invoked simply by running Skyscraper with no command-line options like so:
```
$ Skyscraper
```
I do not recommend using this mode anymore as I consider it to be deprecated after Skyscraper was officially added to the RetroPie-Setup script.

In simple mode Skyscraper will ask you a bunch of questions, create a script based on your answers and finally run the script which scrapes the chosen platform using those options. This can be useful for first-time users, since it gives a bit of an overview of many of the things you can configure Skyscraper to do. If you're curious you can check out the generated script afterwards. It's located at `/home/USER/.skyscraper/skyscript.sh`.

NOTE: If you choose to use this mode, please be aware that many of the scraping modules have limits and user requirements that need to be considered when scraping any number of files. You should familiarize yourself [with these](docs/SCRAPINGMODULES.md) before using Skyscraper in general.

## Release notes

#### Version x.x.x (Features under consideration, all unimplemented)
* Add the option to scrape from cached data purely originating from certain scraping modules
* Consider making aliasMap the global baseName instead of just the search term base name. This will fix missing brackets in alias names being ignored later on
* Make all artwork types custom meaning that their type can be whatever the user wants. The sources will create ones with known types such as 'screenshot' and 'cover', but the user can import other types they define themselves, such as 'cabinet' or whatever else. Internally artwork is then handled by a list of artwork instead of 4 hardcoded types in GameEntry
* Introduce the ability to use <BASENAME> in the 'file="somefolder/<BASENAME>.png"' in artwork.xml which would then look for a file in 'resources' with the game basename.png allowing users to use their own custom artwork data as a workaround to adding new types to import (Thank you to 'jueank' for suggesting this)
* Add the option of scraping custom platforms by configuring them in the config with an alias to an already existing platform. Example: scrape 'pcenginecd' could be scraped as 'pcengine' in case you have those files in a 'roms/pcenginecd' folder instead of the pcengine folder. Check here: https://github.com/muldjord/skyscraper/issues/136
* Create a testmode for the artwork compositor that let's you quickly render an example to see if you got everything set up right in the artwork xml
* Allow 'region' to be a list similar to 'regionPrios'. When using 'region' it should simply keep the default priority list and add those from 'region' to the top. 'regionPrios' should still overwrite it entirely. Naming change probably a good idea, for instance rename 'region' to 'regionsPrefer' or something. 'regionPrios' should probably also be changed to 'regionsOverride'. (Thank you to 'corezon' for suggesting this).
* Implemented a less ridig filename match for the 'import' module to allow for close match filenames

#### Version 3.5.2 (29th April 2020)
* Hid deprecated options from '--help'. Deprecated options still work just aren't shown
* Updated 'Simple Mode' to use the new '--flags' options
* Removed deprecated '<cover>' node and replaced it with '<thumbnail>' for ES gamelist generation (Thank you to 'yisraeldov' for letting me know)
* Added simple error handling for media network requests
* '--flags help' and '--cache help' no longer requires '-p PLATFORM' to be set
* Added video conversion support through config.ini option 'videoConvertCommand="COMMAND %i %o"' option
* Changed 'worldofspectrum' to https

#### Version 3.5.1 (28th April 2020)
* Fixed minor bug in 'daphne' when it only has one rom in 'daphne/roms'

#### Version 3.5.0 (27th April 2020)
* For 'screenscraper', 'thegamesdb', 'mobygames' and 'arcadedb' Skyscraper will now no longer fetch the artworks if user has requested it to remain uncached (Thank you to user 'herbymachine' for the request)
* Added '--skipexistingmedia' option to allow for faster gamelist generation in cases where some artwork already exists in the gamelist media folder (Thank you to 'jacrify' for suggesting this)
* Added '--flags' command-line option that collects certain flag-like options into one
* Added deprecated warnings to all options that are now moved to the '--flags' option
* Added '*.cue' extension to '3do' platform (Thank you to 'therealdealneil' for the suggestion)
* Implemented a hack for the 'daphne' platform that will add the game info to the 'game.daphne' folders instead of the 'game.zip' files (Thank you to 'AlCzervik' for pointing this out)

#### Version 3.4.10 (20th April 2020)
* Re-implemented double-check when saving artwork to cache to avoid 0-byte files on systems with a broken libpng (Thank you to 'jacrify' for letting me know)

#### Version 3.4.9 (17th April 2020)
* Added 'wiiu' platform (Thank you to 'minilandl' for suggesting it)

#### Version 3.4.8 (14th April 2020)
* Lowered sanity check limit of 1024 bytes for artwork from 'screenscraper' to 256 bytes (Thank you to user 'Paul-Colucci' for reporting this)

#### Version 3.4.7 (12th April 2020)
* If '--onlymissing' is set the initial first found rom check is ignored (Thank you to user 'Paul-Colucci' for reporting this)

#### Version 3.4.6 (9th April 2020)
* Implemented nameTemplate as config.ini option. See docs/CONFIGINI.md doc for explanation (Thank you to user 'vg8020' for suggesting this)

#### Version 3.4.5 (7th April 2020)
* Minor fix to 'screenscraper' romnom query

#### Version 3.4.4 (7th April 2020)
* Remove '*.bin' support for 'saturn' platform since '*.cue' is already in there. This caused double-entries in gamelists (Thank you to user 'joaoluizcarvalho' for pointing this out)
* Now handles faulty login (incorrectly entered in config.ini) better with 'screenscraper'
* Fixed bug that caused 'pegasus' launch command to be entered incorrectly (Thank you to user 'Itetrel' for letting me know)
* Added '--onlymissing' CLI option which allows users to skip files that already have any kind of data cached from any source
* Now clearly states what happened when a request timed out when using 'screenscraper' (Thank you to 'seriema' for pointing this out)

#### Version 3.4.3 (14th March 2020)
* All arcade platforms now use 'flyer' from 'screenscraper' for cover artwork instead of 'box-2D' (Thank you to user 'aidy80s' for suggesting this)

#### Version 3.4.2 (26th February 2020)
* Improved filename versioning removal for optimal query generation
* Updated 'mameMap.csv' with latest Mame v0.218 listings
* Added support for 'wheel' artwork type in 'arcadedb' scraping module
* Now supports symlinked paths inside of the input folder (Thank you to rhunjadi for pointing this out)

#### Version 3.4.1 (11th February 2020)
* Further optimized artwork space requirements. Now checks if original takes up less space than resized artwork, then forces use of original for those cases
* The 'thegamesdb' module now also supports wheel and marquee for the games that have them (Thank you to 'tv21' for pointing this out)
* Updated developer and publisher json list for 'thegamesdb'

#### Version 3.4.0 (4th February 2020)
* Disabled config.ini migration as most people should be migrated now
* Added 'launcher' option to 'simple mode' when using 'pegasus' frontend
* Added 'excludeFiles' config option that allows excluding certain files when scraping (Thank you to 'timothybrown' for suggesting this)
* Added 'includeFiles' config option that allows only including certain files when scraping
* Added '--excludefiles' cli option that allows excluding certain files when scraping
* Added '--includefiles' cli option that allows only including certain files when scraping
* If 'noresize' is set all images are now saved to cache in their original format and size instead of always converting them to PNG's (Thank you to 'krcroft' for requesting this)
* Added 'jpgQuality' config option that sets the default JPG quality (0-100) when '--noresize' is NOT set. Screenshots and images with transparency are still saved as PNG's.
* Upped 'screenscraper' request limiter to 1.2 seconds per request to avoid 'maximum threads per minute reached' error message

#### Version 3.3.8 (12th January 2020)
* Fixed bug in platform alias for 'pc (microsoft windows)' which was missing the last ')' bracket (Thank you to 'o0alucard0o' for reporting this)

#### Version 3.3.7 (8th January 2020)
* Added the option to add frontend specific config sections to set artwork xml and other relevant configuration parameters per frontend

#### Version 3.3.6 (5th January 2020)
* Skipped file now includes platform in the filename (Thanks to 'parker-hemphill' for suggesting this)
* Now handles the '****' error message from ScreenScraper (Thank you to 'GiulioCentorame' for reporting this)
* Updated 'thegamesdb' to include the 'v1' prefix (Thank you to 'Clyde' for letting me know)

#### Version 3.3.5 (15th December 2019)
* Updated 'screenscraper' API to support new system name json format (Thank you to 'Micket' for letting me know)

#### Version 3.3.4 (8th December 2019)
* 'igdb' module global key has been removed from Skyscraper. They have moved to free personal keys instead (this is good news!). To get one, register at https://api.igdb.com (Thank you to 'stlbluesfan' for letting me know)
* Fixed disk space check so it also works for 'attractmode' frontend
* Updated 'attractmode' to save overviews in correct location and format (Thanks to 'barolo' for letting me know, thanks to 'teeedubb' and 'barolo' for testing)
* For 'attractmode' added semicolon detection for 'snaps'/'video' parsing for determining resource folders (Thanks to 'barolo' for pointing this out)
* Removed rom limit on 'igdb' module

#### Version 3.3.3 (21st November 2019)
* Added support for the Nintendo 3DS platform as '3ds' (Thank you to 'barolo' for suggesting this)
* Fixed system id for 'coleco' platform when using the 'screenscraper' module causing roms not to be recognized (Thank you to 'DMahlon' for reporting this)

#### Version 3.3.2 (4th November 2019)
* Added '.gcm' file extension to Gamecube platform (Thank you to 'mrsilver76' for suggesting this)
* The '-e' option can now be used to optionally set the Pegasus launch command (Thank you to Sixdd6' for suggesting this)
* Refined 'Found/Skipped' output a bit

#### Version 3.3.1 (17th October 2019)
* Added new '--cache edit:new=<TYPE>' option for efficient batch adding of a resource of a certain type
* Improved ctrl+c handling
* Protected cache write calls from ctrl+c obstruction

#### Version 3.3.0 (15th September 2019)
* MAJOR: File identification now uses new quick id method for up to 75% faster processing (Thank you to 'langest' for finally making me look into this)
* Added 'pc98' platform (Thank you to 'leosmeira' for suggesting it)
* Added 'pokemini' platform (Thank you to 'leosmeira' for suggesting it)
* Renamed all 'sha1' file id's to 'id' as sha1 was misleading
* Changed relevant defines to constexpr
* ScreenScraper now always prioritizes the 'video-normalized' above 'video' (Thank you to 'JuanVCS' for suggesting this)
* Fixed bug in ScreenScraper retry code which made it retry more than necessary

#### Version 3.2.8 (31st August 2019)
* `--cache report` now supports symlinks (Thank you to 'vanativo' for reporting this)

#### Version 3.2.7 (31st August 2019)
* The '--cache report' option now adhere to the '--nosubdirs' option (Thank you to 'vanativo' for reporting this)

#### Version 3.2.6 (24th August 2019)
* Fixed bug where entering a correct fronted in simple mode would loop the question (Thank you to 'thadboyd' for reporting this)

#### Version 3.2.5 (21st August 2019)
* EmulationStation module now converts to '<folder>' entry for all supported platform suffixes (Thank you to 'kgonepostl' for reporting this)

#### Version 3.2.4 (19th August 2019)
* Implemented JSON flaw workaround for the ScreenScraper module

#### Version 3.2.3 (13th August 2019)
* MAJOR: Added support for the 'Pegasus' frontend (set with '-f pegasus')
* Now checks for, and removes, double bracket notes in final game list title
* Fixed minor bug in the 3D gamebox effect renderer
* Completely transparent images are no longer saved when compositing (Thank you to metallkopf for getting me to finally fix this)
* Optimized the final game list assembling code to use game entry references instead of copies
* Optimized all cache resource iterations to use const references instead of copies
* Optimized the entire codebase by removing all Qt-centric foreach iterate-by-copy to use references instead
* Fixed bug where 'screenscraper' would only look for ESRB age classification
* Potential faulty JSON replies from Screenscraper are now saved to '/home/USER/.skyscraper/screenscraper_error.json' for easier debugging

#### Version 3.2.2 (3rd August 2019)
* Added 'bat' scripts to sha1 special handling list (please purge platforms using 'bat' files and rescrape)
* Now discards 'ZZZ(notgame)' results from ScreenScraper
* Fixed double-quote issue when reading titles from scummvm.ini
* Made location of scummvm.ini configurable in config.ini
* Fixed bug in roman and integer numeral conversion functions
* Rewrote the entire 'screenscraper' module to use JSON instead of XML

#### Version 3.2.1 (15th July 2019)
* Fixed bug in 'mobygames' module that could cause an endless loop while getting cover

#### Version 3.2.0 (8th July 2019)
* Improved output of edit mode to show queue size and initial prioritized resources
* Now clearly shows missing resources when adding resources in editing mode
* Made '--cache vacuum' more verbose at verbosity level 2 or higher
* Removed platform check for screenscraper module as it wasn't really necessary
* Added 'videoSizeLimit' config variable (Thank you to 'silverfr0st' for suggesting this)
* Now also processes brackets for skipped games (Thanks to 'hexive' for pointing this out)
* Added 'arcadia' and 'astrocde' MESS platforms (Thank you to 'raphkoster' for suggesting these)
* Added '--cache report:missing=<GROUP or RESOURCE1,2,...>' option (Thank you to JaymesRS for suggesting this)
* Added '--fromfile' option allowing you to work on a subset of files taken from a file. Compatible with files created using the '--cache report' option
* MobyGames module now includes 'platform' id query when relevant. Should improve result accuracy
* Added region support for MobyGames cover artwork
* Added A LOT of regions to region autodetection in general
* Now no longer creates media folders when in resource gathering mode
* Moved help texts for '--cache' options to '--cache help'

#### Version 3.1.4 (23rd March 2019)
* Refactored network communication class
* Added request download size limit to work around possible bug in ScreenScraper API

#### Version 3.1.3 (12th March 2019)
* Improved 'screenscraper' error handling

#### Version 3.1.2 (10th March 2019)
* Added support for Moonlight '*.ml' extension for all platforms
* Added 'ps2' platform for use with Moonlight and all other supported Ps2 extensions
* Added 'steam' platform for use with Moonlight '*.ml' files
* Fixed minor bug in CRC checksum calculator for 'screenscraper'
* Improved workarounds for ScreenScraper XML errors

#### Version 3.1.1 (8th March 2019)
* Worked around a bug in the 'screenscraper' xml output improving stability
* Added limiter to media getter functions for 'screenscraper' which should improve stability

#### Version 3.1.0 (2th March 2019)
* MAJOR: Added '--cache edit' command line option which allows viewing, editing and deleting cached resources for the roms in the queue. Narrow the queue down by providing file names on command line or by using '--startat <FILENAME>' and '--endat <FILENAME>'
* Added 'zx81' platform. Note! The only module that supports it is the 'screenscraper' module

#### Version 3.0.5 (26th February 2019)
* Fixed and improved ES metadata preservation code

#### Version 3.0.4 (18th February 2019)
* Improved Amiga CD32 disc image support to prep for the upcoming support in Amiberry (Thank you to 'HoraceAndTheSpider' for the heads up)
* Added check for Qt5.4.0 and ignores QStorageInfo include if requirement not met (Thank you to 'ByteThis' for reporting this issue)
* Improved work around to fix invalid XML returned by ScreenScraper

#### Version 3.0.3 (16th February 2019)
* Worked around a flaw in the returned screenscraper xml results which caused many games to not be recognized due to invalid xml being returned (Thank you to 'Brunnis' for reporting this)
* Improved SIGINT handler to now allow unclean exit if user is VERY adament about it

#### Version 3.0.2 (14th February 2019)
* Upped the rom limit from 5 to 35 for the "igdb" module
* Upped the rom limit from 25 to 35 for the "mobygames" module
* Added media cache config options to module section
* Add Sharp X1 platform as "x1"
* Now exits nicely when running low on disk space
* Added 'spaceCheck=<BOOL>' to config.ini
* Fixed crash when using '--startat' and '--endat' where the '--endat' file name came before the '--startat' file name. Note! What 'ls' reports in alphabetical order is not always what Skyscraper see as it it locale specific. So be aware of this. A huge thanks to 'Gemba' for taking the time to investigate this bug thoroughly.
* Fixed bug in game list metadata preservation when using relativePaths and '<folder>' entries (thank you to 'HumanRob' for reporting this)
* Fixed game list entries skipping for 'relativePaths' and '<folder>' instances
* Skyscraper now saves the cache and exits nicely on ctrl+c (SIGINT) (thanks to 'krcroft' for pointing this out)
* The 'screenscraper' module now includes 'systemeid' in the query for better results
* Now skips the game list assembling when in gathering mode
* Now skips cache saving when in game list generation mode
* Output now says whether it was a gathering run or a game list generation run

#### Version 3.0.1 (27th January 2019)
* Added note for '--query' option when user forgets to add a file name on command line
* Fixed minor bugs in aliasMap, mameMap and whdLoad map lookups

#### Version 3.0.0 (23rd January 2019)
* Game list creation and media compositing now only processed when leaving out the "-s" option
* Now converts textual "\n" in descriptions to newlines. Useful when importing textual data (Thank you to "scthirouin" for requesting it)
* Added disk space check. Now aborts if disk has less than 200 megs left
* Updated "igdb" API til APIv3000. Key now global for all users
* Fixed ES game list release dates by adding "T000000" (Thank you to "michaelbaudino" for noticing)
* Added "subdirs" to config.ini (Thank you to "otherguy" for noticing it was missing)
* "--startat" and "--endat" now supports current workdir file paths
* Changed "allowExtension" config variable to "addExtensions"
* Changed "noHints" config variable to "hints"
* Changed "noBrackets" config variable to "brackets"
* Changed "noResize" config variable to "resize"
* Removed "--updatedb" cli option as it was deprecated (use "--cache refresh" instead)
* Removed "--nolocaldb" cli option as it is irrelevant after 3.0.0 usage changes
* Changed "--allowext" cli option to "--addext"
* Removed "localDb"/"cache" config sections and moved options to "main"
* Removed "userCreds" option from main and platform sections in config.ini. They should be set in scraping module section instead
* Removed "scraper" config.ini variable as it made no sense to ever set it like that
* Changed "startat" config.ini option to "startAt" to conform with style of other variables
* Changed "endat" config.ini option to "endAt" to conform with style of other variables
* Fixed "--relative" option when combined with roms in subdirs
* "--unattend" options now answers "y" to all "--cache purge:" prompts automatically
* Added "regionPrios" and "langPrios" variables to config.ini for main and platform sections
* Added auto-migration from old config.ini format to v3.0.0 format
* Added auto-migration from "dbs" folder to "cache" folder
* Optimized "--cache validate" A LOT
* Now converts ES game list entries to <folder> entries if a .cue or .m3u file is found in a subfolder. This causes the information and artwork to be shown when selecting the folder in ES rather than the .cue or .m3u file inside the subfolder. This is very useful for psx scraping

#### Version 2.9.5 (22nd December 2018)
* MAJOR: Added option "--purgedb vacuum" which vacuums all resources not related to your current romset. Remember to make backups of your cache before using this
* MAJOR: Added option "--purgedb all" that purges all resources for the selected platform. Remember to make backups of your cache before using this
* MAJOR: Added "--symlink" option which forces cached videos to be symlinked to destination instead of being copied when scraping with the "localdb" scraping module
* MAJOR: Added "esgamelist" emulationstation gamelist.xml scraping module. Contributed by "mgerhardy". Rewritten by me to better conform to Skyscraper design
* MAJOR: Added aliasMap.csv that forces the use of a title alias when searching for specific filenames
* Removed version bracket tag for Amiga lha files
* Improved getCompareTitle for mame games and lha files
* Code cleanup for sqrNotes
* Added the "ti99" platform. (Thank you to "jhbeskow" for suggesting it)

#### Version 2.9.1 (15th December 2018)
* Fixed bug in "screenscraper" module when using --unpack where files exceeding limits would get checksummed as the "Point Blank" game. (Thank you to CookiePLMonster for reporting this issue)
* Raised --unpack size limit to 8 megs

#### Version 2.9.0 (13th December 2018)
* MAJOR: Now looks up Amiga lha files in the Amiberry "whdload_db.xml" and retrieves data from "openretro.org" based on the uuid from the xml
* Added search based fallback pass for Amiga when game isn't found via uuid
* Added "<scanlines>" effect to compositor. Check [here](docs/ARTWORK.md) for more info (Thank you to "jakejm79" for suggesting this)
* "mobygames" module now uses https
* Fixed bug in "openretro" module where "developer" would potentially scrape wrong under certain circumstances
* Improved "description" scraping for "openretro" module
* Improved bracket tag handling for Amiga lha files A LOT
* Fixed minor pass 2 bug when using search based sources
* Added a pass for integer to roman conversion for search based sources (eg. "4" converts to "IV")
* "the" matching now uses regular expression for better precision
* Added "mame-*" platforms to "mameMap" name load list

#### Older releases
Release notes for older releases can be found [here](OLDERRELEASES.md).
