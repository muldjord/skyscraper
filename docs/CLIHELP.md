## Command line options
Look below to find a thorough description of all command line options available when using Skyscraper. These options should be used with the `Skyscraper` command.

If you've installed Skyscraper through the RetroPie-Setup the executable is instead located at `/opt/retropie/supplementary/skyscraper/Skyscraper`. In that case I recommend creating a symbolic link to the executable. Do this by running `$ sudo ln -s /opt/retropie/supplementary/skyscraper/Skyscraper /usr/local/bin/Skyscraper` (without the `$`). This will enable you to just type `Skyscraper` when running it from command line.

#### -h, --help
Outputs the help text for all command line options to the terminal.
###### Example(s)
```
Skyscraper --help
Skyscraper -h
```

  -p <platform>              The platform you wish to scrape.
                             (Currently supports '3do', 'amiga', 'amstradcpc',
                             'apple2', 'arcade', 'atari800', 'atari2600',
                             'atari5200', 'atari7800', 'atarijaguar',
                             'atarilynx', 'atarist', 'c16', 'c64', 'c128',
                             'coco', 'coleco', 'daphne', 'dragon32',
                             'dreamcast', 'fba', 'fds', 'gameandwatch',
                             'gamegear', 'gb', 'gba', 'gbc', 'gc', 'genesis',
                             'intellivision', 'mame-advmame', 'mame-libretro',
                             'mame-mame4all', 'mastersystem', 'megacd',
                             'megadrive', 'msx', 'n64', 'nds', 'neogeo', 'nes',
                             'ngp', 'ngpc', 'oric', 'pc', 'pc88', 'pcfx',
                             'pcengine', 'ports', 'psp', 'psx', 'saturn',
                             'scummvm', 'sega32x', 'segacd', 'sg-1000', 'snes',
                             'ti99', 'trs-80', 'vectrex', 'vic20', 'videopac',
                             'virtualboy', 'wii', 'wonderswan',
                             'wonderswancolor', 'x68000', 'zmachine',
                             'zxspectrum'.)
  -s <module>                Choose scraping module to use while scraping the
                             selected platform.
                             (WEB: 'arcadedb', 'igdb', 'mobygames', 'openretro',
                             'screenscraper', 'thegamesdb' and
                             'worldofspectrum', LOCAL: 'esgamelist', 'import'
                             and 'localdb'. Default is 'localdb')
  -u <key or user:password>  userKey or UserID and Password for use with the
                             selected scraping module.
                             (Default is none)
  -i <path>                  Folder which contains the game/rom files.
                             (default is '~/RetroPie/roms/[platform]')
  --nosubdirs                Do not include input folder subdirectories when
                             scraping.
  --unpack                   Unpacks and checksums the file inside 7z or zip
                             files instead of the compressed file itself. Be
                             aware that this option requires '7z' to be
                             installed on the system to work. Only relevant for
                             'screenscraper' scraping module.
  -g <path>                  Game list export folder.
                             (default depends on frontend)
  -o <path>                  Game media export folder.
                             (default depends on frontend)
  -t <1-8>                   Number of scraper threads to use. This might
                             change depending on the scraping module limits.
                             (default is 4)
  -f <frontend>              Frontend to scrape for.
                             (Currently supports 'emulationstation' and
                             'attractmode'. Default is 'emulationstation')
  -e <emulator>              Set emulator. This is only required by the
                             'attractmode' frontend.
                             (Default is none)
  -m <0-100>                 Minimum match percentage when comparing search
                             result titles to filename titles.
                             (default is 65)
  -l <0-10000>               Maximum game description length. Everything longer
                             than this will be truncated.
                             (default is 2500)
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