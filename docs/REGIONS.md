## Regions
Some game information and / or game media is region-based. Skyscraper provides several ways of configuring these for your convenience. But most importantly; it supports region auto-detection directly from the file names. Read on for more information about how regions are handled by Skyscraper.

## Scraping modules that support regions
* screenscraper (pretty much all game information and media)
* igdb (only release date)
* mobygames (only cover artwork)

Below follows a general list of supported regions. Please note that not all regions are supported by all modules (support is noted inside the `()` for each country or region).

When configuring regions be sure to use the short-names as shown (eg. 'fr' for France).

## List of supported regions
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
* ss: ScreenScraper (screenscraper)

### Region auto-detection
Skyscraper will try to auto-detect the region from the file name. It will look for designations such as `(Europe)` or `(e)` and set the region accordingly. This currently works for the following regions and / or countries:
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

So if your files are named like `Game Name (Europe).zip`, there's no need to configure regions manually. Skyscraper will recognize `Europe` and add it to the top of the internal region priority list. If info or media isn't found for the auto-detected region, it will move down the list and check the next region on the list until it finds one that has data for the requested resource.

### Default region prioritization
Skyscraper's default internal region priority list is as follows. Topmost region has highest priority and so forth.
* Auto-detected region
* Region set with `--region REGION` or `region="REGION"`
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
* ca

## Configuring region manually
If you insist, of course you can configure the region manually. You can either do this on command-line or through `/home/USER/.skyscraper/config.ini`. It is recommended to set it in `config.ini` for a permanent setup.

### config.ini
Read [this](CONFIGINI.md#regionwor) and [this](CONFIGINI.md#regionprioseuusssukworjp).

### Command line
Read [this](CLIHELP.md#--region-code).
