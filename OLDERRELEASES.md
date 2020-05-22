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
