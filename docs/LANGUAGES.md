## List of supported languages
### Scraping modules that support languages
* screenscraper

Pretty much all resources from `screenscraper` have language support. Use the short-name only when setting it, check the examples further down.

### Languages
* de: German
* en: English
* zh: Chinese
* ko: Korean
* da: Danish
* es: Spanish
* fi: Finnish
* fr: French
* hu: Hungarian
* it: Italian
* ja: Japanese
* nl: Dutch
* no: Norwegian
* pl: Polish
* pt: Portuguese
* ru: Russian
* sk: Slovakian
* sv: Swedish
* cz: Czech
* tr: Turkish

## Examples
### Command line
`--lang de`
### ~/.skyscraper/config.ini
```
[main]
lang="de"
```
or
```
[any platform]
lang="de"
```

## Other notes
When a language is specified by the user, this language will be prioritized. However, if a resource can't be found with that specific language, it will fall back to `en` (English).