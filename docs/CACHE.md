# Resource cache
Whenever you scrape any platform with any scraping module, Skyscraper caches each resource internally. A resource can, for instance, be a game `description` or a game `screenshot`. Each game can have several versions of each resource cached locally. One of each type per scraping module. This comes in handy when generating the game list and artwork compositions.

Think of the resource cache as the cache in an internet browser. Most of the data on webpages don't change much. By caching some of the data locally, it can load parts of a webpage from that data instead of loading it from the remote server. This is exactly how the Skyscraper cache works. It helps keep the online servers healthy by not hammering them whenever you need resources you already downloaded once. And it allows you to re-generate the frontend game lists if you add new games or perhaps want to change the style of the exported artwork.

#### Default resource cache folder
The default base folder for all of Skyscrapers' locally cached data is in the `/home/USER/.skyscraper/cache` folder. In this folder you'll find individual platform subfolders. Any of these are selfcontained and contains all of the cached data for that particular platform.

#### Resource and scraping module priorities
There is ONE file that you can and should edit inside each of the `/home/USER/.skyscraper/cache/<PLATFORM>` folders. That file is called `priorities.xml` and decides the scraper priority of resources for each resource type. For instance, if you know that `thegamesdb` always provides the best `descriptions` for games, you'd add an `<order type="description">` node with a `<source>thegamesdb</source>` subnode. You can have multiple `<source>` nodes, Skyscraper will then prefer the topmost source when generating a game list. If the topmost isn't found, it'll use the next one and so on. If the `<order>` node is empty, it will prioritize using timestamps for when each resource was added to the cache.

Skyscraper provides the example file `/home/USER/.skyscraper/cache/priorities.xml.example`. Please don't edit this file manually, as it will be overwritten when you update Skyscraper. When a platform is scraped for the first time, it will automatically copy the example file to `/home/USER/.skyscraper/cache/<PLATFORM>/priorities.xml` unless it already exists. You can of course also copy the file yourself before scraping a platform. If you do so, be sure to remove the `.example` part of the filename so it's just called `priorities.xml`.

#### Update locally cached data
If you wish to update / refresh the locally cached resources for a particular platform and scraping module, Skyscraper provides the `--refresh` option. If this flag is set on the command line, any data in the resource cache will be updated with the new incoming data.

If you wish to just refresh the data for a single rom simply scrape it with `-p <PLATFORM> -s <SCRAPING MODULE> <RELATIVE OR FULL ROM PATH AND FILENAME>` and the locally cached data for that particular rom will be updated / refreshed. You can add more filenames one after the other if you like. If any filename or paths has spaces in it, remember to double-quote it like so `"relative path/to rom/rom filename.sfc"`. You don't need the `--refresh` option when updating single files - it is implicit.

When you've updated information in the resource cache, always remember to re-generate the game list by simply running `Skyscraper -p <PLATFORM>` when you're done. The updated resources won't be visible in your frontend until you do.

## The resource cache folder and file structures
MOST FILES AND FOLDERS INSIDE THE `/home/USER/.skyscraper/cache` FOLDER ARE NOT MEANT TO BE MANIPULATED BY HAND!!! It can be done, but don't complain to me about the format of the database. It is NOT meant to be understood by humans. It is meant to be efficient for reading and parsing by Skyscraper itself. Same goes for the media files that reside in the subfolders.

#### Don't leave custom files in here
If you decide to add your own files to the subfolders, you risk them being deleted by Skyscraper later on if it is run with one of the cache cleanup command line options. You've been warned!

#### Other cool stuff you CAN DO
Each subfolder in the `/home/USER/.skyscraper/cache/` folder is self-contained and can be copied to other Skyscraper installations at your convenience. Just copy the folder itself over to some other computer that has Skyscraper 1.6.0 or later installed, and you can make use of the data when generating game lists. If you add it at a non-default location, set the custom folder with `-d <FOLDER>`.

#### Resource cache format
I do not recommend editing the `db.xml` resource cache files manually. But the format is simple, so you certainly can if you want to.

##### Resource id
The database consists of resource entries connected to a unique id. The id is calculated from the rom data or, in special cases, the filename (in cases where the file data is a script or similar). An entry can look like this:

NOTE! Pre-3.3.0 versions of Skyscraper used `sha1` as the name of the unique id key. Later versions use `id`.

```xml
<resource id="<ID KEY>" type="<RESOURCE TYPE>" source="<SCRAPING SOURCE>" timestamp="<UNIX TIMESTAMP IN MSECS>">Resource data</resource>
```

#### Resource types
##### title
A game title
##### platform
A game platform
##### description
A game description
##### publisher
The publisher of a game
##### developer
The developer of a game
##### players
How many players are supported by a game
##### tags
List of game tags, most often genre related
##### releasedate
The release date of a game
##### ages
Game age rating, defined as youngest allowed age in years
##### rating
Game rating, number between 0 and 1
##### cover
A cover image filename for a game (file exists in `covers` subfolder)
##### screenshot
A screenshot image filename for a game (file exists in `screenshots` subfolder)
##### wheel
A wheel image filename for a game (file exists in `wheels` subfolder)
##### marquee
A marquee image filename for a game (file exists in `marquees` subfolder)
##### video
A video file filename for a game (file exists in `videos` subfolder)
