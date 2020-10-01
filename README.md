# Skyscraper by Lars Muldjord
A powerful and versatile yet easy to use game scraper written in C++ to  use with multiple frontends running on a Linux system (macOS and Windows too, but not officially supported). It scrapes and caches various game resources from various scraping sources, including media such as screenshot, cover and video. It then gives you the option to generate a game list and artwork for the chosen frontend by combining all of the cached resources.

All Skyscraper features are [well-documented](https://github.com/muldjord/skyscraper/tree/master/docs) and there's also a [F.A.Q](https://github.com/muldjord/skyscraper/blob/master/docs/FAQ.md). If you still have questions after reading the documentation, please consider asking them on the [RetroPie subreddit](https://www.reddit.com/r/RetroPie/) or in the official [RetroPie forums](https://retropie.org.uk/forum). The `Issues` page here on Github is for bug reports and feature requests only. Thanks!

##### Code contributions
Pull Requests are not currently accepted. Due to wanting to pursue other projects after having worked on Skyscraper for 3 years, I am currently only fixing scraper-breaking bugs in Skyscraper. For any functionality beyond what is already in place, feel free to fork Skyscraper and fit it to your needs. I am no longer actively considering or commenting user feature requests or pull requests. When / if this changes, this text will be updated.

#### Supported platforms (set with '-p'):
Check the full list of platforms [here](docs/PLATFORMS.md).

#### Supported scraping modules (set with '-s')
Skyscraper supports a variety of different scraping sources called *scraping modules*. Use these to gather game data into the Skyscraper resource cache. Check the full list of scraping modules [here](docs/SCRAPINGMODULES.md) and read more about the resource cache [here](docs/CACHE.md).

#### Supported frontends (set with '-f'):
* EmulationStation
* AttractMode
* Pegasus

## How to install Skyscraper
Follow the steps mentioned below to install the latest version of Skyscraper. Lines beginning with `$` signifies a command you need run in a terminal on the machine you wish to install it on.

NOTE! If you are using the RetroPie distribution, you have the option to install Skyscraper directly from the RetroPie-Setup script (*you need to update the script before installing it!*). Read more about all of that [here](https://retropie.org.uk/docs/Scraper/#lars-muldjords-skyscraper). If not, read on.

### Install prerequisites
#### Linux
Skyscraper needs Qt5.3 or later to compile. For a Retropie, Ubuntu or other Debian derived distro, you can install Qt5 using the following commands:
```
$ sudo apt-get update
$ sudo apt-get install build-essential qt5-default
```
You might be asked for your sudo-password. On RetroPie the default password is `raspberry`. To install Qt5 on other Linux distributions, please refer to their documentation.

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
Windows is not officially supported at this time! But I do semi-regularly compile and release an unsupported Windows 64-bit version that works just fine. Be sure to read the included README from the downloaded file before using it! And just to be clear: You are on your own if you use this version - please don't ask me questions about it. Get the Windows version [here](http://www.muldjord.com/downloads/Skyscraper_3.5.8_unsupported_win_version.zip).

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
* Add support for grouping multi-disk games so they only have one entry in the gamelists. See issues/232 (Thank you to 'igno2k' for suggesting this)
* Update 'igdb' module to use 'oauth2' as described here: https://api-docs.igdb.com/#account-creation

#### Version 3.5.9 (In progress, unreleased)
* Improved memory consumption when handing entries back to main thread
* Added '.au3' file extension to id script exception list (Thank you to 'o0alucard0o' for reporting this)

#### Version 3.5.8 (16th August 2020)
* Now supports spaces in Attract-Mode media paths (Thank you to user 'o0alucard0o' for pointing this out)

#### Version 3.5.7 (31st July 2020)
* Now uses regular expressions when converting text to Pegasus format for better compatibility
* Removed 'extensions' when generating Pegasus game list, as it isn't required

#### Version 3.5.6 (30th July 2020)
* Added 'neogeocd' platform (Thank you to 'igno2k' and 'meveric' for suggesting it)
* 'screenscraper' module now supports region for screenshots (Thank you to 'cameronhimself' for pointing this out)
* Low disk space check for cache is now calculated from cache folder location
* Skyscraper initial output now shows frontend and extras when generating game lists
* Changed 'gamelistFolder' config.ini option to 'gameListFolder' but kept old variable name for backwards compatibility
* Added 'gameListBackup' config.ini option to allow for game list backups being created on each run (Thank you to 'igno2k' for suggesting it)
* Attract-Mode now recognizes '$HOME' in emulator file
* Refactored Attract-Mode media folder getters
* Fixed rare faulty matches from 'esgamelist' module in cases where last part of 'path' was a match with filename (Thank you to 'akamming' for reporting this)
* Exporting for Pegasus frontend now supports preservation of old game list key-value pairs (Thank you to user 'HugLifeTiZ' for suggesting this)
* '--cache report' now also adheres so excludeFiles and includeFiles (Thank you to user 'spilinek' for pointing this out)

#### Version 3.5.5 (18th May 2020)
* Fixed bug in 'igdb' module where it still used old hardcoded key (Thank you to 'stlbluesfan' for reporting this)

#### Version 3.5.4 (14th May 2020)
* Hints file converted to xml (hints.txt -> hints.xml)
* Moved documentation resources into 'docs' folder
* Added 'videoPreferNormalized' config.ini option for use with the 'screenscraper' module

#### Version 3.5.3 (11th May 2020)
* MAJOR: Added video conversion support through config.ini option 'videoConvertCommand="COMMAND %i %o"' option. See docs for more details.
* Removed deprecated `<cover>` node and replaced it with `<thumbnail>` for ES gamelist generation (Thank you to 'yisraeldov' for letting me know)
* Added simple error handling for media network requests
* '--flags help' and '--cache help' no longer requires '-p PLATFORM' to be set
* Changed 'worldofspectrum' scraping module to use https

#### Version 3.5.2 (29th April 2020)
* Hid deprecated options from '--help'. Deprecated options still work just aren't shown
* Updated 'Simple Mode' to use the new '--flags' options

#### Version 3.5.1 (28th April 2020)
* Fixed minor bug in 'daphne' when it only has one rom in 'daphne/roms'

#### Version 3.5.0 (27th April 2020)
* For 'screenscraper', 'thegamesdb', 'mobygames' and 'arcadedb' Skyscraper will now no longer fetch the artworks if user has requested it to remain uncached (Thank you to user 'herbymachine' for the request)
* Added '--skipexistingmedia' option to allow for faster gamelist generation in cases where some artwork already exists in the gamelist media folder (Thank you to 'jacrify' for suggesting this)
* Added '--flags' command-line option that collects certain flag-like options into one
* Added deprecated warnings to all options that are now moved to the '--flags' option
* Added '*.cue' extension to '3do' platform (Thank you to 'therealdealneil' for the suggestion)
* Implemented a hack for the 'daphne' platform that will add the game info to the 'game.daphne' folders instead of the 'game.zip' files (Thank you to 'AlCzervik' for pointing this out)

#### Older releases
Release notes for older releases can be found [here](OLDERRELEASES.md).
