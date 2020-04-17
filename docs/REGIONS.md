## List of supported regions
### Scraping modules that support regions
* screenscraper (pretty much all resources)
* igdb (only release date)
* mobygames (only cover artwork)

Please note that not all regions are supported by all modules (support is noted inside the `()` for each country or region). And only certain resources from those modules can be extracted with a specific country or region (see above).

When setting the country or region use the short-name as shown. Check the examples further down.

### Regions
* de: Germany (screenscraper, mobygames)
* asi: Asia (screenscraper, igdb)
* au: Australia (screenscraper, igdb, mobygames)
* br: Brazil (screenscraper, mobygames)
* bg: Bulgaria (screenscraper, mobygames)
* ca: Canada (screenscraper, mobygames)
* cl: Chile (screenscraper, mobygames)
* cn: China (screenscraper, igdb, mobygames)
* ame: American continent (screenscraper)
* kr: Korea (screenscraper, mobygames)
* cus: Custom (screenscraper)
* dk: Denmark (screenscraper, mobygames)
* sp: Spain (screenscraper, mobygames)
* eu: Europe (screenscraper, igdb)
* fi: Finland (screenscraper, mobygames)
* fr: France (screenscraper, mobygames)
* gr: Greece (screenscraper, mobygames)
* hu: Hungary (screenscraper, mobygames)
* il: Israel (screenscraper, mobygames)
* it: Italy (screenscraper, mobygames)
* jp: Japan (screenscraper, igdb, mobygames)
* kw: Kuwait (screenscraper)
* wor: World (screenscraper, igdb, mobygames)
* mor: Middle East (screenscraper)
* no: Norway (screenscraper, mobygames)
* nz: New Zealand (screenscraper, igdb, mobygames)
* oce: Oceania (screenscraper)
* nl: Netherlands (screenscraper, mobygames)
* pe: Peru (screenscraper)
* pl: Poland (screenscraper, mobygames)
* pt: Portugal (screenscraper, mobygames)
* cz: Czech republic (screenscraper, mobygames)
* uk: United Kingdom (screenscraper, mobygames)
* ru: Russia (screenscraper, mobygames)
* sk: Slovakia (screenscraper, mobygames)
* se: Sweden (screenscraper, mobygames)
* tw: Taiwan (screenscraper, mobygames)
* tr: Turkey (screenscraper, mobygames)
* us: USA (screenscraper, igdb, mobygames)
* ss: ScreenScraper (Know what defines this? Let me know) (screenscraper)

## Examples
### Command line
`--region de`
### /home/USER/.skyscraper/config.ini
```
[main]
region="de"
```
or
```
[any platform]
region="de"
```

## Other notes
### Region autodetection
Skyscraper will try to autodetect the region from the file name. It will look for designations such as `(Europe)` or `(e)` and set the region accordingly. This currently works for the following regions and / or countries:
* eu
* us
* wor
* jp
* br
* kr
* tw
* fr
* de
* it
* sp
* cn
* au
* se
* ca
* nl
* dk
* asi

### Default region prioritization
When a region is specified by the user, this region will be prioritized. However, if a resource can't be found with that specific region, it will fall back to the default list of regions and look for a resource with the topmost region first, then the next and so on:
* eu
* us
* ss
* uk
* wor
* jp
* au
* ame
* de
* cus
* cn
* kr
* asi
* br
* sp
* fr
* gr
* it
* no
* dk
* nz
* nl
* pl
* ru
* se
* tw

### Override default region prioritization list
It is also possible to completely override the default prioritization list with the `regionPrios="<COMMASEPARATED REGION LIST>"` config.ini option. This can *not* be done on command-line.

#### Examples
##### /home/USER/.skyscraper/config.ini
```
[main]
regionPrios="de,us,jp"
```
or
```
[<PLATFORM>]
regionPrios="de,us,jp"
```
