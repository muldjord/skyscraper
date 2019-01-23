# Skyscraper by Lars Muldjord
A powerful and versatile yet easy to use game scraper written in C++ for use with multiple frontends running on a Linux system. It scrapes and caches various game resources from various scraping sources, including media such as screenshot, cover and video. It then gives you the option to generate a game list and artwork for the chosen frontend by combining all of the cached resources.

Any exported artwork can be customized completely. Check the documentation for that [here](docs/ARTWORK.md).

#### Supported platforms (set with '-p'):
Check the full list of platforms [here](docs/PLATFORMS.md).

#### Supported scraping modules (set with '-s')
Skyscraper supports a variety of different scraping sources called *scraping modules*. Use these to gather game data into the Skyscraper resource cache for later use when generating game lists for your chosen frontend. Check the full list of scraping modules [here](docs/SCRAPINGMODULES.md).

##### Generating a game list with composited artwork
Skyscraper can use all previously cached resources to generate a game list and composite artwork as described in `~/.skyscraper/artwork.xml`. Enable the *game list generator* by simply leaving out the `-s` option entirely. Read more about the resource cache [here](docs/CACHE.md).

#### Supported frontends (set with '-f'):
* EmulationStation
* AttractMode

## Patreon
Yes, [I have one](https://www.patreon.com/muldjord). Absolutely NOT a requirement if you want to use Skyscraper! But there it is. And please understand that becoming a patron does in no way give you any special say in what features I work on or how I develop Skyscraper in the future. It will be seen as an appreciative gesture, nothing else. :)

## How to install Skyscraper
Follow the steps below to install the latest version of Skyscraper. Lines beginning with `$` signifies a command you need run in a terminal on the machine you wish to install it on.

### Install prerequisites
#### Linux
Skyscraper needs Qt5.3 or later to compile. For a Retropie, Ubuntu or other Debian derived distro, you can install it using the following commands:
```
$ sudo apt-get update
$ sudo apt-get install qt5-default
```
You might be asked for your sudo password. On RetroPie the default password is `raspberry`. To install Qt5 on other Linux distributions, please refer to their documentation.

#### OSX
Skyscraper works perfectly on OSX aswell but is not officially supported as I don't own a Mac. But with the help of HoraceAndTheSpider and abritinthebay here's the commands needed to install the Qt5 and other prerequisites:
```
$ /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
$ brew install gnu-tar --with-default-names
$ brew install wget
$ brew install qt5
$ brew link qt5 --force
```
If that went well, proceed to the default installation instructions below. It should work and give you a working installation of Skyscraper.

#### Windows
Windows is not supported at this time.

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

## How to uninstall Skyscraper
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

## How to use Skyscraper
IMPORTANT!!! In order for Skyscraper to work properly, it is necessary to quit your frontend before running it! If you're running EmulationStation, you can quit it by pressing F4.

Remember, you can completely customize the artwork Skyscraper exports. Check out the documentation [here](docs/ARTWORK.md). If you just want to use the default (pretty cool looking) artwork Skyscraper provides, read on.

### A simple use case
For first-time users I recommend reading the short and to-the-point [use case](docs/USECASE.md). Please read it and if it catches your interest check out the advanced options [here](#manual-mode-for-advanced-users). If you just want to scrape a platform and be done with it check [Simple mode](#simple-mode).

### Simple mode
When you have completed the installation you can start Skyscraper in *Simple mode* by running Skyscraper with no command line options by typing:
```
$ Skyscraper
```
Skyscraper will then ask you a bunch of questions, create an optimized script based on your answers, and finally run the script which scrapes the chosen platform in an optimal way. This is very useful for first time scrapings, as it will give you the best possible initial result for any given platform. If you're curious you can check out the generated script afterwards. It's located in `~/.skyscraper/skyscript.sh`.

### Manual mode (for advanced users)
Skyscraper is a command line tool, and has many, many options for you to fiddle around with. I recommend taking a look at all of them to familiarize yourself with the possibilites:
```
$ Skyscraper --help
```
This will give you a description of everything Skyscraper can do if you feel adventurous! For a thorough description of all available options, check [here](docs/CLIHELP.md).

Let's go over the most important ones:

* `-p <PLATFORM>`: This tells Skyscraper which platform you wish to scrape during this scraping run. Check the full list of platforms under the `-p` option with `--help`
* `-s <SCRAPING MODULE>`: This tells Skyscraper where you would like to scrape and cache resources from. Read more about that [here](docs/CACHE.md)
* `--cache refresh`: Whenever you scrape a platform with any scraping module Skyscraper caches all of that data locally. When you rescrape a platform it will fetch the data from the cache instead of hammering the online servers. Using the `--cache refresh` option allows you to override this and tells Skyscraper to refresh the cached data directly from the online source. Please only use this option if you know the data you want to scrape has changed at the source
* `--videos`: If you wish to scrape videos for the scraping modules that support it, you need to add the `--videos` option. This is disabled by default because of the significant space requirements needed to save them
* `--unattend`: This just bypasses any questions at the beginning of a scraping run. Setting `--unattend` will then always answer yes to overwriting an existing game list and not skip existing entries

If you have your roms in a non-default location (default is `/home/<user>/RetroPie/roms/<platform>`) or wish to export the game list or artwork to non-default locations, you will also need these:
* `-i <PATH>`: Sets a non-default rom input folder
* `-g <PATH>`: Sets a non-default game list export folder (defaults to the same as rom input folder if scraping for EmulationStation)
* `-o <PATH>`: Sets a non-default artwork export folder (defaults to the same as game list export folder + `/media` if scraping for EmulationStation)

For almost any command line option, consider setting them in the `~/.skyscraper/config.ini` file as described [here](#docs/CONFIGINI.md). This will make the options permanent so you don't need to type them in all the time.

#### Scraping and caching single roms or a subset of roms
Sometimes you'd want to update the cached data for a single or a subset of roms. Skyscraper allows this by letting you either provide one or more single rom filenames to be added to the end of a command line OR by using the `--startat` and `--endat` options (read more about those with `--help`). For single roms, here's an example: `Skyscraper -p amiga -s openretro "/path/to/rom name.lha"`. Be aware that this only updates the resource cache for this particular rom. It DOES NOT update it in your game list. To do so you need to regenerate the game list by simply leaving out the `-s` option entirely like so `Skyscraper -p [platform]`.

### config.ini
A lesser known, but extremely useful, feature of Skyscraper is to add your desired config variables to `~/.skyscraper/config.ini`. Any options set in this file will be used by default by Skyscraper. So if you always use, for example, `-i [some folder]` on command line, you can set the matching option `inputFolder="[some folder]"` in the config.

For a full description of all availabe config options, check [here](docs/CONFIGINI.md).

### Resource cache
One of Skyscraper's most powerful features is the resource cache. It's important to understand how this works in order to use Skyscraper to its full potential. Read more about it [here](docs/CACHE.md).

### Custom data import
I addition to allowing scraping from locally cached resources, Skyscraper also allows you to import your own data into the resource cache with the `-s import` scraping module which in turn allows you to scrape your roms with the data. Read more about how this works [here](docs/IMPORT.md).

### Artwork look and effects
Check the full artwork documentation [here](docs/ARTWORK.md)

## Release notes

#### Version x.x.x (still unimplemented)
* Add "sharpen" effect which sharpens the image
* Improved "blur" and "shadow" effect to be true gaussian
* Add the option to edit cached data for any game
* Add the option to scrape from cached data purely originating from certain scraping modules
* Consider making aliasMap the global baseName instead of just the search term base name. This will fix missing brackets in alias names being ignored later on

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
