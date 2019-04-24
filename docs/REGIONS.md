## List of supported regions
### Scraping modules that support regions
* screenscraper
* igdb

Please note that not all regions are supported by all modules (support is noted inside the `()` for each region). And only certain resources from those modules can be extracted with a specific region. For `screenscraper` pretty much all resources have region support. For `igdb` it's only the release date that has region support. Use the short-name only when setting it, check the examples further down.

### Regions
* de: Germany (screenscraper)
* asi: Asia (screenscraper, igdb)
* au: Australia (screenscraper, igdb)
* br: Brazil (screenscraper)
* bg: Bulgaria (screenscraper)
* ca: Canada (screenscraper)
* cl: Chile (screenscraper)
* cn: China (screenscraper, igdb)
* ame: American continent (screenscraper)
* kr: Korea (screenscraper)
* cus: Custom (screenscraper)
* dk: Denmark (screenscraper)
* sp: Spain (screenscraper)
* eu: Europe (screenscraper, igdb)
* fi: Finland (screenscraper)
* fr: France (screenscraper)
* gr: Greece (screenscraper)
* hu: Hungary (screenscraper)
* il: Israel (screenscraper)
* it: Italy (screenscraper)
* jp: Japan (screenscraper, igdb)
* kw: Kuwait (screenscraper)
* wor: World (screenscraper, igdb)
* mor: Middle East (screenscraper)
* no: Norway (screenscraper)
* nz: New Zealand (screenscraper, igdb)
* oce: Oceania (screenscraper)
* nl: Netherlands (screenscraper)
* pe: Peru (screenscraper)
* pl: Poland (screenscraper)
* pt: Portugal (screenscraper)
* cz: Czech republic (screenscraper)
* uk: United Kingdom (screenscraper)
* ru: Russia (screenscraper)
* sk: Slovakia (screenscraper)
* se: Sweden (screenscraper)
* tw: Taiwan (screenscraper)
* tr: Turkey (screenscraper)
* us: USA (screenscraper, igdb)

## Examples
### Command line
`--region de`
### ~/.skyscraper/config.ini
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
Skyscraper will try to autodetect the region from the file name. It will look for designations such as `(Europe)` or `(e)` and set the region accordingly. This currently only works for Europe, Usa, World and Japan regions.

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
