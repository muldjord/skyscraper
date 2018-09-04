#!/bin/bash

LATEST=`wget -q -O - "https://api.github.com/repos/muldjord/skyscraper/releases/latest" | grep '"tag_name":' | sed -E 's/.*"([^"]+)".*/\1/'`

if [ ! -f VERSION ]
then
    echo "VERSION=0.0.0" > VERSION
fi
source VERSION

if [ "$LATEST" != "$VERSION" ]
then
    echo "--- Fetching Skyscraper v.${LATEST} ---"
    wget -N https://github.com/muldjord/skyscraper/archive/${LATEST}.tar.gz
    if [ $? != 0 ]
    then
	exit
    fi
    echo "--- Unpacking ---"
    tar xvzf ${LATEST}.tar.gz --strip-components 1 --overwrite
    if [ $? != 0 ]
    then
	exit
    fi
    rm ${LATEST}.tar.gz
    echo "--- Cleaning out old build if one exists ---"
    make clean
    qmake
    if [ $? != 0 ]
    then
	exit
    fi
    echo "--- Building Skyscraper v.${LATEST} ---"
    make
    if [ $? != 0 ]
    then
	exit
    fi
    echo "--- Installing Skyscraper v.${LATEST} ---"
    sudo make install
    if [ $? != 0 ]
    then
	exit
    fi
    echo "--- Skyscraper has been updated to v.${LATEST} ---"
else
    echo "--- Skyscraper is already the latest version, exiting ---"
fi
