## config.ini options
Look below to find a thorough description of all options and sections available in the `~/.skyscraper/config.ini` configuration file. These options will then be applied whenever running Skyscraper with the `Skyscraper` command.

Please take note that almost all of these options are set at a useful default (and can therefore be left out) and should only be set if your use case requires it.

If you've installed Skyscraper through the RetroPie-Setup the executable is instead located at `/opt/retropie/supplementary/skyscraper/Skyscraper`. In that case I recommend creating a symbolic link to the executable. Do this by running `$ sudo ln -s /opt/retropie/supplementary/skyscraper/Skyscraper /usr/local/bin/Skyscraper` (without the `$`). This will allow you to just type `Skyscraper` when running it from command line.

Many options can be set on two levels; either `[main]` or `[<PLATFORM>]`. `<PLATFORM>` can be any of the supported platforms (check list with `--help` under the `-p` option), in which case the settings will only be applied while scraping that particular platform. Settings in the `[main]` section will be used while scraping any platform. Setting an option in a `[<PLATFORM>]` section will override any similar option set in `[main]`.

For options dedicated to a single scraping module you can create sections for each of them. For instance, you can create a `[screenscraper]` section and add a `userCreds="user:pass"` line. Doing this will always use these credentials when scraping from the `screenscraper` scraping module.

You can find an example config file at `~/.skyscraper/config.ini.example`. This file contains all available options. Just copy the file to `config.ini` and uncomment and edit the ones you wish to use by removing the `#` in front of the variables. Remember to also uncomment the section the option relates to such as `[main]` or `[amiga]`.

NOTE! You can set a custom configuration file with the `-c <FILENAME>` command line option. Read more all available command line options [here](CLIHELP.md).

###### Example config.ini file
```
[main]
inputFolder="/path/to/your/roms"
cacheFolder="/path/to/your/desired/db/cache/base/folder"
videos="true"
unattend="true"
verbosity="1"
lang="ja"
region="jp"
cacheMarquees="false"

[amiga]
inputFolder="/specific/path/just/for/amiga/roms"
cacheFolder="/specific/path/just/for/amiga/cache"
minMatch="50"
maxLength="200"
artworkXml="artwork-amiga.xml"

[screenscraper]
userCreds="muldjord:666fff"
maxLength="10000"
```
#### inputFolder="/home/pi/RetroPie/roms"
Sets the rom input folder. By default Skyscraper will look for roms in the `/home/<USER>/RetroPie/roms/<PLATFORM>` folder. If your roms are located in a non-default location, you can set the input path using this option.

NOTE! If this is set in the `[main]` section it will automatically add `/<PLATFORM>` to the end of the path. If you want better control consider adding it to a `[<PLATFORM>]` section instead where it will be used as is.

*Allowed in section(s): `[main]`, `[<PLATFORM>]`*

#### gamelistFolder="/home/pi/RetroPie/roms"
Sets the game list export folder. By default Skyscraper exports the game list to the same directory as the rom input folder. This enables you to change that to a non-default location.

NOTE! If this is set in the `[main]` section it will automatically add `/<PLATFORM>` to the end of the path. If you want better control consider adding it to a `[<PLATFORM>]` section instead where it will be used as is.

*Allowed in section(s): `[main]`, `[<PLATFORM>]`*

#### mediaFolder="/home/pi/RetroPie/roms"
Sets the artwork / media output folder. By default Skyscraper outputs the composited artwork files to the game list export folder + `/media`. This allows you to change that to a non-default location.

Read more about the artwork compositing [here](ARTWORK.md).

NOTE! If this is set in the `[main]` section it will automatically add `/<PLATFORM>` to the end of the path. If you want better control consider adding it to a `[<PLAFORM>]` section instead where it will be used as is.

*Allowed in section(s): `[main]`, `[<PLATFORM>]`*

#### cacheFolder="/home/pi/.skyscraper/cache"
Sets a non-default location for the storing and loading of cached game resources. This is what is referred to in the docs as the *resource cache*. By default this folder is set to `~/.skyscraper/cache/<PLATFORM>`. Don't change this unless you have a good reason to (for instance if you want your cache to reside on a USB drive).

NOTE! If this is set in the `[main]` section (recommended) it will automatically add `/<PLATFORM>` to the end of the path. If you want better control consider adding it to a `[<PLAFORM>]` section instead where it will be used as is.

*Allowed in section(s): `[main]`, `[<PLATFORM>]`*

#### cacheResize="false"
By default, to save space, Skyscraper resizes large pieces of artwork before adding them to the resource cache. Setting this option to `"false"` (not recommended unless you really need that extra resolution for the raw images) will disable this and save the artwork files exactly as they are retrieved from the scraping module.

NOTE! This is not related to the artwork compositing that happens when generating a game list. This is *only* related to how Skyscraper handles artwork when adding it to the resource cache while gathering data from the scraping modules.

To read about artwork compositing go [here](ARTWORK.md) instead.

*Allowed in section(s): `[main]`, `[<PLATFORM>]`*

#### cacheCovers="true"
Enables/disables the caching of the resource type `cover` when scraping with any module. If you never use covers in your artwork configuration, setting this to `"false"` can save you some space.

*Allowed in section(s): `[main]`, `[<PLATFORM>]`*

#### cacheScreenshots="true"
Enables/disables the caching of the resource type `screenshot` when scraping with any module. If you never use covers in your artwork configuration, setting this to `"false"` can save you some space.

*Allowed in section(s): `[main]`, `[<PLATFORM>]`*

#### cacheWheels="true"
Enables/disables the caching of the resource type `wheel` when scraping with any module. If you never use covers in your artwork configuration, setting this to `"false"` can save you some space.

*Allowed in section(s): `[main]`, `[<PLATFORM>]`*

#### cacheMarquees="true"
Enables/disables the caching of the resource type `marquee` when scraping with any module. If you never use covers in your artwork configuration, setting this to `"false"` can save you some space.

*Allowed in section(s): `[main]`, `[<PLATFORM>]`*

#### importFolder="/home/pi/.skyscraper/import"
Sets a non-default folder when scraping using the `-s import` module. By default this is set to `~/.skyscraper/import` and will also look for a `/<PLATFORM>` inside of the chosen folder.

Read more about the `-s import` module [here](IMPORT.md).

*Allowed in section(s): `[main]`, `[<PLATFORM>]`*

#### unpack="false"
Some scraping modules use file checksums to identify the game in their databases. If you've compressed your roms to zip or 7z files yourself, this can pose a problem in getting a good result. You can then try setting this option to `"true"`. Doing so will extract the rom and do the file checksum on the rom itself instead of the compressed file.

NOTE! Only enable this option if you are having problems getting the roms identified from the compressed files. It slows down the scraping process significantly and should therefore be avoided if possible.

*Allowed in section(s): `[main]`, `[<PLATFORM>]`*

#### frontend="emulationstation"
Sets the frontend you wish to export a game list for. By default Skyscraper will export an EmulationStation game list, but other frontends are supported as well. If exporting for the `attractmode` frontend, please also take note of the required `emulator=""` option that goes along with using the `attractmode` frontend.

*Allowed in section(s): `[main]`*

#### emulator=""
NOTE! This option is *only* applicable when also setting the `frontend="attractmode"` option. It sets the *emulator* to be used when generating the `attractmode` game list. On RetroPie the emulator name is mostly the same as the platform.

*Allowed in section(s): `[main]`*

#### videos="false"
By default Skyscraper doesn't scrape and cache video resources because of the significant disk space required to save them. You can enable videos using this option.

*Allowed in section(s): `[main]`, `[<PLATFORM>]`*

#### symlink="false"
Enabling this option is only relevant while also setting the `videos="true"` option. It basically means that Skyscraper will create a link to the cached videos instead of copying them when generating the game list media files. This will save a lot of space, but has the caveat that if you somehow remove the videos from the cache, the links will be broken and the videos then won't show anymore.

*Allowed in section(s): `[main]`, `[<PLATFORM>]`*

#### brackets="true"
Disable this option to remove any bracket notes when generating the game list. It will disable notes such as `(Europe)` and `[AGA]` completely. This option is only relevant when generating the game list. It makes no difference when gathering data into the resource cache.

*Allowed in section(s): `[main]`, `[<PLATFORM>]`*

#### maxLength="10000"
Sets the maximum length of returned game descriptions. This is a convenience option if you feel like game descriptions are too long. By default it is set to 2500.

*Allowed in section(s): `[main]`, `[<PLATFORM>]`, `[<SCRAPING MODULE>]`*

#### minMatch="65"
Some scraping modules are based on a file name or title based search. This option sets the minimum percentage any returned results need to match with in order for it to be accepted. For instance, the game `Wonderboy in Monsterland` might return the title `Wonder Boy in Monster Land` which is clearly a match. But it's not a 100% match. So it needs to be set relatively high, while still ignoring bad matches. By default it is set to 65 which has been tested to be a good middle-ground.

*Allowed in section(s): `[main]`, `[<PLATFORM>]`, `[<SCRAPING MODULE>]`*

#### threads="2"
Sets the desired number of parallel threads to be run when scraping. NOTE! Some modules have maximum allowed threads. If you set this higher than the allowed value, it will be auto-adjusted. By default it is set to 4.

*Allowed in section(s): `[main]`, `[<PLATFORM>]`, `[<SCRAPING MODULE>]`*

#### pretend="false"
This option is *only* relevant when generating a game list (by leaving out the `-s <MODULE>` command line option). It disables the game list generator and artwork compositor and only outputs the results of the potential game list generation to the terminal. It is mostly useful when used as a command line option with `--pretend`. It makes little sense to set it here, but you can if you want to.

*Allowed in section(s): `[main]`, `[<PLATFORM>]`*

#### unattend="true"
When generating a game list Skyscraper will check if it already exists and ask if you want to overwrite it. And it will also ask if you wish to skip existing game list entries. By enabling this option Skyscraper will *always* overwrite an existing game list and *never* skip existing entries. This is useful when scripting Skyscraper to avoid the need for user input.

*Allowed in section(s): `[main]`, `[<PLATFORM>]`*

#### unattendSkip="false"
When generating a game list Skyscraper will check if it already exists and ask if you want to overwrite it. And it will also ask if you wish to skip existing game list entries. By enabling this option Skyscraper will *always* overwrite an existing game list and *always* skip existing entries. This is useful when scripting Skyscraper to avoid the need for user input.

*Allowed in section(s): `[main]`, `[<PLATFORM>]`*

#### interactive="false"
When gathering data from any of the scraping modules many potential entries will be returned. Normally Skyscraper chooses the best entry for you. But should you wish to choose the best entry yourself, you can enable this option. Skyscraper will then list the returned entries and let you choose which one is the best one. It is recommended to use the command line option `--interactive` instead in the (hopefully) rare cases where this mode is necessary.

*Allowed in section(s): `[main]`, `[<PLATFORM>]`, `[<SCRAPING MODULE>]`*

#### forceFilename="false"
Enable this option to force Skyscraper to use the file name (excluding extension) instead of the cached scraping module titles when generating a game list.

*Allowed in section(s): `[main]`, `[<PLATFORM>]`*

#### verbosity="1"
Sets how verbose Skyscraper should be when running. Default level is 0. The higher the value, the more info Skyscraper will output to the terminal while running.

*Allowed in section(s): `[main]`, `[<PLATFORM>]`*

#### skipped="false"
If a rom has no resources attached to it in the cache, it will be left out when generating a game list file. It will still show up in the frontend (at least it does for EmulationStation) but it won't exist in the game list file. You can safely leave out / disable this option unless you need the empty entries for some reason.

*Allowed in section(s): `[main]`, `[<PLATFORM>]`*

#### maxFails="30"
Not all scraping modules support all platforms. This means that you can potentially start a scraping run with a module and a platform that is incompatible. This will hammer the servers for potentially hundreds of roms but provide 0 results for any of them. To avoid this Skyscraper has a builtin limit for initially allowed failed rom lookups. If this is reached it will quit. Setting this option allows you to set this limit yourself, but not above a maximum of 200. The default limit is 42. Don't change this unless you have a very good reason to do so.

*Allowed in section(s): `[main]`*

#### lang="en"
Sets the desired language when gathering data into the resource cache. This option is only relevant for certain scraping modules. Get the details [here](LANGUAGES.md).

*Allowed in section(s): `[main]`, `[<PLATFORM>]`*

#### region="wor"
Sets the desired region when gathering data into the resource cache. This option is only relevant for certain scraping modules. Get the details [here](REGIONS.md).

*Allowed in section(s): `[main]`, `[<PLATFORM>]`*

#### langPrios="en,de,es"
Completely overrides the internal language priority list inside of Skyscraper. This list is only relevant for certain scraping modules. Get the details [here](LANGUAGES.md).

*Allowed in section(s): `[main]`, `[<PLATFORM>]`*

#### regionPrios="eu,us,ss,uk,wor,jp"
Completely overrides the internal region priority list inside of Skyscraper. This list is only relevant for certain scraping modules. Get the details [here](REGIONS.md).

*Allowed in section(s): `[main]`, `[<PLATFORM>]`*

#### artworkXml="artwork.xml"
Sets a non-default xml file to use when setting up the artwork compositing. By default Skyscraper uses the file `~/.skyscraper/artwork.xml`. Read more about the artwork.xml format and customization options [here](ARTWORK.md).

NOTE! It can be *very* useful to set this in any platform section where you want to a specific artwork setup / look. You can then have separate artwork xml files for each of them which will be used when compositing the artwork during game list generation.

*Allowed in section(s): `[main]`, `[<PLATFORM>]`*

#### relativePaths="false"
Currently only relevant when generating an EmulationStation game list (which is the default frontend when the `-f` option is left out). Enabling this forces the rom and any media paths inside the game list to be relative to the rom input folder.

*Allowed in section(s): `[main]`, `[<PLATFORM>]`*

#### extensions="*.zip *.uae *.adf"
Completely overwrites the rom extensions Skyscraper allows for the chosen platform.

NOTE! If you feel like you are using a file extension that ought to be supported by default, please report it so it can be added in a later version of Skyscraper.

*Allowed in section(s): `[<PLATFORM>]`*

#### addExtensions="*.zst *.smt"
Adds the rom extensions to the ones that are already supported by the platform.

NOTE! If you feel like you are using a file extension that ought to be supported by default, please report it so it can be added in a later version of Skyscraper.

*Allowed in section(s): `[main]`, `[<PLATFORM>]`*

#### hints="false"
Disables the "Did you know" hints when running Skyscraper.

*Allowed in section(s): `[main]`, `[<PLATFORM>]`*

#### subdirs="true"
By default Skyscraper will include roms located in subfolders. By disabling this option Skyscraper will only scrape the roms located directly in the input folder. See `inputFolder="<PATH>"` further up to read more about the rom input folder.

*Allowed in section(s): `[main]`, `[<PLATFORM>]`*

#### startAt="filename"
If you only ever wish to gather data for a subset of your roms from the scraping modules you can use this option to set the starting rom. It will then scrape alphabetically from that rom and onwards.

NOTE! Please consider using the command line option `--startat <FILENAME>` instead. It makes little sense to set this permanently in your configuration unless you *always* want it to only ever scrape from a certain file and onward. But you can if you absolutely want to.

*Allowed in section(s): `[<PLATFORM>]`*

#### endAt="filename"
If you only ever wish to gather data for a subset of your roms from the scraping modules you can use this option to set the rom to end at. It will then scrape alphabetically until it reaches this rom, then stop.

NOTE! Please consider using the command line option `--endat <FILENAME>` instead. It makes little sense to set this permanently in your configuration unless you *always* want it to only ever scrape up until it reaches a certain file. But you can if you absolutely want to.

*Allowed in section(s): `[<PLATFORM>]`*

#### userCreds="user:password"
Some scraping modules require a user key or a user id + password to work. Check the scraping module overview to see the specific requirements for each module [here](SCRAPINGMODULES.md).

*Allowed in section(s): `[<SCRAPING MODULE>]`*

#### spaceCheck="false"
Skyscraper will continuously check if you are running low on disk space. If you go below 200 MB in either the game list export folder or the resource cache folder, it will quit to make sure your system doesn't become unstable. Some types of file systems provide a faulty result to Skyscraper when it comes to these checks and thus it can be necessary to disable it altogether. You can use this option to do just that.

*Allowed in section(s): `[main]`*

#### memCheck="false"
Skyscraper will continuously check if you are running low on memory. If you go below 50 MB available memory, it will exit nicely to make sure any cached data up until that point isn't lost. You might have reasons to disable this check. You can use this option to do just that.

*Allowed in section(s): `[main]`*
