#!/bin/bash
{
    LATEST_URL='https://api.github.com/repos/muldjord/skyscraper/releases/latest'
    LATEST=`wget -q -O - $LATEST_URL | grep '"tag_name":' | sed -E 's/.*"([^"]+)".*/\1/'`
    VERSION_FILE='VERSION'

    handle_error () {
        local EXITCODE=$?
        local ACTION=$1
        rm -f "$VERSION_FILE"
        echo "--- Failed to $ACTION Skyscraper v.${LATEST}, exiting with code $EXITCODE ---"
        exit $EXITCODE
    }

    prep () {
        if [[ ! $(which qmake) ]]
        then
          handle_error "find 'qmake' in your PATH. Did you install QT?"
        fi

        if [[ ! $(which wget) ]]
        then
          handle_error "find 'wget' in your PATH. Did you install wget?"
        fi

        if [ ! -f "$VERSION_FILE" ]
        then
            echo "VERSION=0.0.0" > "$VERSION_FILE"
        fi

        source "$VERSION_FILE"
    }

    running_mac_os () {
      [[ "$OSTYPE" == "darwin"* ]] && return
      false
    }

    fetch_latest () {
        echo "--- Fetching Skyscraper v.$LATEST ---"
        wget -N https://github.com/muldjord/skyscraper/archive/${LATEST}.tar.gz || handle_error "fetch"
    }

    untar_latest () {
      echo "--- Unpacking ---"
      command="tar xvzf ${LATEST}.tar.gz --strip-components 1"
      # BSD tar doesn't recognize --overwrite, and overwrites by default
      if [[ ! running_mac_os ]]
      then
        command="$command --overwrite"
      fi
      $command || handle_error "unpack"
      rm ${LATEST}.tar.gz
    }

    clean_out_old_build () {
      echo "--- Cleaning out old build if one exists ---"
      # don't attempt 'make clean' if a Makefile doesn't yet exist
      if [[ -e Makefile ]]
      then
        make --ignore-errors clean
      fi
      rm -f .qmake.stash
    }

    generate_makefile () {
      echo "--- Generating Makefile with Qmake ---"
      qmake || handle_error "makefile"
    }

    build_skyscraper () {
      echo "--- Building Skyscraper v.$LATEST ---"

      # On macOS if the version file is in the same location as the source,
      # it will be treated as a source file and fail the build. Rename the
      # file and then rename it back, regardless of build status.
      if [[ running_mac_os ]]
      then
        mv "$VERSION_FILE" "${VERSION_FILE}.bak"
      fi

      make -j$(nproc)
      result=$?

      if [[ running_mac_os ]]
      then
        mv "${VERSION_FILE}.bak" "$VERSION_FILE" 
      fi

      if [[ ! $result -eq 0 ]]
      then
        handle_error "build"
      fi
    }

    install_latest () {
        echo "--- Installing Skyscraper v.$LATEST ---"
        echo "NOTE: Installation is done with sudo, you may be prompted for a password"
        sudo make install || handle_error "install"
        echo "--- Skyscraper has been updated to v.$LATEST ---"
    }

    prep

    if [ $LATEST != $VERSION ]
    then
      fetch_latest
      untar_latest
      clean_out_old_build
      generate_makefile
      build_skyscraper
      install_latest
    else
      echo "--- Skyscraper is already the latest version, exiting ---"
      echo "You can force a reinstall by removing the '$VERSION_FILE' file by"
      echo "running 'rm $VERSION_FILE'. Then rerun ./update_skyscraper.sh afterwards."
    fi
    exit
}
