## Languages
Some game information comes in several languages. Skyscraper provides several ways of configuring these for your convenience. It is important to understand the distinction between game region and language. Setting a language is a user-preferred thing and will only affect the game descriptions and tags (genres). The remaining game data is tied to the region instead (artwork and, in some cases, the game name).

## Scraping modules that support languages
* screenscraper (game description and tags / genres)

Below follows a general list of supported languages. Please note that not all languages are supported by all modules (support is noted inside the `()` for each language).

When configuring languages be sure to use the short-names as shown (eg. 'no' for Norwegian).

## List of supported languages
* de: German (screenscraper)
* en: English (screenscraper)
* zh: Chinese (screenscraper)
* ko: Korean (screenscraper)
* da: Danish (screenscraper)
* es: Spanish (screenscraper)
* fi: Finnish (screenscraper)
* fr: French (screenscraper)
* hu: Hungarian (screenscraper)
* it: Italian (screenscraper)
* ja: Japanese (screenscraper)
* nl: Dutch (screenscraper)
* no: Norwegian (screenscraper)
* pl: Polish (screenscraper)
* pt: Portuguese (screenscraper)
* ru: Russian (screenscraper)
* sk: Slovakian (screenscraper)
* sv: Swedish (screenscraper)
* cz: Czech (screenscraper)
* tr: Turkish (screenscraper)

### Default language prioritization
Skyscraper's default internal language priority list is as follows. Topmost language has highest priority and so forth.
* Auto-detected language or language set with `--lang LANGUAGE` or `language="LANGUAGE"`
* en
* de
* fr
* es

## Configuring language
It is possible to manipulate the default internal language priority list. You can either do this on command-line or through `/home/USER/.skyscraper/config.ini`. It is recommended to set it in `config.ini` for a permanent setup.

### config.ini
Read [this](CONFIGINI.md#langen) and [this](CONFIGINI.md#langpriosendees).

### Command line
Read [this](CLIHELP.md#--lang-code).
