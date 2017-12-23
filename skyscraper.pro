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
unix:examples.files=config.ini.example README.md config_artwork01.ini config_artwork02.ini config_artwork03.ini mameMap.csv
unix:dbexamples.path=/usr/local/etc/skyscraper/dbs
unix:dbexamples.files=dbs/priorities.xml.example dbs/README.md
unix:impexamples.path=/usr/local/etc/skyscraper/import
unix:impexamples.files=import/README.md import/definitions.dat.example1 import/definitions.dat.example2

INSTALLS += target examples dbexamples impexamples

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
           src/scripter.h
           
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
           src/scripter.cpp
