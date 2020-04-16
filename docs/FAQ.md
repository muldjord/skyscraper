Q: I ran Skyscraper and it scraped a lot of artwork and game data, but no gamelist was generated. What's going on?
A: Skyscraper has two modes. Game data gathering mode and gamelist generation mode. You need to run it in gamelist generation mode to generate the gamelist after you have gathered and cached all the data you want. You do this by running it without the `-s MODULE` command line option (eg. `Skyscraper -p snes`).

Q: Skyscraper keeps quitting on me with errors about request limits or similar. What's going on?

A: Almost all of the scraping modules such as ScreenScraper and TheGamesDb have restrictions applied to avoid users overloading their servers. These restrictions are set by them, not Skyscraper. Some modules even require you to log in to use them. Read more about the restrictions and how to deal with them [here](https://github.com/muldjord/skyscraper/blob/master/docs/SCRAPINGMODULES.md). If you've created a user at ScreenScraper and want to use your credentials with Skyscraper, just add the following section to `/home/USER/.skyscraper/config.ini`. Type it *exactly* like this, but with your own user and pass.

```
[screenscraper]
userCreds="USER:PASS"
```

Q: Skyscraper by default generates a composited screenshot that combines both boxart, screenshot and wheel. I just want it to export the raw artwork, how do I do that?

A: Skyscraper includes a powerful artwork compositor you can set up any way you like. Read more about the possibilities [here](https://github.com/muldjord/skyscraper/blob/master/docs/ARTWORK.md). If you just want it to export the raw unmodified screenshot and other pieces of artwork, you can use the `/home/USER/.skyscraper/artwork.xml.example2` and copy it to `/home/USER/.skyscraper/artwork.xml`. This will do the trick.