#!/bin/bash

CURRENT_DIR=$PWD
SRC_DIR=$PWD
BUILD_DIR=$SRC_DIR/build
DEPENDS_DIR=$BUILD_DIR/depends
DOWNLOAD_DIR=$DEPENDS_DIR/download
LIBRARIES_DIR=$DEPENDS_DIR/`gcc -print-multiarch`

GCC_VERSION="5.4.0"

FLAGS="-DCREA_STATIC_BUILD=ON -DCLEAR_VOTES=OFF -DENABLE_SMT_SUPPORT=FALSE -DENABLE_CONTENT_PATCHING=OFF -DCMAKE_BUILD_TYPE=Release"
PROCS=$(nproc)
MAKE_FLAGS=""

function printMsg {
    echo ""
    echo $1
}

function makeDir {
    DIRECTORY=$1
    if [ ! -d "$DIRECTORY" ]; then
        printMsg "Creating $DIRECTORY"
        mkdir -p $DIRECTORY
    fi
}

function checkGccVersion {
    local GCC_INSTALLED_VER="$(gcc -dumpversion)"
    if [ "$(printf '%s\n' "$GCC_VERSION" "$GCC_INSTALLED_VER" | sort -V | head -n1)" = "$GCC_VERSION" ]; then
        echo 0
    else
        echo 1
    fi
}

function buildSource {

    cd $BUILD_DIR

    cmake $FLAGS ..;

    export PERL_USE_UNSAFE_INC=1

    make -j$PROCS $MAKE_FLAGS
}

while test $# -gt 0; do
	case "$1" in
	    --clean)
	        printMsg "Cleaning..."
	        rm -rf $BUILD_DIR
	        mkdir $BUILD_DIR
	        shift
	        ;;
		--test)
		    printMsg "Building Testnet"
			FLAGS="$FLAGS -DBUILD_CREA_TESTNET=ON"
			MAKE_FLAGS="chain_test test_fixed_string plugin_test ./tests/chain_test ./tests/plugin_test ./programs/util/test_fixed_string"
			shift
			;;
		--procs)
			PROCS=$2
		    printMsg "Using $PROCS cores for building"
		    shift
		    shift
			;;
		--lm)
		    FLAGS="$FLAGS -DLOW_MEMORY_NODE=ON"
		    printMsg "Using Low Memory config for building"
		    shift
		    ;;
		--hf)
			rm -rf $BUILD_DIR/libraries/protocol/include/crea/protocol/hardfork.hpp
			shift
			;;
		*)

			;;
	esac
done

git submodule update --init --recursive

buildSource
