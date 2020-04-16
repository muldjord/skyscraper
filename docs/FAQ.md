#### Q: I ran Skyscraper and it scraped a lot of artwork and game data, but no gamelist was generated. What's going on?
<details>
  <summary>Expand answer...</summary>

**A:** Skyscraper has two modes. Game data gathering mode and gamelist generation mode. You need to run it in gamelist generation mode to generate the gamelist after you have gathered and cached all the data you want. You do this by running it without the `-s MODULE` command line option (eg. `Skyscraper -p snes`).
</details>
  
#### Q: Skyscraper keeps quitting on me with errors about request limits or similar. What's going on?
<details>
  <summary>Expand answer...</summary>

**A:** Almost all of the scraping modules such as ScreenScraper and TheGamesDb have restrictions applied to avoid users overloading their servers. These restrictions are set by them, not Skyscraper. Some modules even require you to log in to use them. Read more about the restrictions and how to deal with them [here](https://github.com/muldjord/skyscraper/blob/master/docs/SCRAPINGMODULES.md). If you've created a user at ScreenScraper and want to use your credentials with Skyscraper, just add the following section to `/home/USER/.skyscraper/config.ini`. Type it *exactly* like this, but with your own user and pass.

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

#### Q: I used Skyscraper to generate EmulationStation gamelists for one or more platforms, but when I restarted the system my gamelists hadn't changed. Why is that?
<details>
  <summary>Expand answer...</summary>

**A:** When EmulationStation restarts (eg. when you reboot the system through EmulationStation) it writes its current gameslists back to disk, thereby overwriting any gamelists you just generated with Skyscraper. You need to quit EmulationStation before using Skyscraper to avoid this. You can quit EmulationStation by pressing F4 on a connected keyboard, or by selecting it in the menus.
</details>

#### Q: I installed Skyscraper from the RetroPie optional packages and want to run it from the terminal. But when I type `Skyscraper` it says `Skyscraper: command not found`. Why is that?
<details>
  <summary>Expand answer...</summary>

**A:** The RetroPie-Setup script installs the Skyscraper executable in a location that is not normally searched by the system when looking for executables. You can create a link to a location that is searched through to remedy this by running `sudo ln -s /opt/retropie/supplementary/skyscraper/Skyscraper /usr/local/bin/Skyscraper`. You might have to type in your sudo password. For RetroPie default is `raspberry`. If that command was succesful you should now be able to run Skyscraper simply with `Skyscraper` followed by any options you need.
</details>
