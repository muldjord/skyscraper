## Frontend overview
When generating a game list with Skyscraper you have the option of generating it for several different frontends. A frontend is the graphical interface that lists and launches your games.

Setting a frontend when generating a game list is done by setting the `-f <FRONTEND>` command-line parameter as explained [here](CLIHELP.md#-f-frontend) or by setting it in `/home/USER/.skyscraper/config.ini` as explained [here](CONFIGINI.md#frontendemulationstation). Some frontends have further options that are either optional or required. Check the frontend sections below for more information on this.

WARNING! Skyscraper will overwrite your game list (obviously). So if you have spend a lot of time hand-crafting metadata in a game list for any frontend, please remember to create a backup before overwriting it with Skyscraper. You can also tell Skyscraper to auto-backup old game lists prior to overwriting them. Read more about that [here](CONFIGINI.md#gamelistbackuptrue).

NOTE! When generating a game list for any frontend, Skyscraper will try to preserve certain metadata. Check the frontend sections below for more information on what metadata is preserved per frontend.

#### EmulationStation
* Default game list location: `/home/pi/USER/RetroPie/roms/PLATFORM`
* Default game list filename: `gamelist.xml`

This is the default frontend used when generating a game list with Skyscraper. If no frontend is defined on command-line or in `config.ini` it will fall back to generating for EmulationStation.

##### Metadata preservation
Skyscraper will preserve the following metadata when re-generating a game list for EmulationStation: `favorite`, `hidden`, `playcount`, `lastplayed`, `kidgame`, `sortname`.

#### Attract-Mode
* Default game list location: `/home/pi/USER/.attract/romlists`
* Default game list filename: `<EMULATOR/PLATFORM>.txt`

Attract-Mode is a bit more abstract when it comes to how it saves its game lists and media. To export for Attract-Mode you need to, in addition to setting the frontend, set `-e <EMULATOR>` on command-line or in `config.ini`. The `<EMULATOR>` is a file that describes the platform / emulator you are generating a game list for. The file contains everything needed to tell Attract-Mode how to launch games for the platform, and even where to find the media files for the games (such as screenshots and videos).

If you are running RetroPie most of the `<EMULATOR>` files will have been auto-generated for you. They are usually named the same as the platform you are generating a game list for. So, if you are generating for `snes` you simply use `Skyscraper -p snes -f attractmode -e snes` and that should work just fine. The `<EMULATOR>` files are usually located at `/home/USER/.attract/emulators/`. Check them out if you are curious.

##### Metadata preservation
Skyscraper will preserve the following metadata when re-generating a game list for Attract-Mode: `cloneof`, `rotation`, `control`, `status`, `displaycount`, `displaytype`, `altromname`, `alttitle`, `extra`, `buttons`.

#### Pegasus
* Default game list location: `/home/pi/USER/RetroPie/roms/PLATFORM`
* Default game list filename: `metadata.pegasus.txt`

Pegasus is easy and simple to generate a game list for. Simply do `Skyscraper -p <PLATFORM> -f pegasus`. If you want to specify a custom launch command (if you are using RetroPie you don't have to, a default one will be used), you can set it on command-line with `-e "<COMMAND>"` or in `config.ini` with:
```
[pegasus]
launch="<COMMAND>"
```

NOTE! You need to add the individual platform rom directories to Pegasus (if they are not already defined) before any of them will show up! Add them through the `Settings->Set game directories`. Simply point it to each individual platform sub-directory. For RetroPie you should have a path for each platform (eg. `/home/USER/RetroPie/roms/snes`, `/home/USER/RetroPie/roms/megadrive` etc.).

IMPORTANT! If you are generating game lists for Pegasus, it is highly recommended to disable third-party game list data sources! Start the Pegasus frontend, press ESC on the keyboard and choose `Settings->Enable/disable data sources` and disable everything in that submenu.

Then reload the game lists or restart Pegasus, and all of the platforms should show up with media and game information.

##### Metadata preservation
Skyscraper will preserve any metadata key-value pairs added to the header and / or individual game list entries.
