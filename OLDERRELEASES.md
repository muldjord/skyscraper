#### Version 2.8.3 (8th December 2018)
* Added "region" and "lang" options to "[platform]" section of config.ini
* Added "noResize" variable to "[localDb]" config.ini section
* Fixed bug where "--query" would stop working when region was autodetected

#### Version 2.8.2 (7th December 2018)
* Now autodetects region from filename and prioritizes it when scraping with "screenscraper" unless user has forced region with "--region" option (thank you to "movisman" for suggesting this)
* Fixed issue where "wheel" artwork would not be fetched for specified region even though it exists
* Added ".cue" file extension to Amiga

#### Version 2.8.1 (25th November 2018)
* Now loads tgdb json files from "~/.skyscraper"

#### Version 2.8.0 (4th November 2018)
* Now tries to preserve EmulationStaton gamelist.xml metadata unless new incoming data exists
* Now tries to preserve AttractMode romlist metadata unless new incoming data exists
* Improved terminal output a bit with fflush and progress dots
* Now looks for 'definitions.dat' in base import folder before trying import/[platform] folder

#### Version 2.7.8 (21st October 2018)
* Added CRC32 checksum attribute to 'screenscraper' pass
* Changed the 'import' folder to also look into 'import/[platform]' if it exists so users can create imports per platform. Old method also works (Thanks to tacodog for suggesting this)
* Added 'mpixels' attribute to compositor layers that allows users to scale images to a desired area size instead of using width and height (Thank you to Vynce for suggesting this)

#### Version 2.7.7 (10th October 2018)
* Fixed instances where '--query' would be applied to all roms if a faulty filename was given on command line (Thank you to Razile89 for reporting this)

#### Version 2.7.6 (7th October 2018)
* Added 'Found', 'Not found' and 'Total' to output
* Re-added 'aga' Amiga detection for OpenRetro after refactoring
* Added 'resource' attribute to 'output' nodes which enables user to directly export wheel as marquee by adding a <output type="marquee" resource="wheel"/> node to artwork.xml

#### Version 2.7.5 (21st September 2018)
* Fixed a bug where 'brackets="false"' in config.ini would be flipped (Thanks to Vynce for reporting this)
* Completely refactored pass procedures for cleaner code and to enable '--query' option
* Added '--query' command line option. This option requires a single rom file to be passed on command line aswell, otherwise it will be ignored (Thank you to AnalogHero and Vynce for suggesting this)
* Added scrapers to 'psx' and 'pc' platforms when using Simple Mode

#### Version 2.7.4 (11th September 2018)
* Added '--unpack' command line option and config.ini options which enables the calculation of the checksum of the file inside 7z and zip files instead of the base file itself. Only relevant for 'screenscraper' module and requires 'p7zip' installed. (```$ sudo apt-get install p7zip-full``` for Debian base systems)
* Added 'inputFolder' to [main] section of config.ini
* Added 'gamelistFolder' to [main] section of config.ini
* Added 'mediaFolder' to [main] section of config.ini

#### Version 2.7.3 (4th September 2018)
* Improved image cropping to now also crop black borders, but only for screenshots (Thank you to 'chipsnblip' for suggesting this)
* Made 'import' base folder configurable in config.ini
* Fixed bug in 'import' scraping module that caused dummy titles to be saved to localdb when scraping media resources
* Changed 'curl' to 'wget' in update_skyscraper.sh script to avoid curl requirement

#### Version 2.7.2 (2nd September 2018)
* Fixed bug that caused the <marquee> nodes to disappear from gamelist.xml when skipping entries (Thanks to ozy24 for reporting this)
* Added ###TITLE### tag to import scraping module (Thanks to ozy24 for suggesting this)
* Updated import definition examples and documentation to include ###TITLE### tag and ###AGES### tag
* Updated priorities.xml.example to also include 'import' under 'title'
* Added 'Did you know' hints when running Skyscraper
* Added '--nohints' to disable hints. Can also be set in config.ini

#### Version 2.7.1 (28th August 2018)
* Made sure 'Skyscraper's choice' only appears once per entry when using interactive mode
* Added 'sidescaling' attribute to 'gamebox' compositor effect node (Thanks to darkvader for suggesting this)

#### Version 2.7.0 (27th August 2018)
* Added scraping module specific configs in config.ini to allow different userCreds per scraping module
* Added 'igdb' scraping module
* Added '--interactive' mode which will ask user to choose the best result
* Changed release date patterns to regexps for cleaner code
* Fixed bug which caused <video> tags to disappear when skipping entries with emulationstation (thank you to ozy24 for reporting this)
* Reached commit number 1000!!! I'm gonna celebrate by coding some more
* Conformed [homedir] to be '~' everywhere
* Added several aliases to platforms to better support all scraping modules
* Added option for custom header when initiating network requests
* When generating search names, all instances of ' v.X' and ' rev.X' are now removed prior to search
* Added '--unattendskip' option to allow for always skipping existing entries without user input (thank you to ozy24 for suggesting this)

#### Version 2.6.0 (25th August 2018)
* Added 'mobygames' scraping module

#### Version 2.5.8 (24th August 2018)
* Fixed bug in 'genesis' platform when using the 'screenscraper' module
* Added 'megacd' platform support

#### Version 2.5.7 (21st August 2018)
* Added 'Super Famicom' as alias to 'Super Nintendo'
* Added restrictions for 'openretro' scraping module and removed it from Simple Mode

#### Version 2.5.6 (20th August 2018)
* Added 'atari800' platform support

#### Version 2.5.5 (18th August 2018)
* Changed '--updatedb' to '--refresh' as the name confused users. '--updatedb' still works, but is considered deprecated
* Added 'ss' and 'uk' regions to 'screenscraper' module which should fix A LOT of games that weren't found when using this module
* Added unofficial OSX support to the install script

#### Version 2.5.4 (10th August 2018)
* Added 'extensions=' to platform sections of config.ini to allow overwriting the default extensions for any platform
* Added '--maxfails [int]' command line option and 'maxFails=[int]' config.ini option to allow user to set a higher allowed initial 'Not found' before quitting

#### Version 2.5.3 (24th July 2018)
* Added '--allowext' option which will force allow a file extension for the given platform. Thank you to 'herbymachine' for suggesting this
* Added 'allowExtension' to the '~/.skyscraper/config.ini' variables for both 'main' and 'platform' specific sections. This is useful if you wish to permanently add a file extension to all or one platform when scraping
* Implemented 'developers' change in 'thegamesdb' API
* Implemented 'publishers' change in 'thegamesdb' API
* Fixed 'Tags' bug in 'screenscraper' module

#### Version 2.5.2 (13th July 2018)
* Fixed bug in 'thegamesdb' module that broke platform and genre detection

#### Version 2.5.1 (10th July 2018)
* Added '--relative' option which forces rom relative paths in gamelist for EmulationStation

#### Version 2.5.0 (8th July 2018)
* 'thegamesdb' removed from Simple Mode scraping scripts due to new api restrictions
* Implemented new 'thegamesdb' api, be wary of monthly request limit!
* Made sure 'remaining requests' is clearly stated when using 'thegamesdb'
* Implemented request limit test which makes Skyscraper stop if limit is reached
* Made sure cli header always has correct number of dashes

#### Version 2.4.11 (17th June 2018)
* Added 'mega-cd' as alias for 'segacd' platform. Thank you to user trevor7oaks for reporting this

#### Version 2.4.10 (14th June 2018)
* '/home/pi' is no longer hardcoded anywhere in Skyscraper. Now always uses current user home dir
* Changed 'Estimated time total' to 'Estimated time left' in terminal output

#### Version 2.4.9 (9th June 2018)
* Added 'mame-advmame', 'mame-mame4all' and 'mame-libretro' platforms

#### Version 2.4.8 (31st May 2018)
* Fixed bug that made region default to 'wor' instead of 'eu' and 'us' if unset

#### Version 2.4.7 (30th May 2018)
* Added c16, c128 and vic20 platforms. c16 and c128 will scrape using c64 alias as they aren't directly supported by the sources

#### Version 2.4.6 (25th May 2018)
* Added 'overview' support for AttractMode. It will now create the necessary cfg files to show the game descriptions
* Added '`<kidgame>bool</kidgame>`' output to Emulationstation gamelist generation
* Added 'ages' support in 'screenscraper' module. Will convert PEGI and ESRB to numeric
* Added 'ages' support in 'thegamesdb' module. Will convert PEGI and ESRB to numeric

#### Version 2.4.5 (8th May 2018)
* Added support for the 'x68000' platform

#### Version 2.4.4 (2nd April 2018)
* Changed openretro 'wheel' type to 'marquee' type. It wasn't really a wheel
* 'openretro' now filters out variants to avoid bad screenshots (Thank you AnalogHero)
* Now allows you to set custom dbFolder in config.ini both under main and platform sections
* For Amiga it now searches for "Demo", "aga", "cdtv" and "cd32" in filenames and converts to [] bracket names (Thanks Dom)
* Region 'wor' has been moved below 'eu' and 'us' for Screenscraper since it often returns Japanese titles (Thanks maroonout09)
* Now turns '_-_' into ' ' in filenames before doing searches (Thanks maroonout09)
* Simplified thread result communication with main thread from two functions to just one
* Now again properly tests for a scraping run with 30 consecutive misses, then exits

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
The ARTWORK release. Check artwork documentation [here](docs/ARTWORK.md)
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
* Implemented resource system that allows user to place files in '~/.skyscraper/resources' and use them in the '~/.skyscraper/artwork.xml' layers and effects
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
* Completely rewrote how artwork is handled. Now uses '~/.skyscraper/artwork.xml' for everything.
* Implemented 'wheel' and 'marquee' media types and rewrote EVERYTHING to make use of them. :D (this was a damn long rewrite!!!!)
* Added 'wheel' and 'marquee' media types to local database
* Added 'nocovers', 'noscreenshots', 'nowheels' and 'nomarquees' options to allow user to disable caching of those resources locally.
* Added 'noresize' option to disable resizing artwork resources when saving them to localdb. NOTE! This it NOT directly related to how artwork is rendered. Check the [artwork](docs/ARTWORK.md) documentation if you want to disable them when exporting the frontend artwork.
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
* Added textual import with 'import' scraper using '~/.skyscraper/important/definitions.dat' file
* Added video import with 'import' scraper
* Improved 'uvlist' description scraping
* Now properly handles empty nodes in EmulationStation gamelist.xml export

#### Version 1.7.3 (8th August 2017)
* Added 'developer' support for 'uvlist' scraper
* Improved html unescaping a lot
* Cleaned up xml escaping
* Added 'import' scraper, scraping from resources located in '~/.skyscraper/import' folder

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
* '~/.skyscraper' is now default folder for all files used by Skyscraper
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
