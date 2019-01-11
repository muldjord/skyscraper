## Command line options
Look below to find a thorough description of all command line options available when using Skyscraper. These options should be used with the `Skyscraper` command.

Please take note that almost all of these options are set at a useful default (and can therefore be left out) and should only be used / changed if your use case requires it.

If you've installed Skyscraper through the RetroPie-Setup the executable is instead located at `/opt/retropie/supplementary/skyscraper/Skyscraper`. In that case I recommend creating a symbolic link to the executable. Do this by running `$ sudo ln -s /opt/retropie/supplementary/skyscraper/Skyscraper /usr/local/bin/Skyscraper` (without the `$`). This will enable you to just type `Skyscraper` when running it from command line.

IMPORTANT! Most of the options can also be set in the `~/.skyscraper/config.ini` file thus removing the need to type them on command line all the time. Check the config.ini doc [here](CONFIGINI.md) for more info on this.

#### -h, --help
Outputs the help text for all command line options to the terminal.
###### Example(s)
```
Skyscraper --help
Skyscraper -h
```

#### -p &lt;platform&gt;
Sets the platform you wish to scrape. Supported platforms can be seen using the `--help` option described above.

Running the following commands will scrape from all cached resources and generate a game list and composite artwork using the recipe in `~/.skyscraper/artwork.xml` (check the artwork documentation for more info on this [here](ARTWORK.md).

Before running these commands you need to first gather some data into the cache. Please read the description of `-s <module>` below.
###### Example(s)
```
Skyscraper -p amiga
Skyscraper -p snes

```

#### -s &lt;module&gt;
Sets which scraping module you wish to gather data from. All data scraped from any of the modules will be cached in the localdb resource cache and can then later be used to generate a game list for your frontend. Read more about this in the `-p <platform>` description above.

To generate a game list from the localdb resource cache, just leave out the `-s` option entirely.
###### Example(s)
```
Skyscraper -p amiga -s openretro
Skyscraper -p snes -s screenscraper
Skyscraper -p amiga -s esgamelist
Skyscraper -p snes -s import
```
To read more about each scraping module, check [here](SCRAPINGMODULES.md).

#### -u &lt;key or user:password&gt;
Some scraping modules require a user key or a user id + password to work. Check the scraping module overview to see the specific requirements for each module [here](SCRAPINGMODULES.md).
###### Example(s)
```
Skyscraper -p snes -s screenscraper -u <your key>
```

#### -i &lt;path&gt;
Sets the rom input folder. By default Skyscraper will look for roms in the `/home/[user]/RetroPie/roms/[platform]` folder. If your roms are located in a non-default location, you can set the input path using this option.
###### Example(s)
```
Skyscraper -p snes -i "/path/to/your/snes/roms"
```

#### --nosubdirs
By default Skyscraper will include roms located in subfolders. By adding this option Skyscraper will only scrape the roms located directly in the input folder. See `-i <path>` above to read more about the rom input folder.
###### Example(s)
```
Skyscraper -p snes --nosubdirs
```

#### --unpack
Some scraping modules use file checksums to identify the game in their databases. If you've compressed your roms to zip or 7z files yourself, this can pose a problem in getting a good result. You can then try to use this option. Doing so will extract the rom and do the file checksum on the rom itself instead of the compressed file.

NOTE! Only use this option if you are having problems getting the roms identified from the compressed files. It slows down the scraping process significantly and should therefore be avoided if possible.
###### Example(s)
```
Skyscraper -p snes -s screenscraper --unpack
```

#### -g &lt;path&gt;
Sets the game list export folder. By default Skyscraper exports the game list to the same directory as the rom input folder. This enables you to change that to a non-default location.
###### Example(s)
```
Skyscraper -p snes -s screenscraper -g "/your/desired/game list/export/path"
```

#### -o &lt;path&gt;
Sets the artwork / media output folder. By default Skyscraper outputs the composited artwork files to the game list export folder + `/media`. This allows you to change that to a non-default location. Read more about the artwork compositing [here](ARTWORK.md).
###### Example(s)
```
Skyscraper -p snes -s screenscraper -o "/path/to/where/you/want/the/artwork/files"
```

#### -t &lt;1-8&gt;
Sets the desired number of parallel threads to be run when scraping. NOTE! Some modules have maximum allowed threads. If you set this higher than the allowed value, it will be auto-adjusted.
###### Example(s)
```
Skyscraper -p snes -s thegamesdb -t 5
```

#### -f &lt;frontend&gt;
Sets the frontend you wish to export a game list for. By default Skyscraper will export an EmulationStation game list, but other frontends are supported aswell. If exporting for the `attractmode` frontend, please also take note of the required `-e` option that goes along with using the `attractmode` frontend.
###### Example(s)
```
Skyscraper -p snes -f attractmode -e snes
```

#### -e &lt;emulator&gt;
NOTE! This option is *only* applicable when using the `-f attractmode` option. It sets the *emulator* to be used when generating the `attractmode` game list. On RetroPie the emulator name is mostly the same as the platform.
###### Example(s)
```
Skyscraper -p snes -f attractmode -e snes
```

#### -m &lt;0-100&gt;
Some scraping modules are based on a file name or title based search. This option sets the minimum percentage any returned results need to match with in order for it to be accepted. For instance, the game `Wonderboy in Monsterland` might return the title `Wonder Boy in Monster Land` which is clearly a match. But it's not a 100% match. So it needs to be set relatively high, while still ignoring bad matches. By default it is set to 65 which has been tested to be a good middle-ground.
###### Example(s)
```
Skyscraper -p snes -m 50
```

#### -l &lt;0-10000&gt;
Sets the maximum length of returned game descriptions. This is a convenience option if you feel like game descriptions are too long. By default it is set to 2500.
###### Example(s)
```
Skyscraper -p snes -l 500
```

#### -c &lt;filename&gt;
Sets a non-default config file. By default it Skyscraper uses the file `~/.skyscraper/config.ini`.
###### Example(s)
```
Skyscraper -p snes -c /path/to/config.ini
```

#### -a &lt;filename&gt;
Sets a non-default xml file to use when setting up the artwork compositing. By default it Skyscraper uses the file `~/.skyscraper/artwork.xml`.
###### Example(s)
```
Skyscraper -p snes -a /path/to/artwork.xml
```

#### -d &lt;folder&gt;
Sets a non-default location for the storing and loading of cached game resources. This is what is referred to in the docs as the "localdb resource cache". By default this folder is set to `~/.skyscraper/dbs/[platform set with -p]`. Unless you have a good reason to, don't change this. The folder pointed to should a folder with a Skyscraper `db.xml` file and its required subfolders inside of it.
###### Example(s)
```
Skyscraper -p snes -d /custom/localdb/cache/path
```

#### --refresh
Skyscraper has a localdb resource cache which works just like the browser cache in Firefox. If you scrape and gather resources for a platform with the same scraping module twice, it will grab the data from the cache instead of hammering the online servers again. This has the advantage in the case whree you scrape a rom set twice, only the roms that weren't recognized the first time around will be looked up and fetched from the online servers. Everything else will be loaded from the cache.

You can force all data to be refetched from the servers by setting this option, effectively bypassing the cache.

NOTE! *Only* use this option if you know data has changed for several roms at the source. Otherwise you are hammering the servers for no reason.
###### Example(s)
```
Skyscraper -p snes -s screenscraper --refresh
```

#### --dbstats
This will print the stats for the localdb resource cache that is connected to the chosen platform and then quit. It will tell you how many resources of each type are cached for each scraping module for that particular platform. If you wish to purge all or some of the data from the cache, please check the `--purgedb` option.

NOTE! You can set a custom db folder to show stats for with the `-d` option.
###### Example(s)
```
Skyscraper -p snes --dbstats
```

#### --cleandb
This will test the integrity of the localdb resource cache connected to the chosen platform and then quit. If will remove / clean out any stray files that aren't connected to an entry in the cache and vice versa. It's not really necessary to use this option unless you have manually deleted any of the cached files or entries in the db.xml file connected to the platform.

NOTE! You can set a custom db folder to clean with the `-d` option.
###### Example(s)
```
Skyscraper -p snes --cleandb
```

#### --mergedb &lt;folder&gt;
This option allows you to merge two localdb resource caches together and then quit. It will merge the cache located at the `<folder>` location into the default cache for the chosen platform. You can also set a non-default destination to merge to with the `-d` option. The folder(s) pointed to should be the folder(s) where the `db.xml` file resides.
###### Example(s)
```
Skyscraper -p snes --mergedb /path/to/cache/folder
```

#### --purgedb &lt;resources&gt;
This is a powerful option that allows you to purge the requested resources from the localdb resource cache connected to the selected platform.

You can purge all resources from a certain module with `m:[module]` or of a certain type with `t:[type]` or a combination of the two separated by a `,`.

Supported modules can be seen under `-s` when using the `--help` option. Supported types are: `title`, `platform`, `description`, `publisher`, `developer`, `ages`, `tags`, `rating`, `releasedate`, `cover`, `screenshots`, `wheel`, `marquee`, `video`.

You can also purge all resources that don't have any connection to your current romset for the selected platform by using the `vacuum` keyword. This is extremely useful if you've removed a bunch of roms from your collection and you wish to purge any cached data you don't need anymore.

Lastly, you can purge *all* resources from the cache for the chosen platform using the keyword `all`.

NOTE! You can set a custom db folder to purge resources from with the `-d` option.

Warning! Using any of these commands cannot be undone, so please consider making a backup.
###### Example(s)
```
Skyscraper -p snes --purgedb vacuum
Skyscraper -p snes --purgedb m:thegamesdb
Skyscraper -p snes --purgedb t:cover
Skyscraper -p snes --purgedb m:arcadedb,t:publisher
Skyscraper -p snes --purgedb all
```

#### --videos
By default Skyscraper doesn't scrape and cache video resources because of the significant space required to save them. You can enable videos using this option.
###### Example(s)
```
Skyscraper -p snes -s screenscraper --videos
Skyscraper -p snes -s arcadedb --videos
Skyscraper -p snes --videos
```

#### --symlink
Enabling this option is only relevant while also using the `--videos`. It basically means that Skyscraper will create a link to the cached videos instead of copying them when generating the game list media files. This will save a lot of space, but has the caveat that if you somehow remove the videos from the cache, the links will be broken and the videos then won't show anymore.
###### Example(s)
```
Skyscraper -p snes -s screenscraper --videos --symlink
Skyscraper -p snes -s arcadedb --videos --symlink
Skyscraper -p snes --videos --symlink
```

#### --nocovers
Disables the caching of the resource type `cover` when scraping with any module. If you never use covers in your artwork configuration, this can save you some space.
###### Example(s)
```
Skyscraper -p snes -s thegamesdb --nocovers
```

#### --noscreenshotss
Disables the caching of the resource type `cover` when scraping with any module. If you never use covers in your artwork configuration, this can save you some space.
###### Example(s)
```
Skyscraper -p snes -s thegamesdb --noscreenshotss
```

#### --nowheels
Disables the caching of the resource type `cover` when scraping with any module. If you never use covers in your artwork configuration, this can save you some space.
###### Example(s)
```
Skyscraper -p snes -s thegamesdb --nowheels
```

#### --nomarquees
Disables the caching of the resource type `cover` when scraping with any module. If you never use covers in your artwork configuration, this can save you some space.
###### Example(s)
```
Skyscraper -p snes -s thegamesdb --nomarquees
```

#### --nobrackets
Use this option to disable any bracket notes when generating the game list. It will disable notes such as `(Europe)` and `[AGA]` completely. This option is only relevant when generating the game list. It makes no difference when gathering data into the localdb resource cache.
###### Example(s)
```
Skyscraper -p snes --nobrackets
```

#### --skipped
If a rom has no resources attached to it in the cache, it will be left out when generating a game list file. It will still show up in the frontend (at least it does for EmulationStation) but it won't exist in the game list file. You can safely leave out this option unless you need the empty entries for some reason.
###### Example(s)
```
Skyscraper -p snes --skipped
```

#### --noresize
By default, to save space, Skyscraper resizes large pieces of artwork before adding them to the localdb resource cache. Adding this option will disable this and save the artwork files exactly as they are retrieved from the scraping module.

NOTE! This is not related to the artwork compositing that happens when generating a game list. This is *only* related to how Skyscraper handles artwork when adding it to the localdb resource cache while gathering data from the scraping modules.
###### Example(s)
```
Skyscraper -p amiga -s openretro --noresize
```

#### --startat &lt;filename&gt;
If you wish to gather data for a subset of your roms from the scraping modules you can use this option to set the starting rom. It will then scrape alphabetically from that rom and forwards. Use it in conjunction with the `--endat` option described below to further narrow the subset of files you wish to scrape.

NOTE 1! Enabling this option automatically sets the `--refresh` and `--nosubdirs` options.

NOTE 2! Instead of using this option, if you just want to scrape and cache data for 1 or 2 roms you can provide the filename(s) directly on the command like so: `$ Skyscraper -p snes -s thegamesdb /full/or/partial/path/to/rom.zip`.
###### Example(s)
```
Skyscraper -p snes -s thegamesdb --startat "rom name.zip"
Skyscraper -p snes -s thegamesdb --startat "partial/path/to/rom name.zip"
```

#### --endat &lt;filename&gt;
If you wish to gather data for a subset of your roms from the scraping modules you can use this option to set the rom to end at. It will then scrapes alphabetically until it reaches this rom, then stops. Use it in conjunction with the `--startat` option described above to further narrow the subset of files you wish to scrape.

NOTE 1! Enabling this option automatically sets the `--refresh` and `--nosubdirs` options.

NOTE 2! Instead of using this option, if you just want to scrape and cache data for 1 or 2 roms you can provide the filename(s) directly on the command like so: `$ Skyscraper -p snes -s thegamesdb /full/or/partial/path/to/rom.zip`.
###### Example(s)
```
Skyscraper -p snes -s thegamesdb --endat "rom name.zip"
Skyscraper -p snes -s thegamesdb --endat "partial/path/to/rom name.zip"
```

#### --maxfails &lt;1-200&gt;
Not all scraping modules support all platforms. This means that you can potentially start a scraping run with a module and a platform that is incompatible. This will hammer the servers for potentially hundreds of roms but provide 0 results for any of them. To avoid this Skyscraper has a builtin limit for initially allowed failed rom lookups. If this is reached it will quit. Setting this option allows you to set this limit yourself, but not above a maximum of 200. The default limit is 42. Don't change this unless you have a very good reason to do so.
###### Example(s)
```
Skyscraper -p snes -s thegamesdb --maxfails 75
```

#### --pretend
This option is *only* relevant when generating a game list. It disables the game list generator and artwork compositor and only outputs the results of the potential game list generation to the terminal. It can be very useful to check exactly what and how the data will be combined from the localdb resource cache.
###### Example(s)
```
Skyscraper -p snes --pretend
```

#### --unattend
By default Skyscraper won't overwrite an existing game list and it will ask if you wish to skip existing game list entries. Bu using this option Skyscraper will *always* overwrite an existing game list and *never* skip existing entries. This is useful when scripting Skyscraper to avoid the need of user input.
###### Example(s)
```
Skyscraper -p snes --unattend
```

#### --unattendskip
By default Skyscraper won't overwrite an existing game list and it will ask if you wish to skip existing game list entries. Bu using this option Skyscraper will *always* overwrite an existing game list and *always* skip existing entries. This is useful when scripting Skyscraper to avoid the need of user input.
###### Example(s)
```
Skyscraper -p snes --unattendskip
```

#### --interactive
When gathering data from any of the scraping modules many potential entries will be returned. Normally Skyscraper chooses the best entry for you. But should you wish to choose the best entry yourself, you can enable this option. Skyscraper will then list the returned entries and let you choose which one is the best one.
###### Example(s)
```
Skyscraper -p snes -s thegamesdb --interactive
```

#### --query &lt;string&gt;
For most modules a search query is sent to the scraping module in an URL format. That means that a filename such as "Rick Dangerous.lha" becomes "rick+dangerous". The '+' here means a space. You could probably also use the URL encoded space "rick%20dangerous" but my tests show that most modules expect spaces as '+'. And it is the "rick+dangerous" that you, as the user, can pass as the query, like so:
```
$ Skyscraper -p [platform] -s [module] --query "rick+dangerous" [filename]
```
Remember to also add a filename that you wish to use the override with. Otherwise the query will be ignored.

But not all of the scraping modules are search name based. For instance, the `screenscraper` module can use a variety of different search methods. So for screenscraper you also have the option of overriding the checksums it use to search for a game. This is especially convenient in cases where a filename exists multiple times in their database and your own local file doesn't match with any of the connected checksums (maybe you've compressed the rom yourself or whatever).
In this case you can look up one of the working checksums on the Screenscrapers website (screenscraper.fr) and override the checksum like these examples:

$ Skyscraper -p [platform] -s [module] --query sha1=[checksum] [filename]
$ Skyscraper -p [platform] -s [module] --query md5=[checksum] [filename]
$ Skyscraper -p [platform] -s [module] --query sha1=[checksum]&md5=[checksum]&romnom=[exact url encoded filename] [filename]

The last example combines two of the checksum options and even the `romnom` attribute which is "rom name" in French (Screenscraper is French). You obviously only need one of the checksum options, it's just to show that you can combine them if you really need to.

The `--query` option is an *experts only* option, but it's very useful to get results for those last difficult roms.
###### Example(s)
```
$ Skyscraper -p snes -s thegamesdb --query "rick+dangerous" /full/or/partial/path/to/rom.zip
$ Skyscraper -p snes -s screenscraper --query "md5=[checksum]" /full/or/partial/path/to/rom.zip
$ Skyscraper -p snes -s screenscraper --query "romnom=file name.zip" /full/or/partial/path/to/rom.zip
```

#### --forcefilename
This option forces Skyscraper to use the file name instead of the cached titles when generating a game list.
###### Example(s)
```
Skyscraper -p snes --forcefilename
```

#### --relative
Only relevant when generating an EmulationStation game list (which is the default frontend when the `-f` option is left out). This forces the rom and any media paths inside the game list to be relative to the rom input folder.
###### Example(s)
```
Skyscraper -p snes --relative
```

#### --addext &lt;extension&gt;
If you have a rom that Skyscraper doesn't even try to gather data for, it's probably because it has a file extension that isn't currently supported. This option allows you to temporarily add support for any file extension. If you feel like you are using a file extension that ought to be supported by default, please report it so it can be added in a later version.
###### Example(s)
```
Skyscraper -p snes -s thegamesdb --addext *.ext
Skyscraper -p snes -s thegamesdb --addext "*.ext1 *.ext2"
```

#### --lang &lt;code&gt;
Sets the desired langauge when gathering data into the localdb resource cache. This option is only relevant for certain scraping modules. Get the details [here](LANGUAGES.md).
###### Example(s)
```
Skyscraper -p snes -s screenscraper --lang es
```

#### --lang &lt;code&gt;
Sets the desired region when gathering data into the localdb resource cache. This option is only relevant for certain scraping modules. Get the details [here](REGIONS.md).
###### Example(s)
```
Skyscraper -p snes -s screenscraper --region jp
```

#### --nohints
Diables the "Did you know" hints when running Skyscraper.
###### Example(s)
```
Skyscraper -p snes --nohints
```

#### --verbosity &lt;0-3&gt;
Sets how verbose Skyscraper should be when running. Default level is 0. The higher the value, the more info Skyscraper will print while running.
###### Example(s)
```
Skyscraper -p snes -s screenscraper --verbosity 3
```
