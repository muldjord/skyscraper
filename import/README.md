# Local data import

## Images and videos
To import videos or images into the local Skyscraper resource cache, use the following procedure:
* Name your image file with the EXACT base name of the rom you wish to connect it to. Example: 'Bubble Bobble.nes' will import images with a filename of 'Bubble Bobble.jpg' or 'Bubble Bobble.png' or other image formats. As long as the base name is an exact match. Same goes for video files. I recommend only making use of wellknown video formats, since Skyscraper imports them directly without conversion, so they need to be supported by the frontend you plan to use.
* Place all of your images or videos in the '[homedir]/.skyscraper/import/screenshots', 'covers', 'wheels', 'marquees' or 'videos' folder.
* Now run Skyscraper with 'Skyscraper-p [platform] -s import'. If you named your files correctly, they will now be imported. Look for the green 'YES' in the output at the rom(s) you've placed files for. This will tell you if it succeeded or not.
* The data is now imported. To now make use of the data, read [here](#how-to-actually-use-the-data).

### Special note for videos
If you are importing videos, you also need to add the command line option '--videos' for this to work. Videos aren't imported or scraped per default, since it is considered a huge disk space hog. So keep this in mind if you want to import videos into the cache.

## Textual data (publisher, players, rating and so on)
Skyscraper also allows you to import textual data for any rom you have in your collection. All you need to do is to prepare files for each rom with an exactly matching base name. For instance 'Bubble Bobble.nes' should have a file called 'Bubble Bobble.txt' or 'Bubble Bobble.xml' or whatever suffix you want to use. The suffix is not important. What IS important is that you place all of these raw data files into the '[homedir]/.skyscraper/import/textual' folder. And then you need to make a definitions file so Skyscraper has a recipe for these files.

### Textual data definitions file
In order for Skyscraper to understand your textual data files, it needs a recipe. Or a definition of your format if you like. The format is completely up to you. The file must be placed at '[homedir]/.skyscraper/import/definitions.dat'. Here follows a few examples with a matching data file for comparison:
##### Example 1
Definitions file: [homedir]/.skyscraper/import/definitions.dat
```
<publisher>###PUBLISHER###</publisher>
<players>###PLAYERS</players>
<rating>###RATING###</rating>
```

Import file: [homedir]/.skyscraper/import/textual/[exact rom base name].xml
```
<publisher>This is the publisher</publisher>
<players>1 players</players>
<rating>4.5</rating>
```

##### Example 2
Definitions file: [homedir]/.skyscraper/import/definitions.dat
```
Publisher: ###PUBLISHER###
Players:   ###PLAYERS###
Rating:    ###RATING###
```

Import file: [homedir]/.skyscraper/import/textual/[exact rom base name].txt
```
Publisher: This is the publisher
Players:   2 players
Rating:    3.2
```

#### List of known tags
From the examples above you'll notice the '###SOMETHING###' tags. This is what Skyscraper regocnizes your data from. The full list of tags is:

* ###DESCRIPTION###
* ###DEVELOPER###
* ###PUBLISHER###
* ###PLAYERS###
* ###AGES###
* ###RATING###
* ###TAGS###
* ###RELEASEDATE###

#### Resource formats
For '###RATING###', ###AGES###, and '###RELEASEDATE###' a certain format is required. Rating must be a number between 0.0 and 5.0. Ages must be an integer between 1 and 18 (for instance "16" means it is suitable from ages 16 and up). Release date must be of one of the following formats:
* yyyy
* yyyy-MM
* yyyy-MM-dd
* MM/dd/yyyy
* MMM, yyyy (MMM is Jan, Feb and so on...)
* MMM dd, yyyy (MMM is Jan, Feb and so on...)

## How to actually USE the data
When you've imported all of your data into the local Skyscraper cache, you can make use of it by scraping the platform with 'Skyscraper -p [platform] -s localdb'. This will make use of your imported data. Read more about the localdb scraping module [here](../dbs/README.md).
