TEMPLATE = app
TARGET = Skyscraper
DEPENDPATH += .
INCLUDEPATH += .
CONFIG += release
QT += core network xml
QMAKE_CXXFLAGS += -std=c++11

unix:target.path=/usr/local/bin
unix:target.files=Skyscraper Skyscraper.app/Contents/MacOS/Skyscraper

unix:examples.path=/usr/local/etc/skyscraper
unix:examples.files=config.ini.example README.md hints.txt artwork.xml artwork.xml.example1 artwork.xml.example2 artwork.xml.example3 artwork.xml.example4 aliasMap.csv mameMap.csv docs/ARTWORK.md tgdb_developers.json tgdb_publishers.json

unix:cacheexamples.path=/usr/local/etc/skyscraper/cache
unix:cacheexamples.files=cache/priorities.xml.example docs/CACHE.md

unix:impexamples.path=/usr/local/etc/skyscraper/import
unix:impexamples.files=docs/IMPORT.md import/definitions.dat.example1 import/definitions.dat.example2

unix:resexamples.path=/usr/local/etc/skyscraper/resources
unix:resexamples.files=resources/maskexample.png resources/frameexample.png resources/boxfront.png resources/boxside.png resources/scanlines1.png resources/scanlines2.png

INSTALLS += target examples cacheexamples impexamples resexamples

include(./VERSION)
DEFINES+=VERSION=\\\"$$VERSION\\\"

HEADERS += src/skyscraper.h \
           src/netcomm.h \
           src/xmlreader.h \
           src/settings.h \
           src/compositor.h \
           src/strtools.h \
           src/imgtools.h \
           src/esgamelist.h \
           src/scraperworker.h \
           src/cache.h \
           src/localscraper.h \
           src/importscraper.h \
           src/gameentry.h \
           src/abstractscraper.h \
           src/abstractfrontend.h \
           src/emulationstation.h \
           src/attractmode.h \
           src/openretro.h \
           src/thegamesdb.h \
           src/worldofspectrum.h \
           src/screenscraper.h \
           src/crc32.h \
           src/mobygames.h \
           src/igdb.h \
           src/arcadedb.h \
           src/scripter.h \
           src/platform.h \
           src/layer.h \
           src/fxshadow.h \
           src/fxblur.h \
           src/fxmask.h \
           src/fxframe.h \
           src/fxrounded.h \
           src/fxstroke.h \
           src/fxbrightness.h \
           src/fxcontrast.h \
           src/fxbalance.h \
           src/fxopacity.h \
           src/fxgamebox.h \
           src/fxhue.h \
           src/fxsaturation.h \
           src/fxcolorize.h \
           src/fxrotate.h \
           src/fxscanlines.h \
           src/fxsharpen.h \
           src/nametools.h \
           src/queue.h

SOURCES += src/main.cpp \
           src/skyscraper.cpp \
           src/netcomm.cpp \
           src/xmlreader.cpp \
           src/compositor.cpp \
           src/strtools.cpp \
           src/imgtools.cpp \
           src/esgamelist.cpp \
           src/scraperworker.cpp \
           src/cache.cpp \
           src/localscraper.cpp \
           src/importscraper.cpp \
           src/gameentry.cpp \
           src/abstractscraper.cpp \
           src/abstractfrontend.cpp \
           src/emulationstation.cpp \
           src/attractmode.cpp \
           src/openretro.cpp \
           src/thegamesdb.cpp \
           src/worldofspectrum.cpp \
           src/screenscraper.cpp \
           src/crc32.cpp \
           src/mobygames.cpp \
           src/igdb.cpp \
           src/arcadedb.cpp \
           src/scripter.cpp \
           src/platform.cpp \
           src/layer.cpp \
           src/fxshadow.cpp \
           src/fxblur.cpp \
           src/fxmask.cpp \
           src/fxframe.cpp \
           src/fxrounded.cpp \
           src/fxstroke.cpp \
           src/fxbrightness.cpp \
           src/fxcontrast.cpp \
           src/fxbalance.cpp \
           src/fxopacity.cpp \
           src/fxgamebox.cpp \
           src/fxhue.cpp \
           src/fxsaturation.cpp \
           src/fxcolorize.cpp \
           src/fxrotate.cpp \
           src/fxscanlines.cpp \
           src/fxsharpen.cpp \
           src/nametools.cpp \
           src/queue.cpp
