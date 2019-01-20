## config.ini options
Look below to find a thorough description of all options and sections available in the `~/.skyscraper/config.ini` configuration file. These options will then be applied whenever running Skyscraper with the `Skyscraper` command.

Please take note that almost all of these options are set at a useful default (and can therefore be left out) and should only be set if your use case requires it.

If you've installed Skyscraper through the RetroPie-Setup the executable is instead located at `/opt/retropie/supplementary/skyscraper/Skyscraper`. In that case I recommend creating a symbolic link to the executable. Do this by running `$ sudo ln -s /opt/retropie/supplementary/skyscraper/Skyscraper /usr/local/bin/Skyscraper` (without the `$`). This will enable you to just type `Skyscraper` when running it from command line.

Many options can be set on two levels; either `[main]` or `[<platform>]`. `<platform>` can be any of the supported platforms (check list with `--help` under the `-p` option), in which case the settings will only be applied while scraping that particular platform. Settings in the `[main]` section will be used while scraping any platform. Setting an option in a `[<platform>]` section will override any similar option set in `[main]`.

For options dedicated to a single scraping module you can create sections for each of them. For instance, you can create a `[screenscraper]` section and add a `userCreds="user:pass"` line. Doing this will always use these credentials when scraping from the `screenscraper` scraping module.

You can find an example config file at `~/.skyscraper/config.ini.example`. This file contains all available options. Just copy the file to `config.ini` and uncomment and edit the ones you wish to use by removing the `#` in front of the variables. Remember to also uncomment the section the option relates to such as `[main]` or `[amiga]`.

NOTE! You can set a custom configuration file with the `-c [filename]` command line option. Read more all available command line options [here](CLIHELP.md).

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
Sets the rom input folder. By default Skyscraper will look for roms in the `/home/<user>/RetroPie/roms/<platform>` folder. If your roms are located in a non-default location, you can set the input path using this option.

NOTE! If this is set in the `[main]` section it will automatically add the chosen `<platform>` to the end of the path. If you want better control consider adding it to a `[<platform>]` section instead where it will be used as is.

#### gamelistFolder="/home/pi/RetroPie/roms"
Sets the game list export folder. By default Skyscraper exports the game list to the same directory as the rom input folder. This enables you to change that to a non-default location.

NOTE! If this is set in the `[main]` section it will automatically add the chosen `<platform>` to the end of the path. If you want better control consider adding it to a `[<platform>]` section instead where it will be used as is.
#mediaFolder="/home/pi/RetroPie/roms"
#cacheFolder="/home/pi/.skyscraper/cache"
#cacheResize="false"
#cacheCovers="true"
#cacheScreenshots="true"
#cacheWheels="true"
#cacheMarquees="true"
#importFolder="/home/pi/.skyscraper/import"
#unpack="false"
#frontend="emulationstation"
#emulator=""
#videos="false"
#symlink="false"
#brackets="true"
#maxLength="10000"
#threads="2"
#pretend="false"
#unattend="false"
#unattendSkip="false"
#interactive="false"
#forceFilename="false"
#verbosity="1"
#skipped="false"
#maxFails="30"
#lang="en"
#region="wor"
#langPrios="en,de,es"
#regionPrios="eu,us,ss,uk,wor,jp"
#artworkXml="artwork.xml"
#relativePaths="false"
#addExtensions="*.zst"
#hints="false"
#subdirs="true"

#[<platform, eg 'amiga'>]
#inputFolder="/home/pi/RetroPie/roms/amiga"
#gamelistFolder="/home/pi/RetroPie/roms/amiga"
#mediaFolder="/home/pi/RetroPie/roms/amiga/media"
#cacheFolder="/home/pi/.skyscraper/cache/amiga"
#cacheResize="false"
#cacheCovers="true"
#cacheScreenshots="true"
#cacheWheels="true"
#cacheMarquees="true"
#importFolder="/home/pi/.skyscraper/import/amiga"
#unpack="false"
#emulator=""
#videos="false"
#symlink="false"
#brackets="true"
#lang="en"
#region="wor"
#langPrios="en,de,es"
#regionPrios="eu,us,ss,uk,wor,jp"
#minMatch="0"
#maxLength="10000"
#threads="2"
#startAt="filename"
#endAt="filename"
#pretend="false"
#unattend="false"
#unattendSkip="false"
#interactive="false"
#forceFilename="false"
#verbosity="1"
#skipped="false"
#artworkXml="artwork.xml"
#relativePaths="false"
#extensions="*.zip *.uae *.adf"
#addExtensions="*.zst *.rom"
#subdirs="true"

#[<scraping module, eg 'screenscaper'>]
#userCreds="user:password"
#threads="1"
#maxLength="10000"
#interactive="false"
