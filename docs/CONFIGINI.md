## config.ini options
Look below to find a thorough description of all options and sections available in the `/home/USER/.skyscraper/config.ini` configuration file. These options will then be applied whenever running Skyscraper with the `Skyscraper` command.

Please take note that almost all of these options are set at a useful default (and can therefore be left out) and should only be set if your use case requires it.

NOTE 1! Some users seem to have a habit of editing the `/home/USER/skysource/config.ini` file. THIS IS NOT THE CORRECT FILE! Please read the above. :)

Options can be set on three levels; either `[main]`, `[<PLATFORM>]`, `[<FRONTEND>]` or `[<MODULE>]`.

Settings in the `[main]` section will always be set regardless of selected platform, frontend and module.

`<PLATFORM>` can be any of the supported platforms (check list with `--help` under the `-p` option), in which case the settings will only be applied while scraping that particular platform.

`<FRONTEND>` can be any of the supported frontends (check list with `--help` under the `-f` option), in which case the settings will only be applied while scraping for that particular frontend.

`<MODULE>` can be any of the supported scraping modules (check list with `--help` under the `-s` option), in which case the settings will only be applied while scraping with that particular scraping module.

Each section can have overlapping parameters. In case where a certain option exists in several sections they are prioritized as scraping module first, then frontend, then platform and lastly main.

You can find an example config file at `/home/USER/.skyscraper/config.ini.example`. This file contains all available options. Just copy the file to `config.ini` and uncomment and edit the ones you wish to use by removing the `#` in front of the variables. Remember to also uncomment the section the option relates to such as `[main]` or `[amiga]`.

NOTE! You can set a custom configuration file with the `-c <FILENAME>` command line option. Read more about all available command line options [here](CLIHELP.md).

###### Example config.ini file
```
[main]
inputFolder="/path/to/your/roms"
cacheFolder="/path/to/your/desired/db/cache/base/folder"
gameListBackup="true"
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

[pegasus]
artworkXml="artwork-pegasus.xml"
maxLength="10000"

[screenscraper]
userCreds="USER:PASSWORD"

```

#### inputFolder="/home/pi/RetroPie/roms"
Sets the rom input folder. By default Skyscraper will look for roms in the `/home/<USER>/RetroPie/roms/<PLATFORM>` folder. If your roms are located in a non-default location, you can set the input path using this option.

NOTE! If this is set in the `[main]` section it will automatically add `/<PLATFORM>` to the end of the path. If you want better control consider adding it to a `[<PLATFORM>]` section instead where it will be used as is.

###### Allowed in sections
`[main]`, `[<PLATFORM>]`

#### gameListFolder="/home/pi/RetroPie/roms"
Sets the game list export folder. By default Skyscraper exports the game list to the same directory as the rom input folder. This enables you to change that to a non-default location.

NOTE! If this is set in the `[main]` section it will automatically add `/<PLATFORM>` to the end of the path. If you want better control consider adding it to a `[<PLATFORM>]` section instead where it will be used as is.

###### Allowed in sections
`[main]`, `[<PLATFORM>]`, `[<FRONTEND>]`

#### gameListBackup="true"
When set to true Skyscraper will create a backup of the existing game list each time it is run in game list generation mode (this mode is enabled by leaving out the `-s MODULE` option). The filename and path of the backup will be equal to the existing game list but have a timestamp appended to the name of the format `-yyyyMMdd-hhmmss` (eg. `gamelist.xml-20200530-115900`).

###### Allowed in sections
`[main]`, `[<FRONTEND>]`

#### mediaFolder="/home/pi/RetroPie/roms"
Sets the artwork / media output folder. By default Skyscraper outputs the composited artwork files to the game list export folder + `/media`. This allows you to change that to a non-default location.

Read more about the artwork compositing [here](ARTWORK.md).

NOTE! If this is set in the `[main]` section it will automatically add `/<PLATFORM>` to the end of the path. If you want better control consider adding it to a `[<PLAFORM>]` section instead where it will be used as is.

###### Allowed in sections
`[main]`, `[<PLATFORM>]`, `[<FRONTEND>]`

#### mediaFolderHidden="false"
By default Skyscraper uses the `PLATFORM/media` folder when generating EmulationStation media. Setting this option will change that to `PLATFORM/.media`. Notice the `.` which hides the folder, which can speed up the initial EmulationStation loading sequence when using slow storage such as network file systems.

NOTE 1! This option is ignored if you set the media folder manually.

NOTE 2! Remember to remove your old `PLATFORM/media` folders if you enable this option.

###### Allowed in sections
`[emulationstation]`

#### cacheFolder="/home/pi/.skyscraper/cache"
Sets a non-default location for the storing and loading of cached game resources. This is what is referred to in the docs as the *resource cache*. By default this folder is set to `/home/USER/.skyscraper/cache/<PLATFORM>`. Don't change this unless you have a good reason to (for instance if you want your cache to reside on a USB drive).

NOTE! If this is set in the `[main]` section (recommended) it will automatically add `/<PLATFORM>` to the end of the path. If you want better control consider adding it to a `[<PLAFORM>]` section instead where it will be used as is.

###### Allowed in sections
`[main]`, `[<PLATFORM>]`

#### cacheResize="false"
By default, to save space, Skyscraper resizes large pieces of artwork before adding them to the resource cache. Setting this option to `"false"` will disable this and save the artwork files with their original resolution. Beware that Skyscraper converts all artwork resources to lossless PNG's when saving them. High resolution images such as covers will take up a lot of space! So set this to `"false"` with caution.

NOTE! This is not related to the artwork compositing that happens when generating a game list. This is *only* related to how Skyscraper handles artwork when adding it to the resource cache while gathering data from the scraping modules.

To read about artwork compositing go [here](ARTWORK.md) instead.

###### Allowed in sections
`[main]`, `[<PLATFORM>]`, `[<SCRAPING MODULE>]`

#### cacheRefresh="false"
Skyscraper has a resource cache which works just like the browser cache in Firefox. If you scrape and gather resources for a platform with the same scraping module twice, it will grab the data from the cache instead of hammering the online servers again. This has the advantage in the case where you scrape a rom set twice, only the roms that weren't recognized the first time around will be fetched from the online servers. Everything else will be loaded from the cache.

You can force all data to be refetched from the servers by setting this option to `cacheRefresh="true"`, effectively updating the cached data with new data from the source.

NOTE! *Only* set this option to true if you know data has changed for several roms at the source. Otherwise you are hammering the servers for no reason.

###### Allowed in sections
`[main]`, `[<SCRAPING MODULE>]`

#### nameTemplate="%t"
By default Skyscraper uses just the title as the game name when generating gamelists for any frontend. You can change this to suit your personal preference by setting this option. It works by replacing certain tokens in the name template with the corresponding data. The valid tokens are:

* `%t`: The game title as returned by the scraping sources without bracket information (see `%b` and `%B` below)
* `%f`: The game filename without extension and bracket information (see `%b` and `%B` below)
* `%b`: The game `()` bracket information. This information often comes from the filename, but can also come from the scraping source title (eg. `(USA)` or `(en,fr,de)`)
* `%B`: The game `[]` bracket information. This information often comes from the filename, but can also come from the scraping source title (eg. `[disk 1 of 2]` or `[AGA]`)
* `%a`: The age restriction as returned by the scraping sources (eg. `16+`)
* `%d`: The game developer as returned by the scraping sources
* `%p`: The game publisher as returned by the scraping sources
* `%r`: The game rating as returned by the scraping sources (`0.0` to `5.0`)
* `%P`: The number of players as returned by the scraping sources
* `%D`: The game release date with format `yyyy-mm-dd`

This option also support template groups separated by `;` within the template. The template parser will go over each group. If a group only has empty variables it will not be included in the final game name. So for a template such as `%t;, %P player(s)` where the `%P` is empty because no scraping source has provided the info, it will leave out the `, %P player(s)` part entirely resulting in the title `Game Name`. If this was not separated by `;` the resulting game name would end up being `Game Name, player(s)`.

###### Example(s)
```
nameTemplate="%t [%f];, %P player(s)"
```
Will result in: `1945k III [1945kiii], 2 player(s)`

###### Allowed in sections
`[main]`, `[<PLATFORM>]`

#### jpgQuality="95"
Sets the default jpg quality when saving image resources to the cache. This will be ignored if `--flags noresize` is set. Default is 95.

NOTE! All screenshots and any image resource that uses transparency will always be saved as png images. In those cases this setting will be ignored as png's are lossless.

###### Allowed in sections
`[main]`, `[<PLATFORM>]`, `[<SCRAPING MODULE>]`

#### cacheCovers="true"
Enables/disables the caching of the resource type `cover` when scraping with any module. If you never use covers in your artwork configuration, setting this to `"false"` can save you some space.

###### Allowed in sections
`[main]`, `[<PLATFORM>]`, `[<SCRAPING MODULE>]`

#### cacheScreenshots="true"
Enables/disables the caching of the resource type `screenshot` when scraping with any module. If you never use screenshots in your artwork configuration, setting this to `"false"` can save you some space.

###### Allowed in sections
`[main]`, `[<PLATFORM>]`, `[<SCRAPING MODULE>]`

#### cropBlack="true"
Enables/disables cropping away the black borders around the screenshot resources when compositing the final frontend gamelist artwork.

###### Allowed in sections
`[main]`, `[<PLATFORM>]`, `[<FRONTEND>]`

#### cacheWheels="true"
Enables/disables the caching of the resource type `wheel` when scraping with any module. If you never use wheels in your artwork configuration, setting this to `"false"` can save you some space.

###### Allowed in sections
`[main]`, `[<PLATFORM>]`, `[<SCRAPING MODULE>]`

#### cacheMarquees="true"
Enables/disables the caching of the resource type `marquee` when scraping with any module. If you never use marquees in your artwork configuration, setting this to `"false"` can save you some space.

###### Allowed in sections
`[main]`, `[<PLATFORM>]`, `[<SCRAPING MODULE>]`

#### importFolder="/home/pi/.skyscraper/import"
Sets a non-default folder when scraping using the `-s import` module. By default this is set to `/home/USER/.skyscraper/import` and will also look for a `/<PLATFORM>` inside of the chosen folder.

Read more about the `-s import` module [here](IMPORT.md).

###### Allowed in sections
`[main]`, `[<PLATFORM>]`

#### unpack="false"
Some scraping modules use file checksums to identify the game in their databases. If you've compressed your roms to zip or 7z files yourself, this can pose a problem in getting a good result. You can then try setting this option to `"true"`. Doing so will extract the rom and do the file checksum on the rom itself instead of the compressed file.

NOTE! Only enable this option if you are having problems getting the roms identified from the compressed files. It slows down the scraping process significantly and should therefore be avoided if possible.

###### Allowed in sections
`[main]`, `[<PLATFORM>]`

#### frontend="emulationstation"
Sets the frontend you wish to export a game list for. By default Skyscraper will export an EmulationStation game list, but other frontends are supported as well.

If exporting for the `attractmode` frontend, please also take note of the required `emulator=""` option that goes along with using the `attractmode` frontend.

If exporting for the `pegasus` frontend, please also take note of the optional `launch=""` option that allows you to set the launch parameter used by Pegasus.

Check all supported frontends with '--help' and read a more about the details concerning each of them [here](FRONTENDS.md).

###### Allowed in sections
`[main]`

#### emulator=""
NOTE! This option is *only* applicable when also setting the `frontend="attractmode"` option. It sets the *emulator* to be used when generating the `attractmode` game list. On RetroPie the emulator name is mostly the same as the platform.

###### Allowed in sections
`[main]`, `[<PLATFORM>]`, `[<FRONTEND>]`

#### launch=""
NOTE! This option is *only* applicable when also setting the `frontend="pegasus"` option. It sets the *launch command* to be used when generating the `pegasus` game list. This is optional. It defaults to the RetroPie launch options which works on RetroPie.

###### Allowed in sections
`[main]`, `[<PLATFORM>]`, `[<FRONTEND>]`

#### videos="false"
By default Skyscraper doesn't scrape and cache video resources because of the significant disk space required to save them. You can enable videos using this option.

###### Allowed in sections
`[main]`, `[<PLATFORM>]`, `[<FRONTEND>]`, `[<SCRAPING MODULE>]`

#### videoSizeLimit="42"
If video scraping is enabled you can set the maximum allowed video file size with this variable. The size is in Megabytes. If this size is exceeded the video file won't be saved to the cache.

###### Allowed in sections
`[main]`, `[<PLATFORM>]`, `[<MODULE>]`, `[<SCRAPING MODULE>]`

#### videoConvertCommand="convertvideo.sh %i %o"
Some scraping modules deliver videos that use a codec or color format that some frontends don't support. In those cases it can be useful to convert the videos before saving them in the Skyscraper resource cache.

This setting allows you to set a command that will be run on each video after it has been downloaded from the selected scraping module. See the examples below for inspiration.

The `%i` and `%o` **are required** and will be replaced with the internally used video input (original) and output (converted) filenames as needed by Skyscraper.

If your command / script always converts to a videofile with a specific extension, you also need to set `videoConvertExtension`.

NOTE 1! Set `--verbosity 3` to route all output from your command / script to the terminal while Skyscraper runs. This will help you ensure everything is working as intended.

NOTE 2! The first example below makes use of the excellent `ffmpeg` tool. If you want to use this specific example you need to install `ffmpeg` first. On RetroPie and other Debian-derived distros you can install it with `sudo apt install ffmpeg`.

NOTE 3! If you want to use a script for the video conversion and run it directly without path, you need to place it in the `/home/USER/.skyscraper` folder.

NOTE 4! If you create a script that includes checks on the input video and it sometimes decides not to convert them, you should simply let the script copy the video with `cp %i %o`. The `%i` is a temporary file created internally by Skyscraper. And the `%o` file is the file used by the cache. Both files should exist after a successful conversion. Skyscraper removes the temporary `%i` file automatically.

###### Example(s)
```
videoConvertCommand="ffmpeg -i %i -y -pix_fmt yuv420p -t 00:00:10 -c:v libx264 -crf 23 -c:a aac -b:a 64k -vf scale=640:480:force_original_aspect_ratio=decrease,pad=640:480:(ow-iw)/2:(oh-ih)/2,setsar=1 %o"
videoConvertCommand="videoconvert.sh %i %o"
```

###### Allowed in sections
`[main]`, `[<SCRAPING MODULE>]`

#### videoConvertExtension="mp4"
If you want to force an extension for the converted video file created by the `videoConvertCommand` command, you need to set this option. The converted file will then automatically have this extension no matter what the input file extension is.

For instance, if a scraping module delivers the file `videofile.avi` and you always want the converted files to be `mp4` files, you simply set this option to `videoConvertExtension="mp4"`.

NOTE! It is up to you to make sure that the command you provide in `videoConvertCommand` actually converts to a video file using the selected extension. For some conversion tools such as `ffmpeg` this is handled simply by setting the extension of the output file. But for other tools you might have to provide additional options to the `videoConvertCommand` above.

###### Allowed in sections
`[main]`, `[<SCRAPING MODULE>]`

#### videoPreferNormalized="true"
This option is *only* applicable when scraping with the `-s screenscraper` module. ScreenScraper offers two versions of some of their videos. A normalized version, which adheres to some defined standard they made, and the originals. If you prefer converting or standardizing the videos yourself (see `videoConvertCommand` above) then you can set this to `false`. If you do so Skyscraper will fetch the original videos from ScreenScraper instead of the normalized ones.

NOTE! Be aware that the original videos often vary a lot in codec, color format and size. So it is recommended to convert them afterwards using the `videoConvertCommand`.

###### Example(s)
```
[screenscraper]
videoPreferNormalized="false"
```

###### Allowed in sections
`[screenscraper]`

#### symlink="false"
Enabling this option is only relevant while also setting the `videos="true"` option. It basically means that Skyscraper will create a link to the cached videos instead of copying them when generating the game list media files. This will save a lot of space, but has the caveat that if you somehow remove the videos from the cache, the links will be broken and the videos then won't show anymore.

###### Allowed in sections
`[main]`, `[<PLATFORM>]`, `[<FRONTEND>]`

#### theInFront="false"
Game titles are returned from the scraping sources sometimes as 'The Game' and other times as 'Game, The'. Enabling this option will force Skyscraper to always try and move 'The' to the front of the titles. If it is not enabled, Skyscraper will always try and move it to the end of the title, regardless of how it was originally returned by the scraping sources.

NOTE! When generating gamelists Skyscraper will still sort the games as if the game titles didn't have 'The' at the beginning.

###### Allowed in sections
`[main]`, `[<PLATFORM>]`, `[<FRONTEND>]`

#### brackets="true"
Disable this option to remove any bracket notes when generating the game list. It will disable notes such as `(Europe)` and `[AGA]` completely. This option is only relevant when generating the game list. It makes no difference when gathering data into the resource cache.

###### Allowed in sections
`[main]`, `[<PLATFORM>]`, `[<FRONTEND>]`

#### maxLength="10000"
Sets the maximum length of returned game descriptions. This is a convenience option if you feel like game descriptions are too long. By default it is set to 2500.

###### Allowed in sections
`[main]`, `[<PLATFORM>]`, `[<FRONTEND>]`, `[<SCRAPING MODULE>]`

#### minMatch="65"
Some scraping modules are based on a file name or title based search. This option sets the minimum percentage any returned results need to match with in order for it to be accepted. For instance, the game `Wonderboy in Monsterland` might return the title `Wonder Boy in Monster Land` which is clearly a match. But it's not a 100% match. So it needs to be set relatively high, while still ignoring bad matches. By default it is set to 65 which has been tested to be a good middle-ground.

###### Allowed in sections
`[main]`, `[<PLATFORM>]`, `[<SCRAPING MODULE>]`

#### threads="2"
Sets the desired number of parallel threads to be run when scraping. NOTE! Some modules have maximum allowed threads. If you set this higher than the allowed value, it will be auto-adjusted. By default it is set to 4.

###### Allowed in sections
`[main]`, `[<PLATFORM>]`, `[<SCRAPING MODULE>]`

#### pretend="false"
This option is *only* relevant when generating a game list (by leaving out the `-s <MODULE>` command line option). It disables the game list generator and artwork compositor and only outputs the results of the potential game list generation to the terminal. It is mostly useful when used as a command line flag with `--flags pretend`. It makes little sense to set it here, but you can if you want to.

###### Allowed in sections
`[main]`, `[<PLATFORM>]`

#### unattend="true"
When generating a game list Skyscraper will check if it already exists and ask if you want to overwrite it. And it will also ask if you wish to skip existing game list entries. By enabling this option Skyscraper will *always* overwrite an existing game list and *never* skip existing entries. This is useful when scripting Skyscraper to avoid the need for user input.

###### Allowed in sections
`[main]`, `[<PLATFORM>]`, `[<FRONTEND>]`

#### unattendSkip="false"
When generating a game list Skyscraper will check if it already exists and ask if you want to overwrite it. And it will also ask if you wish to skip existing game list entries. By enabling this option Skyscraper will *always* overwrite an existing game list and *always* skip existing entries. This is useful when scripting Skyscraper to avoid the need for user input.

###### Allowed in sections
`[main]`, `[<PLATFORM>]`, `[<FRONTEND>]`

#### interactive="false"
When gathering data from any of the scraping modules many potential entries will be returned. Normally Skyscraper chooses the best entry for you. But should you wish to choose the best entry yourself, you can enable this option. Skyscraper will then list the returned entries and let you choose which one is the best one. It is recommended to use the command line flag `--flags interactive` instead in the (hopefully) rare cases where this mode is necessary.

###### Allowed in sections
`[main]`, `[<PLATFORM>]`, `[<SCRAPING MODULE>]`

#### forceFilename="false"
Enable this option to force Skyscraper to use the file name (excluding extension) instead of the cached scraping module titles when generating a game list.

NOTE! If you set `forceFilename="true"` and your filenames contain bracket notes such as `(this)` or `[that]` at the end, these will be combined with whatever bracket notes are at the end of the titles returned from the sources. This can cause some confusion. For instance, if you have the filename `Gran Turismo 2 (USA) (Arcade Mode)` and the cached title is `Gran Turismo 2 (Arcade Mode)`, then the gamelist name will become `Gran Turismo 2 (Arcade Mode)(USA)(Arcade Mode)`. You can disable them altogether with the `brackets="no"` option.

###### Allowed in sections
`[main]`, `[<PLATFORM>]`, `[<FRONTEND>]`

#### verbosity="1"
Sets how verbose Skyscraper should be when running. Default level is 0. The higher the value, the more info Skyscraper will output to the terminal while running.

###### Allowed in sections
`[main]`, `[<PLATFORM>]`, `[<FRONTEND>]`

#### skipped="false"
If a rom has no resources attached to it in the cache, it will be left out when generating a game list file. It will still show up in the frontend (at least it does for EmulationStation) but it won't exist in the game list file. You can safely leave out / disable this option unless you need the empty entries for some reason.

###### Allowed in sections
`[main]`, `[<PLATFORM>]`, `[<FRONTEND>]`

#### maxFails="30"
Not all scraping modules support all platforms. This means that you can potentially start a scraping run with a module and a platform that is incompatible. This will hammer the servers for potentially hundreds of roms but provide 0 results for any of them. To avoid this Skyscraper has a builtin limit for initially allowed failed rom lookups. If this is reached it will quit. Setting this option allows you to set this limit yourself, but not above a maximum of 200. The default limit is 42. Don't change this unless you have a very good reason to do so.

###### Allowed in sections
`[main]`

#### region="wor"
Adds the specified region to the top of the existing default internal region priority list. Only one region is supported with this configuration. To overwrite the internal default region priority list entirely check [here](CONFIGINI.md#regionprioseuusssukworjp).

NOTE 1! Read more about how regions are handled in general [here](REGIONS.md).

NOTE 2: Setting this will overwrite any region [auto-detected](REGIONS.md#region-auto-detection) from the file name.

###### Allowed in sections
`[main]`, `[<PLATFORM>]`

###### Example(s)
```
[main]
region="de"
```
or
```
[snes]
region="de"
```

#### regionPrios="eu,us,ss,uk,wor,jp"
Completely overwrites the internal region priority list inside of Skyscraper. Multiple regions can be configured here separated by commas.

NOTE 1! Read more about how regions are handled in general [here](REGIONS.md).

NOTE 2: Any region [auto-detected](REGIONS.md#region-auto-detection) from the file name will still be added to the top of this list.

###### Allowed in sections
`[main]`, `[<PLATFORM>]`

###### Example(s)
```
[main]
regionPrios="de,us,jp"
```
or
```
[megadrive]
regionPrios="de,us,jp"
```

#### lang="en"
Adds the specified language to the top of the existing default internal language priority list. Only one language is supported with this configuration.

NOTE! Read more about how languages are handles in general [here](LANGUAGES.md).

###### Allowed in sections
`[main]`, `[<PLATFORM>]`

###### Example(s)
```
[main]
lang="it"
```
or
```
[snes]
lang="it"
```

#### langPrios="en,de,es"
Completely overwrites the internal language priority list inside of Skyscraper. Multiple languages can be configured here separated by commas.

NOTE! Read more about how languages are handles in general [here](LANGUAGES.md).

###### Allowed in sections
`[main]`, `[<PLATFORM>]`

###### Example(s)
```
[main]
langPrios="it,en"
```
or
```
[megadrive]
langPrios="it,en"
```

#### artworkXml="artwork.xml"
Sets a non-default xml file to use when setting up the artwork compositing. By default Skyscraper uses the file `/home/USER/.skyscraper/artwork.xml`. Read more about the artwork.xml format and customization options [here](ARTWORK.md).

NOTE! It can be *very* useful to set this in any platform section or frontend section where you want a specific artwork setup / look.

###### Allowed in sections
`[main]`, `[<PLATFORM>]`, `[<FRONTEND>]`

#### relativePaths="false"
Currently only relevant when generating an EmulationStation game list (which is the default frontend when the `-f` option is left out). Enabling this forces the rom and any media paths inside the game list to be relative to the rom input folder.

###### Allowed in sections
`[main]`, `[<PLATFORM>]`

#### extensions="*.zip *.uae *.adf"
Completely overwrites the rom extensions Skyscraper allows for the chosen platform.

NOTE! If you feel like you are using a file extension that ought to be supported by default, please report it so it can be added in a later version of Skyscraper.

###### Allowed in sections
`[<PLATFORM>]`

#### addExtensions="*.zst *.smt"
Adds the rom extensions to the ones that are already supported by the platform.

NOTE! If you feel like you are using a file extension that ought to be supported by default, please report it so it can be added in a later version of Skyscraper.

###### Allowed in sections
`[main]`, `[<PLATFORM>]`

#### hints="false"
Disables the "Did you know" hints when running Skyscraper.

###### Allowed in sections
`[main]`, `[<PLATFORM>]`

#### subdirs="true"
By default Skyscraper will include roms located in subfolders. By disabling this option Skyscraper will only scrape the roms located directly in the input folder. See `inputFolder="<PATH>"` further up to read more about the rom input folder.

###### Allowed in sections
`[main]`, `[<PLATFORM>]`

#### startAt="filename"
If you only ever wish to gather data for a subset of your roms from the scraping modules you can use this option to set the starting rom. It will then scrape alphabetically from that rom and onwards.

NOTE! Please consider using the command line option `--startat <FILENAME>` instead. It makes little sense to set this permanently in your configuration unless you *always* want it to only ever scrape from a certain file and onward. But you can if you absolutely want to.

###### Allowed in sections
`[<PLATFORM>]`, `[<FRONTEND>]`

#### endAt="filename"
If you only ever wish to gather data for a subset of your roms from the scraping modules you can use this option to set the rom to end at. It will then scrape alphabetically until it reaches this rom, then stop.

NOTE! Please consider using the command line option `--endat <FILENAME>` instead. It makes little sense to set this permanently in your configuration unless you *always* want it to only ever scrape up until it reaches a certain file. But you can if you absolutely want to.

###### Allowed in sections
`[<PLATFORM>]`, `[<FRONTEND>]`

#### includePattern="&lt;PATTERN1,PATTERN2&gt;"
Per platform Skyscraper have default file extensions that it will accept. This option allows you to only include certain files within that scope. The pattern is a simple asterisk type pattern. In cases where you need to match for a comma you need to escape it as `\,`.

NOTE 1! You might also want to check out the file extension options.

NOTE 2! You might also want to check out the 'includeFrom' option.

###### Allowed in sections
`[main]`, `[<PLATFORM>]`, `[<FRONTEND>]`

#### excludePattern="&lt;PATTERN 1,PATTERN 2&gt;"
Per platform Skyscraper have default file extensions that it will accept. This option allows you to exclude certain files within that scope. The pattern is a simple asterisk type pattern. In cases where you need to match for a comma you need to escape it as `\,`.

NOTE 1! You might also want to check out the file extension options.

NOTE 2! You might also want to check out the 'excludeFrom' option.

NOTE 3! If you create a file named `.skyscraperignore` within any subfolder of the input dir, all files from that directory will be ignored by Skyscraper.

###### Allowed in sections
`[main]`, `[<PLATFORM>]`, `[<FRONTEND>]`

#### includeFrom="&lt;FILENAME&gt;"
Tells Skyscraper to only include the files listed in FILENAME. One filename per line (with FULL path, eg. '/home/pi/RetroPie/roms/snes/subdir/somefile.zip').

This file can be generated with the '--cache report:missing' option or made manually.

NOTE! You might also want to check out the 'includePattern' option.

###### Allowed in sections
`[main]`, `[<PLATFORM>]`

#### excludeFrom="&lt;FILENAME&gt;"
Tells Skyscraper to exclude the files listed in FILENAME. One filename per line (with FULL path, eg. '/home/pi/RetroPie/roms/snes/subdir/somefile.zip').

This file can be generated with the '--cache report:missing' option or made manually.

NOTE 1! You might also want to check out the 'excludePattern' option.

NOTE 2! If you create a file named '.skyscraperignore' within any subfolder of the input dir, all files from that directory will be ignored by Skyscraper.

###### Allowed in sections
`[main]`, `[<PLATFORM>]`

#### userCreds="&lt;CREDENTIALS or KEY&gt;"
Some scraping modules require a `key` or `user:password` to work. You can create a `[<SCRAPING MODULE>]` section and add a `userCreds="user:pass"` or `userCreds="key"` line beneath it. Doing this will always use these credentials when scraping from the module in question. Check the scraping module overview to see the specific requirements for each module [here](SCRAPINGMODULES.md).

###### Example(s)
```
[screenscraper]
userCreds="user:password"
```
or
```
[igdb]
userCreds="key"
```

###### Allowed in sections
`[<SCRAPING MODULE>]`

#### spaceCheck="false"
Skyscraper will continuously check if you are running low on disk space. If you go below 200 MB in either the game list export folder or the resource cache folder, it will quit to make sure your system doesn't become unstable. Some types of file systems provide a faulty result to Skyscraper when it comes to these checks and thus it can be necessary to disable it altogether. You can use this option to do just that.

###### Allowed in sections
`[main]`

#### scummIni="/full/path/to/scummvm.ini"
Allows you to set a non-default location of the scummvm.ini file. This file is used whenever scraping the `scummvm` platform. It converts the shortname such as `monkey2` to the more search-friendly name `Monkey Island 2: LeChuck's Revenge` whenever using one of the file name search based scraping modules.

###### Allowed in sections
`[main]`
