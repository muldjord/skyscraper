#!/bin/bash

LATEST=`curl --silent "https://api.github.com/repos/muldjord/skyscraper/releases/latest" | grep '"tag_name":' | sed -E 's/.*"([^"]+)".*/\1/'`

if [ ! -f VERSION ]
then
    echo "VERSION=0.0.0" > VERSION
fi
source VERSION

if [ "$LATEST" != "$VERSION" ]
then
    echo "--- Fetching Skyscraper v.${LATEST} ---"
    wget https://github.com/muldjord/skyscraper/archive/${LATEST}.tar.gz
    echo "--- Unpacking ---"
    tar xvzf ${LATEST}.tar.gz --strip-components 1
    echo "--- Cleaning out old build ---"
    make clean
    qmake
    echo "--- Building Skyscraper v.${LATEST} ---"
    make
    echo "--- Installing Skyscraper v.${LATEST} ---"
    sudo make install
    echo "--- Skyscraper has been updated to v.${LATEST} ---"
else
    echo "--- Skyscraper is already the latest release, exiting ---"
fi
