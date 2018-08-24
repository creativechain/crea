#!/bin/bash

CURRENT_DIR=$PWD
SRC_DIR=$PWD
BUILD_DIR=$SRC_DIR/build
DEPENDS_DIR=$BUILD_DIR/depends
DOWNLOAD_DIR=$DEPENDS_DIR/download
INCLUDE_DIR=$DEPENDS_DIR/`uname -s`

GCC_VERSION="5.4.0"

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

function downloadDependency {
    CURRENT_DIR=$PWD
    cd $DOWNLOAD_DIR
    URL=$1
    FILE=$2
    OUTPUT_DIR=$3
    if [ ! -f $FILE ]; then
        printMsg "Downloading $FILE"
        wget -c $URL -O $FILE
    fi

    if [ ! -d $OUTPUT_DIR ]; then
        if [[ $FILE == *.tar.bz2 ]]; then
            tar xjf $FILE
        else
            tar -xzf $FILE
        fi
    fi

    cd $CURRENT_DIR
}

function buildBoost {
    local file=$DEPENDS_DIR/.boost
    if [ ! -f $file ]; then
        downloadDependency "https://sourceforge.net/projects/boost/files/boost/1.60.0/boost_1_60_0.tar.bz2/download" "boost_1_60_0.tar.bz2" "boost_1_60_0"
        printMsg "Building Boost 1.60.0"
        if [ ! checkGccVersion ]; then
            curl 'https://raw.githubusercontent.com/boostorg/multiprecision/f9c8f9ec091ad232c0a291904f7839d665d098e0/include/boost/multiprecision/cpp_int.hpp' \
            > $DOWNLOAD_DIR/boost_1_60_0/boost/multiprecision/cpp_int.hpp
        fi

        cd $DOWNLOAD_DIR/boost_1_60_0
        ./bootstrap.sh "--prefix=$INCLUDE_DIR"
        ./b2 install
        touch $file
     else
        echo "Boost already built..."
     fi
}

function buildOpenssl {
    local file=$DEPENDS_DIR/.openssl
    if [ ! -f $file ]; then
        downloadDependency "https://www.openssl.org/source/openssl-1.0.2o.tar.gz" "openssl-1.0.2o.tar.gz" "openssl-1.0.2o"
        printMsg "Building OpenSSL 1.0.2"
        cd $DOWNLOAD_DIR/openssl-1.0.2o
        ./config --prefix=$INCLUDE_DIR
        make
        make install
        touch $file
     else
        echo "OpenSSL already built..."
     fi
}

function buildSource {
    printMsg "Building source"
    cd $BUILD_DIR
    export BOOST_ROOT=$INCLUDE_DIR
    export OPENSSL_ROOT_DIR=$INCLUDE_DIR
    if [ ! checkGccVersion ]; then
        cmake -DCMAKE_CXX_FLAGS_RELEASE="-fpermissive -O3" -DENABLE_CONTENT_PATCHING=OFF -DLOW_MEMORY_NODE=ON -DCMAKE_BUILD_TYPE=Release ..
    else
        cmake -DCMAKE_CXX_FLAGS_RELEASE="-fpermissive -O3" -DCMAKE_BUILD_TYPE=Release ..
    fi

    make -j$(nproc) creativecoind
    make -j$(nproc) cli_wallet
}

makeDir $DOWNLOAD_DIR
git submodule update --init --recursive
buildBoost
buildOpenssl
buildSource
