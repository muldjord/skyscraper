## A typical Skyscraper use case
The following describes a typical way of using Skyscraper. It's important to know that Skyscraper isn't just a "run once and you're done" tool. It isn't complicated to use by any means. It just has a couple of layers you should familiarize yourself with in order to get the best results.

So let's dig in!

### You have some files and no data
You probably installed Skyscraper in order to get some data and artwork on your frontend game lists. So let's say that you have a bunch of Super Nintendo files you wish to populate with data.


#### The gathering phase
Skyscraper can gather data from several scraping modules / sources (set with `-s [module]`). So you'd start by scraping the `snes` platform (set with `-p [platform]`) with one or more of those modules. Each time you do so, all of the data will be saved in the Skyscraper cache.

```
$ Skyscraper -p snes -s screenscraper
```
This should have given you a lot of data for your games. But some of it might still be missing. So let's continue gathering some data from another module.
```
$ Skyscraper -p snes -s thegamesdb
```
For each of those two commands, all of the data that was gathered is now cached inside Skyscraper. For each game it will contain information from both of those sources.

#### The combining phase
And here comes the clever part. By having gathered data from both sources (`screenscraper` *and* `thegamesdb`) there's a good chance that you will get a complete result if the data from both is combined. And that's *exactly* what Skyscraper can do for you.
```
$ Skyscraper -p snes
```
Running that command will implicitly add `-s localdb` which scrapes data from the local Skyscraper cache. And it will do so by combining all of the cached data into the most complete results. This phase also includes the artwork compositing and game list generation.

### So what then?
You're done! The platform has been scraped and you can relaunch your frontend to enjoy the newly scraped data and artwork. Or you can move on and scrape data for another platform. Just remember to distinguish between the *gathering* phase and the *combining* phase. Always gather first, then combine afterwards. *Always!*

### But what about...
Yes, yes, yes. This is just the basics. You can customize the artwork completely to your liking. You can prioritize the different cached resources any way you like. You can scrape single games in order to gather information for just that one game. You can enable video scraping. You can import your own data into the cache. You can clean your cache. You can create aliases for games that are difficult to scrape. You can change your preferred region and language for the modules that support it. You can set up keys or user id's and password for the scraping modules that take advantage of this. There's *a lot* you can do if you want to.

The important thing to note is that most of the defaults for Skyscraper is set to work well for most users. So unless you see something you want to change, you don't have to. But it's equally important to know that you *can*.

And if you're interested in any of this, please read the entire documentation thorougly. Everything is documented to the last detail. It can seem overwhelming. But it also gives you some really powerful options to scrape just the way you want.
