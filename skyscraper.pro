TEMPLATE = app
TARGET = Skyscraper
DEPENDPATH += .
INCLUDEPATH += .
CONFIG += release
QT += core network xml
QMAKE_CXXFLAGS += -std=c++11

unix:target.path=/usr/local/bin
unix:target.files=Skyscraper

unix:examples.path=/usr/local/etc/skyscraper
unix:examples.files=config.ini.example README.md artwork.xml artwork.xml.example mameMap.csv ARTWORK.md

unix:dbexamples.path=/usr/local/etc/skyscraper/dbs
unix:dbexamples.files=dbs/priorities.xml.example dbs/README.md

unix:impexamples.path=/usr/local/etc/skyscraper/import
unix:impexamples.files=import/README.md import/definitions.dat.example1 import/definitions.dat.example2

unix:resexamples.path=/usr/local/etc/skyscraper/resources
unix:resexamples.files=resources/maskexample.png resources/frameexample.png resources/boxfront.png resources/boxside.png

INSTALLS += target examples dbexamples impexamples resexamples

include(./VERSION)
DEFINES+=VERSION=\\\"$$VERSION\\\"

HEADERS += src/skyscraper.h \
           src/netcomm.h \
           src/xmlreader.h \
           src/settings.h \
           src/compositor.h \
           src/strtools.h \
           src/scraperworker.h \
           src/localdb.h \
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
           src/fxgamebox.h
           
SOURCES += src/main.cpp \
           src/skyscraper.cpp \
           src/netcomm.cpp \
           src/xmlreader.cpp \
           src/compositor.cpp \
           src/strtools.cpp \
           src/scraperworker.cpp \
           src/localdb.cpp \
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
           src/arcadedb.cpp \
           src/scripter.cpp \
           src/platform.cpp \
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
           src/fxgamebox.cpp
