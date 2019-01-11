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

#### -p <platform>
Sets the platform you wish to scrape. Supported platforms can be seen using the `--help` option described above.

Running the following commands will scrape from all cached resources and generate a game list and composite artwork using the recipe in `~/.skyscraper/artwork.xml` (check the artwork documentation for more info on this [here](ARTWORK.md).

Before running these commands you need to first gather some data into the cache. Please read the description of `-s <module>` below.
###### Example(s)
```
Skyscraper -p amiga
Skyscraper -p snes

```

#### -s <module>
Sets which scraping module you wish to gather data from. All data scraped from any of the modules will be cached in the localdb resource cache and can then later be used to generate a game list for your frontend. Read more about this in the `-p <platform>` description above.
###### Example(s)
```
Skyscraper -p amiga -s openretro
Skyscraper -p snes -s screenscraper
Skyscraper -p amiga -s esgamelist
Skyscraper -p snes -s import
```
To read more about each scraping module, check [here](SCRAPINGMODULES.md).

#### -u <key or user:password>
Some scraping modules require a user key or a user id + password to work. Check the scraping module overview to see the specific requirements for each module [here](SCRAPINGMODULES.md).
###### Example(s)
```
Skyscraper -p snes -s screenscraper -u <your key>
```

#### -i <path>
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

#### -g <path>
Sets the game list export folder. By default Skyscraper exports the game list to the same directory as the rom input folder. This enables you to change that to a non-default location.
###### Example(s)
```
Skyscraper -p snes -s screenscraper -g "/your/desired/game list/export/path"
```

#### -o <path>
Sets the artwork output folder. By default Skyscraper outputs the composited artwork files to the game list export folder + `/media`. This allows you to change that to a non-default location. Read more about the artwork compositing [here](ARTWORK.md).
###### Example(s)
```
Skyscraper -p snes -s screenscraper -o "/path/to/where/you/want/the/artwork/files"
```

#### -t <1-8>
Sets the desired number of parallel threads to be run when scraping. NOTE! Some modules have maximum allowed threads. If you set this higher than the allowed value, it will be auto-adjusted.
###### Example(s)
```
Skyscraper -p snes -s thegamesdb -t 5
```

#### -f <frontend>
Sets the frontend you wish to export a game list for. By default Skyscraper will export an EmulationStation game list, but other frontends are supported aswell. If exporting for the `attractmode` frontend, please also take note of the required `-e` option that goes along with using the `attractmode` frontend.
###### Example(s)
```
Skyscraper -p snes -f attractmode -e snes
```

#### -e &lt;emulator&gt;
Note! This option is *only* applicable when using the `-f attractmode` option. It sets the *emulator* to be used when generating the `attractmode` game list. On RetroPie the emulator name is mostly the same as the platform.
###### Example(s)
```
Skyscraper -p snes -f attractmode -e snes
```

#### -m <0-100>
Some scraping modules are based on a file name or title based search. This option sets the minimum percentage any returned results need to match with in order for it to be accepted. For instance, the game `Wonderboy in Monsterland` might return the title `Wonder Boy in Monster Land` which is clearly a match. But it's not a 100% match. So it needs to be set relatively high, while still ignoring bad matches. By default it is set to 65 which has been tested to be a good middle-ground.
###### Example(s)
```
Skyscraper -p snes -s thegamesdb -m 50
```

#### -l &lt;0-10000&gt;
Sets the maximum length of returned game descriptions. This is a convenience option if you feel like game descriptions are too long. By default it is set to 2500.
###### Example(s)
```
Skyscraper -p snes -l 500
```

#### -c <filename>
Sets a non-default config file. By default it Skyscraper uses the file `~/.skyscraper/config.ini`.
###### Example(s)
```
Skyscraper -p snes -s thegamesdb -c /path/to/config.ini
```

#### 
###### Example(s)
```
Skyscraper -p snes -s thegamesdb -m 50
```

#### 
###### Example(s)
```
Skyscraper -p snes -s thegamesdb -m 50
```

#### 
###### Example(s)
```
Skyscraper -p snes -s thegamesdb -m 50
```

#### 
###### Example(s)
```
Skyscraper -p snes -s thegamesdb -m 50
```

#### 
###### Example(s)
```
Skyscraper -p snes -s thegamesdb -m 50
```
  -c <filename>              Use this config file to set up Skyscraper.
                             (default is '~/.skyscraper/config.ini')
  -a <filename>              Use this artwork xml file to set up the artwork
                             compositing.
                             (default is '~/.skyscraper/artwork.xml')
  -d <folder>                Set local resource database cache folder.
                             (default is '~/.skyscraper/dbs/[platform]')
  --nolocaldb                Disables the localdb resource cache. Other localdb
                             flags will then be ignored.
  --refresh                  Refresh resources in the localdb resource cache
                             from the selected scraping module.
  --dbstats                  Show stats for the localdb resource cache. This
                             will also be shown with a verbosity level of 1 or
                             more.
  --cleandb                  Remove media files that have no entry in the db
                             and vice versa. Set specific db folder with '-d'
                             otherwise default db folder is used.
  --mergedb <folder>         Merge data from the specified db folder into local
                             destination db. Set db you wish to merge from with
                             this flag. Set destination db folder with '-d'
                             otherwise default db folder is used as destination.
  --purgedb <resources>      Purges all requested resources from the localdb
                             resource cache. You can define either module
                             'm:[module]' or type 't:[type]' or both
                             comma-separated (example
                             'm:thegamesdb,t:description').
                             You can also just type 'vacuum' which will compare
                             your romset to any cached resource and remove the
                             resources that you no longer have roms for.
                             Lastly, you can type 'all' which will purge ALL
                             resources from the cache that are connected to the
                             currently selected platform / db folder.
                             Set specific db folder with '-d' otherwise default
                             db folder is used.
  --videos                   Enables video scraping for any scraping module.
                             Also enables caching of video resources in the
                             local databases. Beware, this takes up a lot of
                             disk space!
  --symlink                  Forces cached videos to be symlinked to game list
                             destination to save space. WARNING! Deleting or
                             moving files from your cache can invalidate the
                             links!
  --nocovers                 Disable covers/boxart from being cached locally.
                             Only do this if you do not plan to use the cover
                             artwork in 'artwork.xml'
  --noscreenshots            Disable screenshots/snaps from being cached
                             locally. Only do this if you do not plan to use the
                             screenshot artwork in 'artwork.xml'
  --nowheels                 Disable wheels from being cached locally. Only do
                             this if you do not plan to use the wheel artwork in
                             'artwork.xml'
  --nomarquees               Disable marquees from being cached locally. Only
                             do this if you do not plan to use the marquee
                             artwork in 'artwork.xml'
  --nobrackets               Disables any [] and () tags in the frontend game
                             titles.
  --skipped                  Include skipped entries when writing final
                             gamelist.
  --noresize                 Disable resizing of artwork when saving it to the
                             localdb resource cache. Normally they are resized
                             to save space. Setting this option will save them
                             as is. NOTE! This is NOT related to how Skyscraper
                             renders the artwork when scraping. Check the online
                             'Artwork' documentation to know more about this.
  --startat <filename>       Tells Skyscraper which file to start at. Forces
                             '--refresh' and '--nosubdirs' enabled.
  --endat <filename>         Tells Skyscraper which file to end at. Forces
                             '--refresh' and '--nosubdirs' enabled.
  --maxfails <1-200>         Sets the allowed number of initial 'Not found'
                             results before rage-quitting. (Default is 42)
  --pretend                  Don't alter any files, just print the results on
                             screen. This option is on by default for all
                             scraping modules except the 'localdb' module.
  --unattend                 Don't ask any initial questions when scraping. It
                             will then always overwrite existing gamelist and
                             not skip existing entries.
  --unattendskip             Don't ask any initial questions when scraping. It
                             will then always overwrite existing gamelist and
                             always skip existing entries.
  --interactive              Always ask user to choose best result.
  --query <string>           Allows you to set a custom search query (eg.
                             'rick+dangerous' for name based modules or
                             'sha1=[checksum]', 'md5=[checksum]' or
                             'romnom=[filename]' for the 'screenscraper'
                             module). Requires the single rom filename you wish
                             to override for to be passed on command line
                             aswell, otherwise it will be ignored.
  --forcefilename            Use filename as game name instead of the returned
                             game title.
  --relative                 Forces all gamelist paths to be relative to rom
                             location.
  --addext <extension>       Add this or these file extension(s) to accepted
                             file extensions during a scraping run. (example:
                             '*.zst' or '*.zst *.ext)
  --lang <code>              Set preferred result language for scraping modules
                             that support it.
                             (Default 'en')
  --region <code>            Set preferred game region for scraping modules
                             that support it.
                             (Default prioritization is 'eu', 'us', 'wor' and
                             'jp' in that order)
  --nohints                  Disables the 'DID YOU KNOW:' hints when running
                             Skyscraper.
  --verbosity <0-3>          Print more info while scraping
                             (Default is 0.)