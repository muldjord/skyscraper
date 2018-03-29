# Skyscraper by Lars Muldjord
A powerful and versatile yet easy to use game scraper written in C++ for use with multiple frontends running on a Linux system. It scrapes and caches various game resources from various web sources, including media such as screenshot, cover and video. It then gives you the option to combine all of these resources into the most complete results by using the provided 'localdb' scraping module.

Any exported artwork can be customized completely. Check the documentation for that [here](ARTWORK.md).

#### Currently supports the following frontends (set with '-f'):
* EmulationStation
* AttractMode

#### Currently supports the following platforms (set with '-p'):
Check the full list of platforms [here](PLATFORMS.md).

#### Currently supports the following scraping sources (set with '-s')
* WEB: openretro.org
* WEB: thegamesdb.net
* WEB: worldofspectrum.org
* WEB: adb.arcadeitalia.net (Arcade Database by motoschifo, arcadedatabase@gmail.com, https://www.youtube.com/c/ArcadeDatabase)
* WEB: screenscraper.fr
* LOCAL: localdb (scrapes exclusively from cached resources, read more [here](#local-database-features))
* LOCAL: import (imports resources located in '[homedir]/.skyscraper/import' into the local database. Read more under "Local data import")

... More scraping sources will be added in future releases!

#### Support me on Patreon
You can support my work [here](https://www.patreon.com/muldjord). Do keep in mind that pledging is in no way a requirement for using Skyscraper. I mainly do this for my own amusement and self-education.

## How to install Skyscraper
Follow the steps below to install the latest version of Skyscraper. Lines beginning with '$' signifies a command you need run in a terminal on the machine you wish to install it on.

### Install prerequisites
Skyscraper needs Qt5.3 or later to compile. For a Retropie, Ubuntu or other Debian derived distro, you can install it using the following commands:
* $ sudo apt-get update
* $ sudo apt-get install qt5-default

You might be asked for your sudo password. On RetroPie the default password is 'raspberry'. To install Qt5 on other Linux distributions, please refer to their documentation.

### Download, compile and install
When you've installed the prerequisites as described above, you can install Skyscraper by typing in the following commands:
* $ cd
* $ mkdir skysource
* $ cd skysource
* $ curl https://raw.githubusercontent.com/muldjord/skyscraper/master/update_skyscraper.sh | bash

The last command will download and run the latest update script from Github. During the installation you might be asked for your sudo password. On RetroPie the default password is 'raspberry'.

When the script has completed you are ready to run Skyscraper!

### Updating Skyscraper
From Skyscraper 2.3.2 and newer you can update to the latest version simply by running the following commands:
* $ cd
* $ cd skysource
* $ ./update_skyscraper.sh

You might be asked for your sudo password during the update. On RetroPie the default password is 'raspberry'. If your version is older than 2.3.2 (check with '--help') you need to follow the [installation instructions](#download-compile-and-install) instead.

## How to use Skyscraper
IMPORTANT!!! In order for Skyscraper to work properly, it is necessary to quit your frontend before running it! If you're running EmulationStation, you can quit it by pressing F4.

Remember, you can completely customize the artwork Skyscraper exports. Check out the documentation [here](ARTWORK.md). If you just want to use the default (pretty cool looking) artwork Skyscraper provides, read on.

### Simple mode
When you have completed the installation you can start Skyscraper in 'simple mode' by running Skyscraper with no command line options by typing:
* $ Skyscraper

Skyscraper will then ask you a bunch of questions, create an optimized script based on your answers, and finally run the script which scrapes the chosen platform in an optimal way. This is very useful for first time scrapings, as it will give you the best possible initial result for any given platform. If you're curious you can check out the generated script afterwards. It's located in '[homedir]/.skyscraper/skyscript.sh'.

### Manual mode (for advanced users)
Skyscraper is a command line tool, and has many, many options for you to fiddle around with. I recommend taking a look at all of them to familiarize yourself with the possibilites:
* $ Skyscraper --help

This will give you a description of everything Skyscraper can do if you feel adventurous!

NOTE: If you have already scraped a platform using 'Simple mode', any subsequent scrapings of that particular platform should be done with:
* $ Skyscraper -p [platform]

This will scrape the platform using the 'localdb' scraping module, and will make use of all the resources you have gathered in the local database cache. A full description of the 'localdb' scraping module can be found [here](#local-database-features). You can of course add further command line options if needed.

NOTE: To enable video scraping for the scraping modules that support it, you need to add the '--videos' command line option. This is disabled per default because of the significant space requirements needed to save them.

### config.ini
A lesser know, but extremely useful, feature of Skyscraper is to add your desired config variables to '`[homedir]/.skyscraper/config.ini`'. Any options set in this file will be used per default by Skyscraper. So if you always use, for example, '`-m 100`' on command line, you can set the matching option '`minMatch="100"`' in the config.

Many options can be set on two levels; either `[main]` or `[platform]`. Platform can be changed to any of the supported platforms (check list with '--help'), in which case the settings will only be applied while scraping that particular platform. Settings in the `[main]` section will always be used.

You can find an example config file at '`[homedir]/.skyscraper/config.ini.example`'. This file contains all available options. Just uncomment the ones you wish to use by removing the "#" in front of the variables.

### Local database features
Whenever you scrape any platform with any web scraping module, Skyscraper caches each resource locally. A resource can, for instance, be a game 'title' or a game 'screenshot'. Each game can have several versions of each resource cached locally. One of each type per web scraping module. This comes in handy when using the 'localdb' scraping module.

#### LocalDb scraping module
After a while you'll have accumulated a decent amount of locally cached data for any given platform. To exclusively make use of this data Skyscraper provides the 'localdb' scraping module (set with '-s localdb'). By using this source Skyscraper *only* scrapes from the locally cached data. Depending on how many sources you've scraped any given platform with, the 'localdb' module will give you almost perfect results, with almost no data missing. Per default any resource type is prioritized by timestamp. But it is also possible to prioritize them by scraping source. So if you prefer the 'description' results from a certain scraping module, you can easily make sure that these will be prioritized above any other descriptions available. Read more about how to do this [here](dbs/README.md).

#### Update local data
If you wish to update / refresh the locally cached resources for a particular scraping module, Skyscraper provides the '--updatedb' option. If this flag is set on the command line, any data in the local cache will be updated with the new incoming data. So if rom X has a description or any other resource that has been updated online and you wish to make use of it, simply scrape the platform with '-s [scraping module] --updatedb' and the locally cached data will be updated / refreshed.

#### Default db folder
The default folder for all of Skyscrapers' locally cached data is in the '[homefolder]/.skyscraper/dbs' subfolder. In this folder you'll find the individual platform db subfolders. Any platform db folder is selfcontained and can be copied to a USB drive, or zipped up and uploaded to share with friends.

#### User-defined databases
Normally Skyscraper uses a default local db folder for each platform. But a friend might have send you a copy of his local database folder, and you wish to scrape from his data. In this case Skyscraper allows you to force the use of a local database with the '-d [db folder]' command line option. Keep in mind that if your friend has zipped the db folder for convenience, you need to unzip it before use. Skyscraper does *not* currently support zipped db folders.

#### Tiny words of warning
If you start copying your local databases to and from friends, or you accumulate some really big local databases that you sleep with at night because you love them so much - ALWAYS remember to back these up from time to time! Skyscraper is software. Software has bugs. And even though I do quite a bit of testing and feel confident in my code, bugs are inevitable from time to time.

Basically what I'm trying to say is that it is entirely your own fault if you've spent 6 months creating a bunch of local db's and suddenly you overwrite them unintentionally or Skyscraper corrupts the data for some i-have-no-idea-how reason. It could happen. So... PLAN YOUR BACKUPS! And don't come crying to me. :D

### Local data import
I addition to allowing scraping from local resources, Skyscraper also allows you to import your own data into the local cache, which in turn allows you to scrape your roms with it using the '-s localdb' scraping module. For a quick overview read on below. For a more detailed description with examples go [here](import/README.md).

#### Artwork import
Skyscraper allows you to import various artwork resources from the local '[homedir]/.skyscraper/import' folders. Simply place your data inside these folders with the EXACT filename of the roms you wish to connect them to. For instance, if you have a rom called 'Bubble Bobble.nes' you would place your screenshot for this rom inside '[homedir]/.skyscraper/import/screenshots' called 'Bubble Bobble.png'. Other image file formats are also supported.

#### Textual data import
For textual data, you need to first create a file called '[homedir]/.skyscraper/import/definitions.dat'. In this file, you must define the file content format you are providing for each rom. For instance, if your data comes in the form of 1 xml file per rom, and you wish to scrape 'publisher' for this rom, perhaps your input file has a node like '```<publisher>This is the publisher</publisher>```'. In the 'definitions.dat' file you'd then add a line looking like '```<publisher>###PUBLISHER###</publisher```'. The '```###PUBLISHER###```' tag is recognized by Skyscraper.

Now run the scraper with the '-s import' option:
* $ Skyscraper -p [platform] -s import

If you've named the files correctly, the game will show with a green 'YES' for cover, screenshot, wheel, marquee and/or video (if you've enabled them with the '--videos' option). If you've imported textual data, it will show the data at the relevant output line.

Now, to make use of the imported data, scrape with the '-s localdb' scraping module and your resources will be prioritized above all others as defined in '[homedir]/.skyscraper/dbs/[platform]/priorities.xml'.
* $ Skyscraper -p [platform] -s localdb

Then start your frontend and enjoy your newly imported data. :)

### Artwork look and effects
Check the full artwork documentation [here](ARTWORK.md)

## Release notes

#### Version x.x.x (still unimplemented)
* Added 'sharpen' effect which sharpens the image
* Improved 'blur' and 'shadow' effect to be true gaussian
* Implemented the option to only scrape a single resource type during a scraping run
* Added 'mobygames' scraping module
* Implemented 'ratings' such as ESRB ratings from sources ('classification'?, 'ages'?)
* Added <kidgame>bool</kidgame> output when 'ages' permits it
* Added option to --purgedb to purge all resources not related to your current romset
* Added option to --purgedb to purge everything completely

#### Version 2.4.4 (in progress, unreleased)
* For Amiga, searches for "Demo", "aga", "cdtv" and "cd32" in filenames and converts to [AGA] and [CD32] bracket names (Thanks Dom)

#### Version 2.4.3 (25th March 2018)
* Fixed modules that aren't filename based to just always return first entry (Thank you AnalogHero)

#### Version 2.4.2 (25th March 2018)
* Reordered options in '--help' output
* Changed help text for certain options for clarity
* Made sure import scraper uses complete baseName in case filenames have more than one dots (Thank you AnalogHero)

#### Version 2.4.1 (25th March 2018)
* Now auto-adds '--videos' option when using 'import' scraping module (Thank you AnalogHero)
* Fixed bug in import scraper that would hand it the wrong filename to look for

#### Version 2.4.0 (24rd March 2018)
* Fixed bugs in overloaded functions for import and localdb...

#### Version 2.3.9 (23rd March 2018)
* Fixed bug in network communicator that would cause everything to stall

#### Version 2.3.8 (23rd March 2018)
* Implemented user credentials ('-u user:password') to set up threads for 'screenscraper' module
* Made sure artwork output gets exported, even if entry has no base artwork resource
* Changed 'verbose' to 'verbosity' to allow levels and made terminal output more useful overall
* Added '--dbstats' command line option that prints stats for the selected local dabatase cache
* Added '--purgedb' command line option that allows purging resources from localdb
* Fixed bugs in mergedb command line option
* Fixed bug in Simle Mode where 'attractmode' would not work properly (thank you Humayun)

#### Version 2.3.7 (18th March 2018)
* Implemented user credentials ('-u user:password') to set up threads for 'screenscraper' module
* Now checks for .lha suffix and adds spaces where appropriate to get better results
* Improved returned image data validity check (libpng errors still happen, but can be ignored)
* Rewrote the worker to main thread communication a bit
* Implemented '--startat' option that tells Skyscraper the first file to scrape
* Implemented '--endat' option that tells Skyscraper the last file to scrape
* Added thread id to terminal output
* Applied serverside artwork size limit to openretro module to avoid running out of memory
* Improved network communication class

#### Version 2.3.6 (12th March 2018)
* Completely rewrote the openretro parser to make use of the 'edit' page instead
* Added '*.lha' suffix to amiga platform
* Changed default scraping module for all platforms to 'localdb'
* Added 'wheel' support to 'openretro' scraping module
* Rewrote thread queue so entries are always taken alphabetically
* Now forces 4 threads for 'screenscraper' scraping module to accomodate their limits
* Updated screenscraper API to use v2
* Added 'rating' to screenscraper scraping module

#### Version 2.3.5 (24th February 2018)
* IMPORTANT: Fixed bug that caused resources to be mixed up between games because Qt's network cache wasn't cleared (Probably not a Qt bug, but a DAMN hard bug to spot either way). All previous Skyscraper releases have this bug, SO PLEASE UPDATE!!!
* Made 'scummvm' parsing look for config in homedir aswell ('.scummvmrc')
* Now always removes brackets from returned titles
* Now always shows current scraping module in output
* Optimized search matching even more
* No longer asks user about skipping entries if filenames are provided on command line

#### Version 2.3.4 (19th February 2018)
* 'gamebox' effect no longer cuts top and bottom from source layer
* 'gamebox' effect now handles side artwork that is too tall
* Optimized localdb mutexes
* Fixed issue where searchName could be empty and cause Skyscraper to stall for 15 minutes
* Now no longer runs screenscraper sha1 and md5 passes if filesize is 0

#### Version 2.3.3 (17th February 2018)
* Added 'hue' effect which changes the hue of the image
* Added 'saturation' effect which changes the color saturation of the image
* Added 'colorize' effect which renders the image using a single hue
* Added 'rotate' effect that rotates the parent layer around the x, y or z axis
* Added layer render 'mode' attribute ('overlay', 'colordodge', 'hardlight' and so on)
* Added layer 'opacity' attribute (0-100)
* Improved front and side textures for the 'gamebox' effect
* Optimized search passes a bit
* Re-added platform checking on 'screenscraper' and added all arcade platforms as aliases

#### Version 2.3.2 (13th February 2018)
* Added support for 'wii' and 'gc' platforms
* Added '.chd' format to a bunch of platforms
* Added more robust filtering of the faulty screenscraper 'hack-Link' results
* Added 'update_skyscraper.sh' script which enables auto-updating to latest version

#### Version 2.3.1 (13th February 2018)
* Fixed 'players' tag to always conform to a 1-digit format
* Now filters out ".hack-Link" results from 'screenscraper' to avoid bad localdb data
* Added note to output about how many new resources have been added during scraping run
* Added 'color="#fffff"' option to stroke effect for the geeky people (including me of course)
* Conformed 'game tags' to 'Platform, Action' format
* Fixed so 'localdb' folder isn't created inside dbs media folders
* Optimized the mameMap a bit
* Improved the searchMatch system to also consider numerals
* Now looks up 'scummvm' dummy files in 'scummvm.ini' and uses the correct game name

#### Version 2.3.0 (7th February 2018)
The ARTWORK release. Check artwork documentation [here](ARTWORK.md)
* MAJOR: Completely rewrote the artwork compositing engine
    * Now supports nested layers which anchors to the parent layer for easy placement
    * Implemented 'balance' effect that adjusts the colors of the parent layer
    * Implemented 'blur' effect that blurs the parent layer
    * Implemented 'brightness' effect that adjusts the brightness of the parent layer
    * Implemented 'contrast' effect that adjusts the contrast of the parent layer
    * Implemented 'frame' effect that allows you to add a graphical frame to the parent layer
    * Implemented 'gamebox' effect that turns the parent layer into a nice looking 3D game box
    * Implemented 'mask' effect that allows you to mask out certain parts of the parent layer
    * Implemented 'opacity' effect that adjusts the opacity of the parent layer
    * Implemented 'rounded' effect that rounds the corners of the parent layer
    * Implemented 'stroke' effect that outlines the parent layer
    * Improved 'shadow' effect to adhere perfectly to softness as radius
* Added '-a' command line option for setting custom artwork xml config file
* Added 'artworkXml' config file options for setting custom artwork xml config file
* Implemented resource system that allows user to place files in '[homedir]/.skyscraper/resources' and use them in the '[homedir]/.skyscraper/artwork.xml' layers and effects
* Added 'From cache' boolean to output plus note about '--updatedb'
* 'simple mode' now also accepts "Y" as a yes answer instead of just "y"
* Now also looks for 'jp' region if no english region media is found for 'screenscraper' module
* Now always accepts 'screenscraper' results no matter if platform matches or not
* Now sets 'minMatch' to 0 by default for 'localdb' , 'arcadedb' and 'screenscraper' scraping modules. Can be overruled on command line and in config file
* Made localdb more thread safe, might've fixed rare issues of resources being mixed up internally
* Now works with filenames provided on command line even if they don't include full path
* Added resource sources to output
* Added 'wonderswan' and 'wonderswancolor' platforms

#### Version 2.2.7 (31st January 2018)
* Optimized shadow rendering A LOT! WROOOOOOOOOM!!!
* 'simple mode' 'Do you want to run the script' answer now defaults to Y
* Fixed bug in 'simple mode' where answering 'y' to adding brackets would actually remove them

#### Version 2.2.6a (18th January 2018)
* Now always sets '--updatedb' when using the import scraper

#### Version 2.2.6 (17th January 2018)
* Now always caches resources locally, even if pretend is set
* Optimized 'simple mode' generated script. Now has '--pretend' set for all non-local modules to avoid artwork processing on those runs. This is a lot faster and provides the same result
* Added the possibility to supply one or more filenames on the command line - it will then ONLY scrape those particular files. Platform still has to be set with '-p' for this to work
* Fixed bug where [tags] would be appended twice when using '--forcefilename'

#### Version 2.2.5 (3rd January 2018)
* Improved artwork 'shadow' effect. Now renders correctly on artwork with transparency
* Added '(uncached)' to terminal output for media types that user has disabled for caching
* Now always uses existing cached resources when scraping with any scraping module. Use '--updatedb' to force a refresh
* Improved 'artwork.xml' parser to better handle xml syntax problems
* Improved '--cleandb' so it always resaves the db after a cleaning run
* Updated 'simple mode' to also include questions about '--forcefilename' and '--updatedb'
* Now always converts images to 16bit externally. Still uses 32bit internally
* Added 'pc' and 'ports' platform

#### Version 2.2.0 (31st of December 2017)
* Completely rewrote how artwork is handled. Now uses '[homedir]/.skyscraper/artwork.xml' for everything.
* Implemented 'wheel' and 'marquee' media types and rewrote EVERYTHING to make use of them. :D (this was a damn long rewrite!!!!)
* Added 'wheel' and 'marquee' media types to local database
* Added 'nocovers', 'noscreenshots', 'nowheels' and 'nomarquees' options to allow user to disable caching of those resources locally.
* Added 'noresize' option to disable resizing artwork resources when saving them to localdb. NOTE! This it NOT directly related to how artwork is rendered. Check the [artwork](ARTWORK.md) documentation if you want to disable them when exporting the frontend artwork.
* Added support for 'fba' platform

#### Version 2.1.1 (27th December 2017)
* Added support for '3do', 'amstradcpc', 'coco', 'daphne', 'dragon32', 'dreamcast', 'fds' and 'gameandwatch', 'intellivision', 'ngp', 'oric', 'pc88', 'pcfx', 'saturn', 'sg-1000', 'trs-80' and 'zmachine' platforms
* Added 'forcefilename' option which forces the file name to be used in the frontend instead of the one returned by the scraping module
* Changed 'gamelist.xml' to 'game list' in 'simple mode' question

#### Version 2.1.0 (26th December 2017)
* Added 'simple mode' that allows users a more automated way of running Skyscraper. Invoke it by running Skyscraper with no options by simply typing 'Skyscraper'
* A bunch of code refactoring to make platform related code easier to maintain and enable better platform checking

#### Version 2.0.6 (23rd December 2017)
* Added support for atarilynx, vectrex, ngpc (Neo Geo Pocket Color), sega32x, virtualboy and videopac platforms
* Changed all 'qDebug' call to 'printf' calls

#### Version 2.0.5c (23rd December 2017)
* Fixed a bug where the import module wouldn't look for all resource types during the same run. Thank you to dylosaur for discovering this.

#### Version 2.0.5b (5th December 2017)
* Added support for *.7z files for all platforms.

#### Version 2.0.5a (15th November 2017)
* Fixed Amiga 'adf' files not being scraped because I messed up and typed it as 'afd'. Thank you to James Kerridge for letting me know about this.

#### Version 2.0.5 (14th November 2017)
* Added support for 'scummvm' platform in scraping module 'thegamesdb' or 'openretro'.
* Now only removes 'the' from searchName if longer than 10 chars.
* Now always converts underscores to spaces in search- and compare names.
* Added edit distance optimization ('the sequel' will now match 'some game: the sequel' 100%).

#### Version 2.0.4 (5th November 2017)
* Default language is now 'en' instead of 'us'.
* Empty video tags no longer added to gamelist.xml for emulationstation.

#### Version 2.0.3 (5th October 2017)
* Added '--lang' and '--region' command line options. Defaults to 'us' and 'wor' if none is provided.
* Language and region support now implemented for 'screenscraper' module.
* Removed 'stats' option as it didn't make much sense anymore.

#### Version 2.0.2 (4th October 2017)
* Updated 'arcadedb' result parsing to fit new format
* Now scrapes 'msx' platform families correctly with the 'screenscraper' module
* Optimized file checksumming
* Fixed a bug in 'screenscraper' module not allowing platforms with multiple platform name to be recognized. (thanks to 'qqplayer' for pointing this out!)

#### Version 2.0.1 (15th September 2017)
* Removed 'mamedb' source files as they were no longer in use
* Slightly changed help text for scraping modules
* 'thegamesdb' now properly uses Qt's XML parser
* 'screenscraper' now properly uses Qt's XML parser
* Started implementing region and lang support for 'screenscraper', but still not enabled

#### Version 2.0.0 (7th September 2017)
* Back to basics: Removed several web sources. Now only allows the ones I have explicit permission to use.
* Properly implemented official API for 'arcadedb' module
* Added scraping module info to output per result but only when using '--verbose'
* Added check for unreasonably bad scraping runs, making Skyscraper exit if 30 of 30 files miss from the get-go

### Older unsupported releases (no longer available and don't ask for them)

#### Version 1.8.2 (27th August 2017)
* Added support for 'coleco' platform
* Added support for 'pcengine' platform
* Added zip support to all platforms per request from users. :)

#### Version 1.8.1 (22nd August 2017)
* Added 'rating' scraping to 'thegamesdb'

#### Version 1.8.0 (14th August 2017)
* Added 'arcadedb' scraper module *with* video support
* Vastly improved scraping of 'neogeo' and 'arcade' platforms in general by mapping the filenames to real names from mameMap.csv
* Improved 'neogeo' and 'arcade' search platform matching

#### Version 1.7.4 (10th August 2017)
* Added textual import with 'import' scraper using '[homedir]/.skyscraper/important/definitions.dat' file
* Added video import with 'import' scraper
* Improved 'uvlist' description scraping
* Now properly handles empty nodes in EmulationStation gamelist.xml export

#### Version 1.7.3 (8th August 2017)
* Added 'developer' support for 'uvlist' scraper
* Improved html unescaping a lot
* Cleaned up xml escaping
* Added 'import' scraper, scraping from resources located in '[homedir]/.skyscraper/import' folder

#### Version 1.7.2 (7th August 2017)
* Added 'uvlist' scraping module
* Added rating resource and support
* Added rating support to lemonamiga
* Added rating support to lemon64
* Added rating support to mobygames

#### Version 1.7.1c (7th August 2017)
* Fixed make install

#### Version 1.7.1 (7th August 2017)
* Moved all source files to 'src' folder
* '[homedir]/.skyscraper' is now default folder for all files used by Skyscraper
* '/usr/local/bin/Skyscraper' is now default location for Skyscraper executable
* Refined '--help' output a bit
* Fixed lemon64 scraping
* Added 'lemonamiga' scraping module
* Added '--skipped' command line option
* Added 'make install' for correct installation of files

#### Version 1.7.0 (5th August 2017)
* MAJOR: Fixed and refined 'attractmode' frontend implementation, now works in a basic manner
* 'attractmode' can now skip existing entries
* 'emulationstation' now properly add brackets to 'name' on skipped entries
* Added check for 'db.xml' when doing '--cleandb'
* Refactored GameEntry variables
* Changed GameEntry from struct to class
* Added 'Overall title similarity' to final output
* Added 'Overall completeness' to final output
* Code refactoring here, there and everywhere
* Now accepts results where we have low editDistance, but high similarity (For instance "Disney's Darkwing Duck" with fileName "Darkwing Duck").
* Added '--nobrackets' option that disables and [] and () tags in the frontend game titles. (Thanks for the feedback 'incunabula')
* Fixed bracket parsing
* Now always uses completeBaseName since some filenames have more than one '.'
* Completely rewrote sorting algorithm. 30 lines became one with a nifty C++11 lambda :D
* Added zip format to GameGear and MSX platforms
* Now uses filenames for output image files again

#### Version 1.6.0 (1st August 2017)
* Now allows more resources of same type, as long as 'source' differs
* Now allows user to set priorities for local resource sources
* Fixed a bug that would nullify timestamp of local resources
* Optimized LocalDb communication to improve scraping speed
* Added README.md to dbs subfolder
* Added priorities.xml.example file to dbs subfolder. Automatically copies this to new databases when they are created if none already exists.
* Implemented '--cleandb' command line option that removes files with no resource entry
* Implemented '--mergedb' command line option that merges two local databases together
* Now no longer does sha1 for roms bigger than 50 MBs (Pi runs out of ram when reading them). Instead does sha1 on filename for those special cases.
* Removed default platform when scraping. You are now forced to put in a valid platform with '-p [platform]'
* Added more initial info when running Skyscraper
* Added '--unattend' command line option
* Added 'source' attribute to local database resources
* Removed 'mobygames' descriptions from 'openretro' scraper. Now uses native descriptions.
* Improved cover and screenshot scraping for 'openretro' module
* Disabled filling in missing data when scraping from web sources. User is meant to use 'localdb' scraping module for this.
* Implemented date formats to standardize output and better support EmulationStation requirements

#### Version 1.5.0 (27th July 2017)
* MAJOR: Added support for local database resources
* MAJOR: Added support for video scraping (currently supported in the 'screenscraper' scraping module)
* MAJOR: Added 'localdb' scraping module
* Added video tag in EmulationStation gamelist.xml output. Beware though, the Pi's are having a difficult time showing the videos properly.
* Added several new command line options relevant to the new video and localdb features
* Added cover, screenshot and video as part of the result output with "YES" or "NO" depending on whether they were found or not
* Fixed a bug where image tag in gamelist.xml had wrong path when using non-default path
* Now uses rom or filename (for .uae) sha1 for image filename, in case people have several roms with the same name under subdirs
* Added 'players' scraping for 'mobygames' module and improved screenshot getter even more

#### Version 1.1.8b (24th July 2017)
* Implemented a slightly hacky fix that removes some (but not all) warnings caused by a known Qt bug
* Improved final sorting algorithm
* Code refactoring / polishing

#### Version 1.1.8a (23rd July 2017)
* Hardcoded screenscraper devid and password, since this seems to be the right way of doing it

#### Version 1.1.8 (23rd July 2017)
* Refactored internal config handling to be MUCH cleaner
* Fixed bug in xml escaping
* Fixed the final sorting algorithm so it no longer outputs double entries

#### Version 1.1.7 (21st July 2017)
* Added '--nosubdirs' command line argument to exclude subdirs when scraping
* Slightly changed screen output colors and text
* Implemented generic AbstractFrontend class
* Added support for multiple frontends set with '-f'
* Changed default scraper for certain platforms based on new stats

#### Version 1.1.6 (20th July 2017)
* Corrected max threads from 4 to 8 in command line help text
* Fixed default config value bug

#### Version 1.1.5 (20th July 2017)
* Added 'msx' platform support
* Added 'psp' platform support
* Added possibility for one input platform to match many different scraper source platforms

#### Version 1.1.4 (20th July 2017)
* Added 'atarist' support
* Improved mobygames getScreenshot function

#### Version 1.1.3 (19th July 2017)
* Improved sorting by always moving "The" to end of game title

#### Version 1.1.2 (19th July 2017)
* Upped allowed number of threads to 8 (Wrooooooom!!!)
* Fixed a nasty race condition related to config file access that caused regular crashes
* Refactored config default loading to be easier to understand
* Fixed a silly memory hog that caused memory to be eaten up (not a leak, just really silly)

#### Version 1.1.1 (18th July 2017)
* Now allows user to skip existing entries
* Now allows user to set max description length with '-l'
* Refactored thread result communication to be much cleaner

#### Version 1.1.0a (16th July 2017)
* Fixed a bug where Skyscraper would get caught in an endless loop when scraping 'amiga' using the 'openretro' scraping module.

#### Version 1.1.0 (14th July 2017)
* Now quits if GamesDatabase has blocked you.
* Added support for user ID and KEY when using scrapers that require it
* Added 'screenscraper' scraping module
* Mobygames scraping module no longer truncates descriptions
* OpenRetro scraping module no longer truncates descriptions

#### Version 1.0.1 (4th July 2017)
* OpenRetro now always removes '[AGA]' from search results, since it will be appended later
* Now supports multitags of both the '()' and '[]' variety
* Now properly removes html tags from game descriptions in various scraper modules
* Fixed situations where using OpenRetro would result in a few blank covers

#### Version 1.0.0 (3rd July 2017)
* Added 'worldofspectrum' scraping module (Have fun, Dom! :D)
* Now also handles filename parenthesis comments (eg. '(disc 1 of 2)')
* Now properly handles number of threads if number of files are less than allowed threads
* Changed default scraping module for a number of platforms based on stats

#### Version 0.9.9 (2nd July 2017)
* Added 'gamesdatabase' scraping module
* Now detects if a game is a sequel and pays more attention to it when looking for matches
* Now properly appends tag such as [AGA] back into the title name when writing the xml
* Added '--stats' command line option for exporting platform scraping stats
* Changed default scraper for a bunch of platforms based on stats
* Added support for the 'apple2' platform
* Added support for the 'atari5200' platform
* Added support for the 'atarijaguar' platform
* Added support for the 'gb' platform
* Added support for the 'gbc' platform
* Added support for the 'n64' platform
* Added support for the 'nds' platform
* Added support for the 'segacd' platform

#### Version 0.9.8 (29th June 2017)
* Added MameDB scraper module
* Added support for the 'neogeo' platform
* Added support for the 'arcade' platform
* Added support for the 'atari2600' platform
* Added support for the 'atari7800' platform
* Added support for the 'gamegear' platform
* Added support for the 'mastersystem' platform
* Added 'Estimated time' to output
* Redesigned thread initialization
* FINALLY found and fixed memory leak :) Verified with 'ps'

#### Version 0.9.7a (27th June 2017)
* Probably fixed a memory leak... I hope. :S

#### Version 0.9.7 (26th June 2017):
* Artwork is now fully customizable (check 'config.ini.example')
* Added artwork dropshadow effect
* Added artwork config examples
* Added support for megadrive / genesis

#### Version 0.9.6 (25th June 2017):
* HOL scraping module added

#### Version 0.9.5 (23rd June 2017):
* Now properly supports config file setup using '-c' (see 'config.ini.example')
* Now allows user to force a certain scraper module using '-s'
* MobyGames scraping support added
* Added support for psx, nes, snes and zxspectrum
* Now provide 'config.ini.example'

#### Version 0.9.4 (21st June 2017):
* Lemon64 scraping support added (now default for 'c64')

#### Version 0.9.3 (19th June 2017):
* Fixed major bug in Amiga scraping which caused it to skip more than half :S

#### Version 0.9.2 (19th June 2017):
* Fixed minor error in command line descriptions
* Now prints chosen platform when starting

#### Version 0.9.1 (19th June 2017):
* Removed 'unpublished' command line option, as it was too specific

#### Version 0.9.0 (19th June 2017):
* Now supports both Amiga and C64 scraping. Automatically chooses best scraper for each platform
* Modularized scraping definitions
* TheGamesDb scraping support added (default for 'c64')
* OpenRetro + MobyGames (for descriptions) scraping support added (default for Amiga)

#### Version 0.8.0 (15th June 2017):
* Added threaded scraping! Set number of threads with "-t" command line option. Check "--help" for more info

#### Version 0.7.2 (12th June 2017):
* Added options for gamelist and images folder

#### Version 0.7.0 (11th June 2017):
* First public release
