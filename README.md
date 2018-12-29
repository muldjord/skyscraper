# Skyscraper by Lars Muldjord
A powerful and versatile yet easy to use game scraper written in C++ for use with multiple frontends running on a Linux system. It scrapes and caches various game resources from various scraping sources, including media such as screenshot, cover and video. It then gives you the option to combine all of these resources into the most complete results by using the provided `localdb` scraping module.

Any exported artwork can be customized completely. Check the documentation for that [here](ARTWORK.md).

#### Currently supports the following frontends (set with '-f'):
* EmulationStation
* AttractMode

#### Currently supports the following platforms (set with '-p'):
Check the full list of platforms [here](PLATFORMS.md).

#### Currently supports the following scraping sources (set with '-s')
* ONLINE: screenscraper.fr
* ONLINE: openretro.org
* ONLINE: thegamesdb.net
* ONLINE: worldofspectrum.org
* ONLINE: adb.arcadeitalia.net (Arcade Database by motoschifo, arcadedatabase@gmail.com, [youtube](https://www.youtube.com/c/ArcadeDatabase))
* ONLINE: mobygames.com
* ONLINE: igdb.com
* LOCAL: localdb (scrapes exclusively from cached resources. Read more [here](#local-database-features))
* LOCAL: import (imports resources into the local cache. Read more about this [here](import/README.md))
* LOCAL: esgamelist (by Lars Muldjord and mgerhardy. Scrapes data from an EmulationStation gamelist.xml)

... More scraping sources will be added in future releases!

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

Remember, you can completely customize the artwork Skyscraper exports. Check out the documentation [here](ARTWORK.md). If you just want to use the default (pretty cool looking) artwork Skyscraper provides, read on.

### A simple use case
For first-time users I recommend reading the short and to-the-point [use case](USECASE.md). Please read it and if it catches your interest check out the advanced options [here](#manual-mode-for-advanced-users). If you just want to scrape a platform and be done with it check [Simple mode](#simple-mode).

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
This will give you a description of everything Skyscraper can do if you feel adventurous! Let's go over the most important ones:

* `-p [platform]`: This tells Skyscraper which platform you wish to scrape during this scraping run. Check the full list of platforms under the `-p` option with `--help`
* `-s [scraping module]`: This tells Skyscraper where you would like to fetch data from. The most important one being the `localdb` module. Read more about that [here](dbs/README.md)
* `--refresh`: Whenever you scrape a platform with any scraping module Skyscraper caches all of that data locally. When you rescrape a platform it will fetch the data from the cache instead of hammering the online servers. Using the `--refresh` option allows you to override this and tells Skyscraper to refresh the cached data directly from the online source. Please only use this option if you know the data you want to scrape has changed at the source
* `--videos`: If you wish to scrape videos for the scraping modules that support it, you need to add the `--videos` option. This is disabled by default because of the significant space requirements needed to save them
* `--pretend`: This bypasses any game list generation and artwork compositing code, but still caches any incoming data. Useful when scraping with the non-localdb modules, as it's faster while you are just gathering data into your localdb cache
* `--unattend`: This just bypasses any questions at the beginning of a scraping run. Setting `--unattend` will then always answer yes to overwriting an existing game list and not skip existing entries

If you have your roms in a non-default location (default is `/home/[user]/RetroPie/roms/[platform]`) or wish to export the game list or artwork to non-default locations, you will also need these:
* `-i [path]`: Sets a non-default rom input folder
* `-g [path]`: Sets a non-default game list export folder (defaults to the same as rom input folder if scraping for EmulationStation)
* `-o [path]`: Sets a non-default artwork export folder (defaults to the same as game list export folder + `/media` if scraping for EmulationStation)

For almost any command line option, consider setting them in the `~/.skyscraper/config.ini` as described [here](#configini). This will make the options permanent so you don't need to type them in all the time.

#### Scraping and caching single roms or a subset of roms
Sometimes you'd want to update the cached data for a single or a subset of roms. Skyscraper allows this by letting you either provide one or more single rom filenames to be added to the end of a command line OR by using the `--startat` and `--endat` options (read more about those with `--help`). For single roms, here's an example: `Skyscraper -p amiga -s openretro "/path/to/rom name.lha"`. Be aware that this only updates the local cache for this particular rom. It DOES NOT update it in your game list. To do so you need to rescrape the platform using the cached data with the command `Skyscraper -p [platform]`. This implicitly adds `-s localdb` which is the default scraper and scrapes the platform using all of your previously cached data.

### config.ini
A lesser known, but extremely useful, feature of Skyscraper is to add your desired config variables to `~/.skyscraper/config.ini`. Any options set in this file will be used by default by Skyscraper. So if you always use, for example, `-i [some folder]` on command line, you can set the matching option `inputFolder="[some folder]"` in the config.

Many options can be set on two levels; either `[main]` or `[amiga]`. `amiga` can be any of the supported platforms (check list with `--help` under the `-p` option), in which case the settings will only be applied while scraping that particular platform. Settings in the `[main]` section will be used while scraping any platform.

For options dedicated to a single scraping module, you can create sections for each of them. For instance, you can create a `[screenscraper]` section and add a `userCreds="user:pass"` line. Doing this will always use these credentials when scraping from the `screenscraper` scraping module.

Lastly, you can also add a `[localDb]` section to enable / disable the caching of certain resource types.

You can find an example config file at `~/.skyscraper/config.ini.example`. This file contains all available options. Just copy the file to `config.ini` and uncomment and edit the ones you wish to use by removing the `#` in front of the variables.

### Local database cache
One of Skyscraper's most powerful features is the local database cache. It's important to understand how this works in order to use Skyscraper to its full potential. Read more about it [here](dbs/README.md).

### Custom data import
I addition to allowing scraping from locally cached resources, Skyscraper also allows you to import your own data into the local cache with the `-s import` scraping module which in turn allows you to scrape your roms with the data. Read more about how this works [here](import/README.md).

### Artwork look and effects
Check the full artwork documentation [here](ARTWORK.md)

## Release notes

#### Version x.x.x (still unimplemented)
* Add "sharpen" effect which sharpens the image
* Improved "blur" and "shadow" effect to be true gaussian
* Add the option to edit cached data for any game
* Add the option to scrape from cached data purely originating from certain scraping modules
* Add bool check for writing cache and game list media files in case disk has run out of space. Output any problems to stdout

#### Version 3.0.0 (In progress, unreleased)
* '--pretend' now default for all scraping modules except 'localdb'

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
* Added "<scanlines>" effect to compositor. Check [here](ARTWORK.md) for more info (Thank you to "jakejm79" for suggesting this)
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
