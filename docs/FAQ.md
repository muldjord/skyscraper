### The official Skyscraper F.A.Q (frequently asked questions)
A quick note before I let you loose on the questions:
* You can get a list of all available command-line options by running `Skyscraper --help`
* For a more thorough description of the command-line options go [here](https://github.com/muldjord/skyscraper/blob/master/docs/CLIHELP.md)
* You can read all about the available `config.ini` options [here](https://github.com/muldjord/skyscraper/blob/master/docs/CONFIGINI.md)

And now for the questions (and answers)...

#### Q: I ran Skyscraper and it scraped a lot of artwork and game data, but no gamelist was generated. What's going on?
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

**A:** Whenever you make any gamelist specific changes to Skyscraper's configurations, you need to regenerate the gamelists for your chosen frontend afterwards for the changes to take effect. This can be done simply by running Skyscraper with `Skyscraper -p PLATFORM`. This will regenerate the gamelist for that platform using your new configuration.
</details>

#### Q: I used Skyscraper to generate EmulationStation gamelists for one or more platforms, but when I restarted the system the gamelist hadn't changed. Why is that?
<details>
  <summary>Expand answer...</summary>

**A:** When EmulationStation restarts (eg. when you reboot the system through EmulationStation) it writes its current gameslists back to disk, thereby overwriting any gamelists you just generated with Skyscraper. You need to quit EmulationStation before generating the gamelists with Skyscraper to avoid this. You can quit EmulationStation by pressing F4 on a connected keyboard, or by selecting it in the menus.
</details>

#### Q: I installed Skyscraper from the RetroPie optional packages and want to run it from the terminal. But when I type `Skyscraper` it says `Skyscraper: command not found`. Why is that?
<details>
  <summary>Expand answer...</summary>

**A:** The RetroPie-Setup script installs the Skyscraper executable in a location that is not normally searched by the system when looking for executables. For ease of use you can create a link to a location that is searched by running `sudo ln -s /opt/retropie/supplementary/skyscraper/Skyscraper /usr/local/bin/Skyscraper`. You might be asked for your sudo password, for RetroPie default is `raspberry`. If that command was succesful you should now be able to run Skyscraper simply with `Skyscraper` followed by any options you need.
</details>

#### Q: I want to set up different options for different platforms. It's really tiresome setting it all on command-line. Can this be done easier?
<details>
  <summary>Expand answer...</summary>

**A:** YES! Absolutely. In addition to taking options from the command-line, Skyscraper also reads the `/home/USER/.skyscraper/config.ini` file, which is where you should set everything up. This file allows you to configure things both globally, per-platform, per-frontend and per-scraping module. Read more about the available options and option priorities [here](https://github.com/muldjord/skyscraper/blob/master/docs/CONFIGINI.md)
</details>
