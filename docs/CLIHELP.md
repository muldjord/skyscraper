## Command line options
Look below to find a thorough description of all command line options available when using Skyscraper. These options should be used with the `Skyscraper` command.

Also consider that almost all of these options are set at a useful default (and can therefore be left out) and should only be used / changed if your use case requires it.

NOTE! If you've installed Skyscraper through the RetroPie-Setup script, it is recommended to create a symbolic link to the executable. Do this by running `sudo ln -s /opt/retropie/supplementary/skyscraper/Skyscraper /usr/local/bin/Skyscraper`. This will allow you to just type `Skyscraper` when running it from command line.

IMPORTANT! Most of the options can also be set in the `/home/USER/.skyscraper/config.ini` file thus removing the need to type them on command line all the time. Check the config.ini doc [here](CONFIGINI.md) for more info on this.

### -h, --help
Outputs the help text for all command line options to the terminal.

###### Example(s)
```
Skyscraper --help
Skyscraper -h
```

### -p &lt;PLATFORM&gt;
Sets the platform you wish to scrape. Supported platforms can be seen using the `--help` option described above.

Running the following commands will scrape from all cached resources and generate a game list and composite artwork using the recipe in `/home/USER/.skyscraper/artwork.xml` (check the artwork documentation for more info on this [here](ARTWORK.md).

Before running these commands you need to first gather some data into the cache. Please read the description of `-s <MODULE>` below.

###### Example(s)
```
Skyscraper -p amiga
Skyscraper -p snes

```

### -s &lt;MODULE&gt;
Sets which scraping module you wish to gather data from. All data scraped from any of the modules will be cached in the resource cache and can then later be used to generate a game list for your frontend. Read more about this in the `-p <PLATFORM>` description above.

To generate a game list from the resource cache, just leave out the `-s` option entirely.

###### Example(s)
```
Skyscraper -p amiga -s openretro
Skyscraper -p snes -s screenscraper
Skyscraper -p amiga -s esgamelist
Skyscraper -p snes -s import
```
To read more about each scraping module, check [here](SCRAPINGMODULES.md).

### -u &lt;KEY or USERID:PASSWORD&gt;
Some scraping modules require a user key or a user id + password to work. Check the scraping module overview to see the specific requirements for each module [here](SCRAPINGMODULES.md). Consider setting this in [`config.ini`](CONFIGINI.md#usercredsuserpassword) instead.

###### Example(s)
```
Skyscraper -p snes -s screenscraper -u <userid:password>
```

### -i &lt;PATH&gt;
Sets the rom input folder. By default Skyscraper will look for roms in the `/home/<user>/RetroPie/roms/<PLATFORM>` folder. If your roms are located in a non-default location, you can set the input path using this option. Consider setting this in [`config.ini`](CONFIGINI.md#inputfolderhomepiretropieroms) instead.

###### Example(s)
```
Skyscraper -p snes -i "/path/to/your/snes/roms"
```

### --nosubdirs
This option is deprecated! Please set it using the [`--flags`](#--flags-flag1flag2) option instead.

### --unpack
This option is deprecated! Please set it using the [`--flags`](#--flags-flag1flag2) option instead.

### -g &lt;PATH&gt;
Sets the game list export folder. By default Skyscraper exports the game list to the same directory as the rom input folder. This enables you to change that to a non-default location. Consider setting this in [`config.ini`](CONFIGINI.md#gamelistfolderhomepiretropieroms) instead.

###### Example(s)
```
Skyscraper -p snes -s screenscraper -g "/your/desired/game list/export/path"
```

### -o &lt;PATH&gt;
Sets the artwork / media output folder. By default Skyscraper outputs the composited artwork files to the game list export folder + `/media`. This allows you to change that to a non-default location. Read more about the artwork compositing [here](ARTWORK.md). Consider setting this in [`config.ini`](CONFIGINI.md#mediafolderhomepiretropieroms) instead.

###### Example(s)
```
Skyscraper -p snes -s screenscraper -o "/path/to/where/you/want/the/artwork/files"
```

### -t &lt;1-8&gt;
Sets the desired number of parallel threads to be run when scraping. NOTE! Some modules have maximum allowed threads. If you set this higher than the allowed value, it will be auto-adjusted. By default it is set to 4.

###### Example(s)
```
Skyscraper -p snes -s thegamesdb -t 5
```

### -f &lt;FRONTEND&gt;
Sets the frontend you wish to export a game list for. By default Skyscraper will export an EmulationStation game list, but other frontends are supported as well. If exporting for the `attractmode` frontend, please also take note of the required `-e` option that goes along with using the `attractmode` frontend. Consider setting this in [`config.ini`](CONFIGINI.md#frontendemulationstation) instead.

Check all supported frontends with '--help' and read a more about the details concerning each of them [here](FRONTENDS.md).

###### Example(s)
```
Skyscraper -p snes -f pegasus
Skyscraper -p snes -f attractmode -e snes
```

### -e &lt;STRING&gt;
NOTE! This option is applicable *only* when using the `-f attractmode` or the `-f pegasus` option.

When using `-f attractmode` it is required to set the *emulator* to be used when generating the `attractmode` game list. On RetroPie the emulator name is mostly the same as the platform. Consider setting this in [`config.ini`](CONFIGINI.md#emulator) instead.

It can *optionally* be used with `-f pegasus` to set the launch command used by the Pegasus game list. On RetroPie this defaults to the RetroPie launch command which works with RetroPie. Consider setting this in [`config.ini`](CONFIGINI.md#launch) instead.

###### Example(s)
```
Skyscraper -p snes -f attractmode -e snes
Skyscraper -p snes -f pegasus -e "/opt/retropie/supplementary/runcommand/runcommand.sh 0 _SYS_ snes"
```

### -m &lt;0-100&gt;
Some scraping modules are based on a filename or title based search. This option sets the minimum percentage any returned results need to match with in order for it to be accepted. For instance, the game `Wonderboy in Monsterland` might return the title `Wonder Boy in Monster Land` which is clearly a match. But it's not a 100% match. So it needs to be set relatively high, while still ignoring bad matches. By default it is set to 65 which has been tested to be a good middle-ground. Consider setting this in [`config.ini`](CONFIGINI.md#minmatch65) instead.

###### Example(s)
```
Skyscraper -p snes -s thegamesdb -m 50
```

### -l &lt;0-10000&gt;
Sets the maximum length of returned game descriptions. This is a convenience option if you feel like game descriptions are too long. By default it is set to 2500. Consider setting this in [`config.ini`](CONFIGINI.md#maxlength10000) instead.

###### Example(s)
```
Skyscraper -p snes -l 500
```

### -c &lt;FILENAME&gt;
Sets a non-default config file. By default Skyscraper uses the file `/home/USER/.skyscraper/config.ini`.

###### Example(s)
```
Skyscraper -p snes -c "/path/to/config.ini"
```

### -a &lt;FILENAME&gt;
Sets a non-default xml file to use when setting up the artwork compositing. By default Skyscraper uses the file `/home/USER/.skyscraper/artwork.xml`. Read more about the artwork.xml format and customization options [here](ARTWORK.md). Consider setting this in [`config.ini`](CONFIGINI.md#artworkxmlartworkxml) instead.

###### Example(s)
```
Skyscraper -p snes -a "/path/to/artwork.xml"
```

### -d &lt;FOLDER&gt;
Sets a non-default location for the storing and loading of cached game resources. This is what is referred to in the docs as the *resource cache*. By default this folder is set to `/home/USER/.skyscraper/cache/<PLATFORM>`. Don't change this unless you have a good reason to (for instance if you want your cache to reside on a USB drive). The folder pointed to should be a folder with a Skyscraper `db.xml` file and its required subfolders inside of it (`covers`, `screenshots` etc.).

NOTE! If you wish to always use a certain location as base folder for your resource cache (for instance a folder on a USB drive), it is *strongly* recommended to set this in the config.ini file instead. Read more about the relevant config.ini option [here](CONFIGINI.md#cachefolderhomepiskyscrapercache).

###### Example(s)
```
Skyscraper -p snes -d "/custom/cache/path"
```

### --refresh
Skyscraper has a resource cache which works just like the browser cache in Firefox. If you scrape and gather resources for a platform with the same scraping module twice, it will grab the data from the cache instead of hammering the online servers again. This has the advantage in the case where you scrape a rom set twice, only the roms that weren't recognized the first time around will be fetched from the online servers. Everything else will be loaded from the cache.

You can force all data to be refetched from the servers by setting this option, effectively updating the cached data with new data from the source.

NOTE! *Only* use this option if you know data has changed for several roms at the source. Otherwise you are hammering the servers for no reason.

###### Example(s)
```
Skyscraper -p snes -s screenscraper --refresh
```

### --flags <FLAG1,FLAG2,...>
From Skyscraper 3.5.0 all command-line options that change the scraping behaviour have been combined into this option. Check below for a complete list of all the available flags and what they do. You can also get this list by using `--flags help`.

To enable multiple flags simply separate them by commas (eg. `--flags FLAG1,FLAG2`).

NOTE! The old options will continue to function for the time being, but if you have scripts using them, please migrate to this new `--flags FLAG1,FLAG2` format as soon as possible. The old options *will* be removed at some point in the future. To see which flags are considered deprecated check `--help`.

#### forcefilename
This flag forces Skyscraper to use the filename (excluding extension) instead of the cached titles when generating a game list. Consider setting this in [`config.ini`](CONFIGINI.md#forcefilenamefalse) instead.

NOTE! If 'nameTemplate' is set in config.ini this flag is ignored.
#### interactive
When gathering data from any of the scraping modules many potential entries will be returned. Normally Skyscraper chooses the best entry for you. But should you wish to choose the best entry yourself, you can enable this flag. Skyscraper will then list the returned entries and let you choose which one is the best one.
#### nobrackets
Use this flag to disable any bracket notes when generating the game list. It will disable notes such as `(Europe)` and `[AGA]` completely. This flag is only relevant when generating the game list. It makes no difference when gathering data into the resource cache. Consider setting this in [`config.ini`](CONFIGINI.md#bracketstrue) instead.

NOTE! If 'nameTemplate' is set in config.ini this flag is ignored.
#### nocovers
Disables the caching of the resource type `cover` when scraping with any module. If you never use covers in your artwork configuration, this flag can save you some space. Consider setting this in [`config.ini`](CONFIGINI.md#cachecoverstrue) instead.
#### nocropblack
Disables cropping away the black borders around screenshot resources when compositing the final frontend artwork.
#### nohints
Disables the "Did you know" hints when running Skyscraper. Consider setting this in [`config.ini`](CONFIGINI.md#hintsfalse) instead.
#### nomarquees
Disables the caching of the resource type `marquee` when scraping with any module. If you never use marquees in your artwork configuration, this flag can save you some space. Consider setting this in [`config.ini`](CONFIGINI.md#cachemarqueestrue) instead.
#### noresize
By default, to save space, Skyscraper resizes large pieces of artwork before adding them to the resource cache. Adding this flag will disable this and save the artwork files exactly as they are retrieved from the scraping module. Consider setting this in [`config.ini`](CONFIGINI.md#cacheresizefalse) instead.

NOTE! This is not related to the artwork compositing that happens when generating a game list. This is *only* related to how Skyscraper handles artwork when adding it to the resource cache while gathering data from the scraping modules.
#### noscreenshots
Disables the caching of the resource type `screenshot` when scraping with any module. If you never use screenshots in your artwork configuration, this flag can save you some space. Consider setting this in [`config.ini`](CONFIGINI.md#cachescreenshotstrue) instead.
#### nosubdirs
By default Skyscraper will include roms located in subfolders. By adding this flag Skyscraper will only scrape the roms located directly in the input folder. See `-i <PATH>` above to read more about the rom input folder. Consider setting this in [`config.ini`](CONFIGINI.md#subdirstrue) instead.
#### nowheels
Disables the caching of the resource type `wheel` when scraping with any module. If you never use wheels in your artwork configuration, this flag can save you some space. Consider setting this in [`config.ini`](CONFIGINI.md#cachewheelstrue) instead.
#### onlymissing
This flag tells Skyscraper to skip all files which already have any piece of data from any source in the cache. This is useful if you just scraped almost all files from a platform succesfully with one source, and then want to only scrape the remaining games with a different source to fill in the holes. Normally Skyscraper will scrape all files again with the second source.
#### pretend
This flag is *only* relevant when generating a game list (by leaving out the `-s <MODULE>` option). It disables the game list generator and artwork compositor and only outputs the results of the potential game list generation to the terminal. It can be very useful to check exactly what and how the data will be combined from the resource cache.
#### relative
Only relevant when generating an EmulationStation game list (which is the default frontend when the `-f` option is left out). This forces the rom and any media paths (if they are the same as the input folder) inside the game list to be relative to the rom input folder. Consider setting this in [`config.ini`](CONFIGINI.md#relativepathsfalse) instead.
#### skipexistingcovers
When generating gamelists, skip processing covers that already exist in the media output folder.
#### skipexistingmarquees
When generating gamelists, skip processing marquees that already exist in the media output folder.
#### skipexistingscreenshots
When generating gamelists, skip processing screenshots that already exist in the media output folder.
#### skipexistingvideos
When generating gamelists, skip copying videos that already exist in the media output folder.
#### skipexistingwheels
When generating gamelists, skip processing wheels that already exist in the media output folder.
#### skipped
If a rom has no resources attached to it in the cache, it will be left out when generating a game list file. It will still show up in the frontend (at least it does for EmulationStation) but it won't exist in the game list file. You can safely leave out this flag unless you need the empty entries for some reason. Consider setting this in [`config.ini`](CONFIGINI.md#skippedfalse) instead.
#### symlink
Enabling this flag is currently only relevant while also using the `videos` flag. It basically means that Skyscraper will create a link to the cached videos instead of copying them when generating the game list media files. This will save a lot of space, but has the caveat that if you somehow remove the videos from the cache, the links will be broken and the videos then won't show anymore. Consider setting this in [`config.ini`](CONFIGINI.md#symlinkfalse) instead.
#### theinfront
Game titles are returned from the scraping sources sometimes as 'The Game' and other times as 'Game, The'. Enabling this flag will force Skyscraper to always try and move 'The' to the front of the titles. If it is not enabled, Skyscraper will always try and move it to the end of the title, regardless of how it was originally returned by the scraping sources.

NOTE! When generating gamelists Skyscraper will still sort the games as if the game titles didn't have 'The' at the beginning.
#### unattend
When generating a game list Skyscraper will check if it already exists and ask if you want to overwrite it. And it will also ask if you wish to skip existing game list entries. By using this flag Skyscraper will *always* overwrite an existing game list and *never* skip existing entries. This is useful when scripting Skyscraper to avoid the need for user input. Consider setting this in [`config.ini`](CONFIGINI.md#unattendtrue) instead.
#### unattendskip
When generating a game list Skyscraper will check if it already exists and ask if you want to overwrite it. And it will also ask if you wish to skip existing game list entries. By using this flag Skyscraper will *always* overwrite an existing game list and *always* skip existing entries. This is useful when scripting Skyscraper to avoid the need for user input. Consider setting this in [`config.ini`](CONFIGINI.md#unattendskipfalse) instead.
#### unpack
Some scraping modules use file checksums to identify the game in their databases. If you've compressed your roms to zip or 7z files yourself, this can pose a problem in getting a good result. You can then try to use this flag. Doing so will extract the rom and do the file checksum on the rom itself instead of the compressed file.

NOTE! Only use this flag if you are having problems getting the roms identified from the compressed files. It slows down the scraping process significantly and should therefore be avoided if possible.
#### videos
By default Skyscraper doesn't scrape and cache video resources because of the significant disk space required to save them. You can enable videos using this flag. Consider setting this in [`config.ini`](CONFIGINI.md#videosfalse) instead.

###### Example(s)
```
Skyscraper -p amiga --flags forcefilename,nosubdirs,skipexistingwheels
Skyscraper -p nes --flags videos,nomarquees
```

### --cache <COMMAND[:OPTIONS]>
This is the cache master option. It contains several subcommands that allows you to manipulate the cached data for the selected platform.

NOTE! For any of these commands you can set a non-default resource cache folder with the `-d` option. The folder pointed to should be a folder with a Skyscraper `db.xml` file and its required subfolders inside of it (`covers`, `screenshots` etc.).

Read more about the resource cache [here](CACHE.md).

#### --cache help
Outputs a description of all available `--cache` functions.

#### --cache edit[:new=&lt;TYPE&gt;]
Allows editing of any cached resources connected to your roms. The editing mode will go through each of the files in the queue one by one, allowing you to add and remove resources as needed. Any resource you add manually will be prioritized above all others.

You can provide one or more filenames to the end of the command line or use the `--fromfile` option to edit the resources for just those files. You can use the `--startat` and `--endat` options to edit a span of roms. If none of those options are used, it will edit all of the roms in the input folder one by one.

For efficiency, when adding a lot of resources of the same type, you can also add the optional 'new=<TYPE>' which will make it very easy to batch insert resources of the defined type to all the files you are editing. TYPE can be any of the known textual resources: title, platform, releasedate, developer, publisher, players, ages, genres, rating, description.

###### Example(s)
```
Skyscraper -p snes --cache edit <FILENAME 1> <FILENAME 2>
Skyscraper -p snes --cache edit --startat <FILENAME> --endat <FILENAME>
Skyscraper -p snes --cache edit:new=developer --startat <FILENAME> --endat <FILENAME>
Skyscraper -p snes --cache edit
Skyscraper -p snes --cache edit --fromfile "/home/pi/.skyscraper/reports/report-snes-missing_developer-20190708.txt"
Skyscraper -p snes --cache edit:new=ages --fromfile "/home/pi/.skyscraper/reports/report-snes-missing_ages-20190708.txt"
```

#### --cache merge:&lt;FOLDER&gt;
This option allows you to merge two resource caches together. It will merge the cache located at the `<FOLDER>` location into the default cache for the chosen platform. The path specified must be a path containing the `db.xml` file. You can also set a non-default destination to merge to with the `-d` option.

###### Example(s)
```
Skyscraper -p snes --cache merge:"path to/source/cache/snes"
Skyscraper -p snes --cache merge:"path to/source/cache/snes" -d "/path to/nondefault/destination/cache/snes"
```

#### --cache purge:&lt;KEYWORD|MODULE and/or TYPE&gt;
This is a powerful option that allows you to purge the requested resources from the resource cache connected to the selected platform.

You can purge *all* resources from the cache for the chosen platform using the keyword `all`.

You can purge specific resources from a certain module with `m=<MODULE>` or of a certain type with `t=<TYPE>` or a combination of the two separated by a `,`.

Supported modules can be seen under `-s` when using the `--help` option. Supported types are: `title`, `platform`, `description`, `publisher`, `developer`, `ages`, `tags`, `rating`, `releasedate`, `cover`, `screenshot`, `wheel`, `marquee`, `video`.

Warning! Purging anything from the cache cannot be undone, so please consider making a backup.

###### Example(s)
```
Skyscraper -p snes --cache purge:all
Skyscraper -p snes --cache purge:m=thegamesdb
Skyscraper -p snes --cache purge:t=cover
Skyscraper -p snes --cache purge:m=thegamesdb,t=cover
```

#### --cache refresh
Same as [--refresh](#--refresh).

###### Example(s)
```
Skyscraper -p snes -s screenscraper --cache refresh
```

#### --cache report:missing=&lt;all, textual, artwork, media or RESOURCE1,RESOURCE2,...&gt;
Will create report(s) containing all filenames of games missing the selected resource type(s). File(s) will be exported to `/home/USER/.skyscraper/reports/report-<PLATFORM>-missing_<RESOURCE>-yyyymmdd.txt`

You can use any of the following:
* all: Creates reports for all resource types
* textual: Creates reports for all textual resource types
* artwork: Creates reports for all artwork related resource types excluding 'video'
* media: Creates reports for all media resource types including 'video'
* type1,type2,type3,...:

Supported resource types are: `title`, `platform`, `description`, `publisher`, `developer`, `ages`, `tags`, `rating`, `releasedate`, `cover`, `screenshot`, `wheel`, `marquee`, `video`.

NOTE! The reports can be fed back into Skyscraper using the `--fromfile <REPORTFILE>` option, which tells Skyscraper to only work on the files contained in the report. This is useful in combination with, for instance, the `--cache edit` option or the `--cache refresh`/`--refresh` (they are the same) option(s).

###### Example(s)
```
Skyscraper -p snes --cache report:missing=textual
Skyscraper -p snes --cache report:missing=artwork
Skyscraper -p snes --cache report:missing=developer
Skyscraper -p snes --cache report:missing=publisher,screenshot
```

#### --cache show
Shows the cache stats for the chosen platform. It will list how many resources of each type you currently have cached for each scraping module.

###### Example(s)
```
Skyscraper -p snes --cache show
```

#### --cache vacuum
You can purge all resources that don't have any connection to your current romset for the selected platform by using the `vacuum` command. This is extremely useful if you've removed a bunch of roms from your collection and you wish to purge any cached data you don't need anymore.

Warning! Vacuuming the cache cannot be undone, so please consider making a backup.

###### Example(s)
```
Skyscraper -p snes --cache vacuum
```

#### --cache validate
This will test the integrity of the resource cache connected to the chosen platform. It will remove / clean out any stray files that aren't connected to an entry in the cache and vice versa. It's not really necessary to use this option unless you have manually deleted any of the cached files or entries in the `db.xml` file connected to the platform.

NOTE! This option doesn't clean up your game list media folders. You will need to do that yourself since Skyscraper has no idea what files you might keep in those folders. This option only relates to the resource cache database and related files.

###### Example(s)
```
Skyscraper -p snes --cache validate
```

### --videos
This option is deprecated! Please set it using the [`--flags`](#--flags-flag1flag2) option instead.

### --symlink
This option is deprecated! Please set it using the [`--flags`](#--flags-flag1flag2) option instead.

### --nocovers
This option is deprecated! Please set it using the [`--flags`](#--flags-flag1flag2) option instead.

### --noscreenshots
This option is deprecated! Please set it using the [`--flags`](#--flags-flag1flag2) option instead.

### --nowheels
This option is deprecated! Please set it using the [`--flags`](#--flags-flag1flag2) option instead.

### --nomarquees
This option is deprecated! Please set it using the [`--flags`](#--flags-flag1flag2) option instead.

### --nobrackets
This option is deprecated! Please set it using the [`--flags`](#--flags-flag1flag2) option instead.

### --skipped
This option is deprecated! Please set it using the [`--flags`](#--flags-flag1flag2) option instead.

### --noresize
This option is deprecated! Please set it using the [`--flags`](#--flags-flag1flag2) option instead.

### --fromfile &lt;FILENAME&gt;
Using this option is the equivalent to adding a bunch of filenames to work on directly on the commandline. It reads one line at a time from `<FILENAME>` and adds them to the queue of files to work on. This is very useful in combination with the `--cache edit` option or if you want to refresh data for just those files using `-s <SCRAPING MODULE>`.

NOTE! The `--cache report` option creates files that are directly usable with the `--fromfile` option.

###### Example(s)
```
Skyscraper -p snes --cache edit --fromfile "/home/pi/.skyscraper/reports/report-snes-missing_developer-20190708.txt"
Skyscraper -p snes -s screenscraper --fromfile "/home/pi/.skyscraper/reports/report-snes-missing_developer-20190708.txt"
```

### --onlymissing
This option is deprecated! Please set it using the [`--flags`](#--flags-flag1flag2) option instead.

### --startat &lt;FILENAME&gt;
If you wish to work on a subset of your roms you can use this option to set the starting rom. Use it in conjunction with the `--endat` option described below to further narrow the subset of files.

NOTE 1! Enabling this option automatically sets the `--refresh` option and enables the `nosubdirs` flag.

NOTE 2! Instead of using this option, if you just want to gather resources for 1 or 2 roms, you can provide the filename(s) directly on the command like so: `$ Skyscraper -p snes -s thegamesdb "/full/or/partial/path/to/rom.zip"`. You can also use the `--fromfile` option.

###### Example(s)
```
Skyscraper -p snes --cache edit --startat "rom name.zip"
Skyscraper -p snes -s thegamesdb --startat "partial/path/to/rom name.zip"
```

### --endat &lt;FILENAME&gt;
If you wish to work on a subset of your roms you can use this option to set the rom to end at. Use it in conjunction with the `--startat` option described above to further narrow the subset of files.

NOTE 1! Enabling this option automatically sets the `--refresh` option and enables the `nosubdirs` flag.

NOTE 2! Instead of using this option, if you just want to gather resources for 1 or 2 roms, you can provide the filename(s) directly on the command like so: `$ Skyscraper -p snes -s thegamesdb "/full/or/partial/path/to/rom.zip"`. You can also use the `--fromfile` option.

###### Example(s)
```
Skyscraper -p snes --cache edit --endat "rom name.zip"
Skyscraper -p snes -s thegamesdb --endat "partial/path/to/rom name.zip"
```

### --includepattern &lt;PATTERN 1,PATTERN 2&gt;
Per platform Skyscraper have default file extensions that it will accept. This option allows you to only include certain files within that scope. The pattern is a simple asterisk type pattern. You can add several patterns by separating them with ','. In cases where you need to match for a comma you need to escape it as '\,' (see last example).

IMPORTANT! Remember to double-quote the pattern as seen in the examples to avoid odd behaviour.

NOTE! You might also want to check out the file extension options.

###### Example(s)
```
Skyscraper -p snes -s screenscraper --includepattern "Super*"
Skyscraper -p amiga --includepattern "*AGA*"
Skyscraper -p amiga --includepattern "*AGA*,Super*"
Skyscraper -p amiga --includepattern "*AGA*,Super*,*\, The"
```

### --excludepattern &lt;PATTERN 1, PATTERN 2&gt;
Per platform Skyscraper have default file extensions that it will accept. This option allows you to exclude certain files within that scope. The pattern is a simple asterisk type pattern. You can add several patterns by separating them with ','. In cases where you need to match for a comma you need to escape it as '\,' (see last example).

IMPORTANT! Remember to double-quote the pattern as seen in the examples to avoid odd behaviour.

NOTE 1! You might also want to check out the file extension options.

NOTE 2! You might also want to check out the '--excludefrom' option.

NOTE 3! If you create a file named `.skyscraperignore` within any subfolder of the input dir, all files from that directory will be ignored by Skyscraper.

###### Example(s)
```
Skyscraper -p snes -s screenscraper --excludepattern "*[BIOS]*"
Skyscraper -p amiga --excludepattern "AGA*"
Skyscraper -p amiga --excludepattern "*AGA*,Super*"
Skyscraper -p amiga --excludepattern "*AGA*,Super*,*\, The"
```

#### --includefrom &lt;FILENAME&gt;
Tells Skyscraper to only include the files listed in FILENAME. One filename per line (with FULL path, eg. '/home/pi/RetroPie/roms/snes/subdir/somefile.zip').

This file can be generated with the '--cache report:missing' option or made manually.

NOTE! You might also want to check out the 'includepattern' option.

###### Example(s)
```
Skyscraper -p snes -s screenscraper --includefrom "/home/pi/.skyscraper/includes.txt"
```

#### --excludefrom &lt;FILENAME&gt;
Tells Skyscraper to exclude the files listed in FILENAME. One filename per line (with FULL path, eg. '/home/pi/RetroPie/roms/snes/subdir/somefile.zip').

This file can be generated with the '--cache report:missing' option or made manually.

NOTE! You might also want to check out the 'excludepattern' option.

###### Example(s)
```
Skyscraper -p snes -s screenscraper --excludefrom "/home/pi/.skyscraper/excludes.txt"
```

### --maxfails &lt;1-200&gt;
Not all scraping modules support all platforms. This means that you can potentially start a scraping run with a module and a platform that is incompatible. This will hammer the servers for potentially hundreds of roms but provide 0 results for any of them. To avoid this Skyscraper has a builtin limit for initially allowed failed rom lookups. If this is reached it will quit. Setting this option allows you to set this limit yourself, but not above a maximum of 200. The default limit is 42. Don't change this unless you have a very good reason to do so.

###### Example(s)
```
Skyscraper -p snes -s thegamesdb --maxfails 75
```

### --pretend
This option is deprecated! Please set it using the [`--flags`](#--flags-flag1flag2) option instead.

### --unattend
This option is deprecated! Please set it using the [`--flags`](#--flags-flag1flag2) option instead.

### --unattendskip
This option is deprecated! Please set it using the [`--flags`](#--flags-flag1flag2) option instead.

### --interactive
This option is deprecated! Please set it using the [`--flags`](#--flags-flag1flag2) option instead.

### --query &lt;STRING&gt;
For most modules a search query is sent to the scraping module in an URL format. This means that a filename such as "Rick Dangerous.lha" becomes "rick+dangerous". The '+' here means a space. You could probably also use the URL encoded space "rick%20dangerous" but my tests show that most modules expect spaces as '+'. And it is the "rick+dangerous" part that you, as the user, can pass as the query, like so:
```
$ Skyscraper -p <PLATFORM> -s <MODULE> --query "rick+dangerous" <FILENAME>
```
Remember to also add a filename that you wish to use the override with. Otherwise the query will be ignored.

But not all of the scraping modules are search name based. For instance, the `screenscraper` module can use a variety of different search methods. So for screenscraper you also have the option of overriding the checksums it uses to search for a game. This is especially convenient in cases where a filename exists multiple times in their database and your own local file doesn't match with any of the connected checksums (maybe you've compressed the rom yourself). In this case you can look up one of the working checksums on the Screenscraper website (screenscraper.fr) and override the checksum.

You can use any combination of `crc=<CHECKSUM>`, `md5=<CHECKSUM>`, `sha1=<CHECKSUM>` and `romnom=<FILENAME>` (without the `<` and `>`! Also "romnom" is "rom name" in French - Screenscraper is French). Most times you only need one of these, but you can combine them by separating them with a `&`.

The `--query` option is an *experts only* option, but it's very useful to get results for those last difficult roms.

###### Example(s)
```
$ Skyscraper -p snes -s thegamesdb --query "rick+dangerous" /full/or/partial/path/to/rom.zip
$ Skyscraper -p snes -s screenscraper --query "md5=<CHECKSUM>" /full/or/partial/path/to/rom.zip
$ Skyscraper -p snes -s screenscraper --query "romnom=file%20name.zip" /full/or/partial/path/to/rom.zip
$ Skyscraper -p snes -s screenscraper --query "sha1=<CHECKSUM>&romnom=file%20name.zip" /full/or/partial/path/to/rom.zip
```

### --forcefilename
This option is deprecated! Please set it using the [`--flags`](#--flags-flag1flag2) option instead.

### --relative
This option is deprecated! Please set it using the [`--flags`](#--flags-flag1flag2) option instead.

### --addext &lt;EXTENSION&gt;
If you have a rom that Skyscraper doesn't even try to gather data for, it might be because it has a file extension that isn't currently supported. This option allows you to temporarily add support for any file extension. If you feel like you are using a file extension that ought to be supported by default, please report it so it can be added in a later version of Skyscraper. Consider setting this in [`config.ini`](CONFIGINI.md#addextensionszst-smt) instead.

###### Example(s)
```
Skyscraper -p snes -s thegamesdb --addext *.ext
Skyscraper -p snes -s thegamesdb --addext "*.ext1 *.ext2"
```

### --lang &lt;CODE&gt;
Adds the specified language to the top of the existing default internal language priority list. Read more about languages [here](LANGUAGES.md). Only one language is supported with this configuration. For a permanent setup you should consider setting this in [`config.ini`](CONFIGINI.md#langen) instead.

###### Example(s)
```
Skyscraper -p snes -s screenscraper --lang es
```

### --region &lt;CODE&gt;
Adds the specified region to the top of the existing default internal region priority list. Only one region is supported with this configuration. For a permanent setup you should consider setting this in [`config.ini`](CONFIGINI.md#regionwor) instead.

NOTE 1! Read more about how regions are handled in general [here](REGIONS.md).

NOTE 2: Setting this will overwrite any region [auto-detected](REGIONS.md#region-auto-detection) from the file name.

###### Example(s)
```
Skyscraper -p snes -s screenscraper --region jp
```

### --nohints
This option is deprecated! Please set it using the [`--flags`](#--flags-flag1flag2) option instead.

### --verbosity &lt;0-3&gt;
Sets how verbose Skyscraper should be when running. Default level is 0. The higher the value, the more info Skyscraper will output to the terminal while running. Consider setting this in [`config.ini`](CONFIGINI.md#verbosity1) instead.

###### Example(s)
```
Skyscraper -p snes -s screenscraper --verbosity 3
```
