#!/bin/bash
{
    LATEST="$(wget -q -O - "https://api.github.com/repos/muldjord/skyscraper/releases/latest")"
    LATEST=${LATEST##*tag_name\":\"} # strip everything before the version tag
    LATEST=${LATEST%%\"*} # strip everything after the version tag

    if [ ! -f VERSION ]
    then
	echo "VERSION=0.0.0" > VERSION
    fi
    source VERSION

    handle_error () {
	local EXITCODE=$?
	local ACTION=$1
	rm --force VERSION
	echo "--- Failed to $ACTION Skyscraper v.${LATEST}, exiting with code $EXITCODE ---"
	exit $EXITCODE
    }

    if [ $LATEST != $VERSION ]
    then
	echo "--- Fetching Skyscraper v.$LATEST ---"
	wget -N https://github.com/muldjord/skyscraper/archive/${LATEST}.tar.gz || handle_error "fetch"
	echo "--- Unpacking ---"
	tar xvzf ${LATEST}.tar.gz --strip-components 1 --overwrite || handle_error "unpack"
	rm ${LATEST}.tar.gz
	echo "--- Cleaning out old build if one exists ---"
	make --ignore-errors clean
	rm --force .qmake.stash
	qmake || handle_error "clean old"
	echo "--- Building Skyscraper v.$LATEST ---"
	make -j$(nproc) || handle_error "build"
	echo "--- Installing Skyscraper v.$LATEST ---"
	sudo make install || handle_error "install"
	echo "--- Skyscraper has been updated to v.$LATEST ---"
    else
	echo "--- Skyscraper is already the latest version, exiting ---"
	echo "You can force a reinstall by removing the VERSION file by running rm VERSION. Then rerun ./update_skyscraper.sh afterwards."
    fi
    exit
}
