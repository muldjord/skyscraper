### The official Skyscraper F.A.Q (frequently asked questions)
A quick note before I let you loose on the questions:
* You can get a list of all available command-line options by running `Skyscraper --help`
* For a more thorough description of the command-line options go [here](https://github.com/muldjord/skyscraper/blob/master/docs/CLIHELP.md)
* You can read all about the available `config.ini` options [here](https://github.com/muldjord/skyscraper/blob/master/docs/CONFIGINI.md)

And now for the questions (and answers)...

#### Q: I ran Skyscraper and it scraped a lot of artwork and game data, but no data is showing up in my gamelists. What's going on?
<details>
  <summary>Expand answer...</summary>

**A:** You probably forgot to generate the gamelist. Skyscraper has two modes; resource gathering mode and gamelist generation mode. First you gather data into Skyscraper's resource cache by scraping the platform with any of the supported scraping modules (eg. `Skyscraper -p snes -s thegamesdb`). When you feel like you have gathered all the resources that you need, you then generate the gamelist by simply leaving out the `-s MODULE` option (eg. `Skyscraper -p snes`).

Read more about the resource cache [here](https://github.com/muldjord/skyscraper/blob/master/docs/CACHE.md).
</details>
  
#### Q: Skyscraper keeps quitting on me with errors about request limits or similar. What's going on?
<details>
  <summary>Expand answer...</summary>

**A:** Almost all of the scraping modules, such as ScreenScraper and TheGamesDb, have restrictions applied to avoid users overloading their servers. These restrictions are set by them, not Skyscraper. Some modules even require you to log in to use them. Read more about the restrictions and how to deal with them [here](https://github.com/muldjord/skyscraper/blob/master/docs/SCRAPINGMODULES.md). If you've created a user at ScreenScraper and want to use your credentials with Skyscraper, just add the following section to `/home/USER/.skyscraper/config.ini`. Type it *exactly* like this, but with your own user and pass.

```
[screenscraper]
userCreds="USER:PASS"
```
</details>

#### Q: Skyscraper by default generates a composited screenshot that combines both boxart, screenshot and wheel. I just want it to export the raw artwork, how do I do that?
<details>
  <summary>Expand answer...</summary>

**A:** Skyscraper includes a powerful artwork compositor you can set up any way you like. Read more about the possibilities [here](https://github.com/muldjord/skyscraper/blob/master/docs/ARTWORK.md). If you just want it to export the raw unmodified screenshot and other pieces of artwork, you can use the `/home/USER/.skyscraper/artwork.xml.example2` and copy it to `/home/USER/.skyscraper/artwork.xml`. Remember to regenerate the gamelists after you've done this to make use of the new artwork configuration. This can be done simply by running Skyscraper with `Skyscraper -p PLATFORM`. This will regenerate the gamelist for that platform using this new artwork configuration.
</details>

#### Q: I've edited the artwork.xml or some other part of Skyscraper's gamelist specific configurations, but the games still show up the same inside my frontend. Why is that?
<details>
  <summary>Expand answer...</summary>

**A:** Whenever you make any gamelist specific changes to Skyscraper's configurations, you need to regenerate the gamelists for your chosen frontend afterwards for the changes to take effect. This can be done simply by running Skyscraper with `Skyscraper -p PLATFORM`. This will regenerate the gamelist for that platform for the default EmulationStation frontend using your new configuration.

If you wish to export for one of the other supported frontends, you need to specify this. Read more about the frontends and their requirements [here](FRONTENDS.md).
</details>

#### Q: I used Skyscraper to generate EmulationStation gamelists for one or more platforms, but when I restarted the system the gamelist hadn't changed. Why is that?
<details>
  <summary>Expand answer...</summary>

**A:** Did you remember to quit EmulationStation before running Skyscraper? EmulationStation writes its current gamelists back to disk when it is closed. If you run Skyscraper while EmulationStation is also running, it will overwrite any gamelists you just generated. You need to quit EmulationStation before generating the gamelists with Skyscraper to avoid this. You can quit EmulationStation by pressing F4 on a connected keyboard, or by selecting quit from the menus.
</details>

#### Q: I installed Skyscraper from the RetroPie optional packages and want to run it from the terminal. But when I type `Skyscraper` it says `Skyscraper: command not found`. Why is that?
<details>
  <summary>Expand answer...</summary>

**A:** The RetroPie-Setup script installs the Skyscraper executable in a non-default location that is not searched by the OS. You can fix this by creating a symbolic link to the executable in a location that the OS is aware of by running `sudo ln -s /opt/retropie/supplementary/skyscraper/Skyscraper /usr/local/bin/Skyscraper`. You might be asked for your sudo password, for RetroPie default is `raspberry`. If that command was succesful you should now be able to run Skyscraper simply with `Skyscraper` followed by any options you need.

WARNING!!! Some people tend to run Skyscraper with `sudo Skyscraper ...`. Do *not* do this! `sudo` is *not* needed for running Skyscraper and will mess up your file permissions.
</details>

#### Q: I want to set up different options for different platforms. It's really tiresome setting it all on command-line. Can this be done easier?
<details>
  <summary>Expand answer...</summary>

**A:** YES! Absolutely. In addition to taking options from the command-line, Skyscraper also reads the `/home/USER/.skyscraper/config.ini` file, which is where you should set everything up. This file allows you to configure things both globally, per-platform, per-frontend and per-scraping module. Read more about the available options and option priorities [here](https://github.com/muldjord/skyscraper/blob/master/docs/CONFIGINI.md)
</details>

#### Q: When I try to scrape data or generate gamelists I get all sorts of write permission errors and missing configurations. What's going on?
<details>
  <summary>Expand answer...</summary>

**A:** You are probably running, or at some point ran, Skyscraper with `sudo Skyscraper` instead of just `Skyscraper`. Doing so will result in all sorts of weird behaviour as Skyscraper was then run as the `root` user instead of the ordinary user. Depending on what command-line parameters you ran Skyscraper with, this will have caused permission issues for your files in `/home/USER/RetroPie/roms` subfolders and might also have caused permission issues with the `/home/USER/.skyscraper` folder.

This is a common problem for new Linux users who are used to working with Windows. In Linux you rarely need Administrator privileges. Running a command with `sudo` in front of it will run it as the root / Administrator user. And any folders or files being generated while that command runs, will then be owned by root. When you run software as the normal user afterwards and it tries to write to those folders or files, it will fail.

You need to reset those permissions back to be owned by your ordinary user. If you are running RetroPie, the following commands might fix your problem (RUN AT YOUR OWN RISK!):
```
$ sudo chown -R pi:pi /home/pi/.skyscraper
$ sudo chown -R pi:pi /home/pi/RetroPie/roms
```
The above commands *should* be run with sudo, as we need root permission to reset the folders and files (hence the warning). But in general, never run anything with `sudo` unless you are specifically told to do so.
</details>

#### Q: I generated a game list for the Pegasus frontend. When I fire up Pegasus the data is there, but it doesn't look very good. Why is that?
<details>
  <summary>Expand answer...</summary>

**A:** Skyscraper contains a powerful artwork compositor that can customize the exported screenshot, boxart etc. any way you like. This is done through the `/home/USER/.skyscraper/artwork.xml` file. The default configuration for this file is made to look good when exporting for the default frontend EmulationStation. When you use a different frontend, you need to customize this file a bit to make it look good. For Pegasus I suggest something like this:
```
<?xml version="1.0" encoding="UTF-8"?>
<artwork>
  <output type="screenshot" width="640"/>
  <output type="cover" width="640" height="480">
    <layer resource="cover" height="480" align="center" valign="middle">
      <gamebox side="wheel" rotate="90"/>
    </layer>
  </output>
  <output type="wheel" height="200"/>
</artwork>
```
Read more about how to set up a custom `artwork.xml` [here](ARTWORK.md).
</details>

#### Q: I've changed my `artwork.xml` configuration and regenerated the game list, but it still looks the same in the frontend. Why is that?
<details>
  <summary>Expand answer...</summary>

**A:** Did you perhaps edit `/home/USER/skysource/artwork.xml` instead of `/home/USER/.skyscraper/artwork.xml`? The latter is the one you need to edit.

Read more about how to set up a custom `artwork.xml` [here](ARTWORK.md).
</details>

