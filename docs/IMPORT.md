# Custom data import
The following describes how to import your own custom textual, artwork and / or video data into the resource cache for later use when generating game lists (enable game list generation mode simply by leaving out the `-s` command line option).

NOTE 1: For any path in the following description, you can also add a platform subfolder. Skyscraper will auto-detect this and use it instead of the base import folder. For instance, you can create the folder `/home/USER/.skyscraper/import/amiga` and it will use that as base instead of `/home/USER/.skyscraper/import/` when importing for the `amiga` platform.

NOTE 2: Be sure to also check the `--cache edit` option [here](CLIHELP.md#--cache-editnewtype).

## Images and videos
To import videos or images into the resource cache, use the following procedure:
* Name your image or video file with the *exact* base name of the rom you wish to connect it to. Example: `Bubble Bobble.nes` will import images with a filename of `Bubble Bobble.jpg` or `Bubble Bobble.png` or other well-known image formats. As long as the base name is an *exact* match. Same goes for video files. I recommend only making use of well-known video formats since Skyscraper imports them directly without conversion, so they need to be supported directly by the frontend you plan to use.
* Place all of your images or videos in the `/home/USER/.skyscraper/import/screenshots`, `covers`, `wheels`, `marquees` or `videos` folders.
* Now run Skyscraper with `Skyscraper -p <PLATFORM> -s import`. If you named your files correctly, they will now be imported. Look for the green 'YES' in the output at the rom(s) you've placed files for. This will tell you if it succeeded or not.
* The data is now imported into the resource cache. To make use of if read [here](#how-to-actually-use-the-data).

### Special note for videos
If you are importing videos, you also need to add the command line flag `--flags videos` for this to work. Videos aren't imported or scraped by default, since it is considered a huge disk space hog. So keep this in mind if you want to import videos into the cache. If you plan on always using videos, consider adding this option to the [config.ini](CONFIGINI.md) instead.

## Textual data (publisher, players, rating and so on)
Skyscraper also allows you to import textual data for any rom you have in your collection. All you need to do is to prepare files for each rom with an *exactly* matching base name. For instance `Bubble Bobble.nes` should have a file called `Bubble Bobble.txt` or `Bubble Bobble.xml` or whatever suffix you want to use. The suffix is not important. What *is* important is that you place all of these raw data files into the `/home/USER/.skyscraper/import/textual` folder. And then you need to make a definitions file so Skyscraper has a recipe for these files.

### Textual data definitions file
In order for Skyscraper to understand your textual data files, it needs a recipe. Or a definition of your format if you like. The format is completely up to you. The file must be placed at `/home/USER/.skyscraper/import/definitions.dat`. Here follows a few examples with a matching data file for comparison:

#### Example 1
Definitions file: `/home/USER/.skyscraper/import/definitions.dat`
```
<game>
  <title>###TITLE###</title>
  <description>###DESCRIPTION###</description>
  <developer>###DEVELOPER###</developer>
  <publisher>###PUBLISHER###</publisher>
  <players>###PLAYERS###</players>
  <rating>###RATING###</rating>
  <ages>###AGES###</ages>
  <genre>###TAGS###</genre>
  <releasedate>###RELEASEDATE###</releasedate>
</game>
```

Import file: `/home/USER/.skyscraper/import/textual/<EXACT ROM BASE NAME>.xml`
```
<game>
  <title>The Game Title</title>
  <description>This game is about yada, yada yada.</description>
  <developer>The game developer</developer>
  <publisher>The game publisher</publisher>
  <players>Maximum number of players supported (example '4')</players>
  <rating>The game rating 0-5 (example '3.5')</rating>
  <ages>Minimum age restriction as integer (example '16')</ages>
  <genre>Genres, Comma-separated</genre>
  <releasedate>The game release date (example '1985-06-01')</releasedate>
</game>

```
Make sure any line matches *exactly* with the line in the recipe including white-space characters such as spaces and tabs! Otherwise Skyscraper won't recognize it.

#### Example 2
Definitions file: `/home/USER/.skyscraper/import/definitions.dat`
```
Title      : ###TITLE###
Description: ###DESCRIPTION###
Developer  : ###DEVELOPER###
Publisher  : ###PUBLISHER###
Players    : ###PLAYERS###
Rating     : ###RATING###
Rating     : ###AGES###
Genre      : ###TAGS###
Date       : ###RELEASEDATE###
```

Import file: `/home/USER/.skyscraper/import/textual/<EXACT ROM BASE NAME>.txt`
```
Title      : Game Title
Description: This game is about yada, yada yada.
Developer  : The game developer
Publisher  : The game publisher
Players    : Maximum number of players supported (example '4')
Rating     : The game rating 0-5 (example '3.5')
Ages       : Minimum age restriction as integer (example '16')
Genre      : Genres, Comma-separated
Date       : The game release date (example '1985-06-01')
```

#### List of known tags
From the examples above you'll notice the `###SOMETHING###` tags. This is what Skyscraper recognizes your data from. The supported tags are:

* `###TITLE###`
* `###DESCRIPTION###`
* `###DEVELOPER###`
* `###PUBLISHER###`
* `###PLAYERS###`
* `###AGES###`
* `###RATING###`
* `###TAGS###`
* `###RELEASEDATE###`

#### Resource formats
For `###PLAYERS###`, `###RATING###`, `###AGES###`, and `###RELEASEDATE###` a certain format is required. Players must contain just the maximum number of players as an integer such as '4'. Rating must be a number between 0.0 and 5.0. Ages must be an integer between 1 and 18 (for instance "16" means it is suitable from ages 16 and up). Release date must be of one of the following formats:
* `yyyy`
* `yyyy-MM`
* `yyyy-MM-dd`
* `yyyy-MMM-dd` (MMM is Jan, Feb and so on...)
* `MM/dd/yyyy`
* `MMM, yyyy` (MMM is Jan, Feb and so on...)
* `MMM dd, yyyy` (MMM is Jan, Feb and so on...)

## How to actually USE the data
When you've imported all of your data into the resource cache, you can make use of it by enabling Skyscrapers *game list generation* mode simply by leaving out the `-s` command line option entirely. The game list generator will then make use of your imported data. If you don't know what the resource cache is, read more about it [here](CACHE.md).
