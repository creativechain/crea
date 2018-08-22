#!/bin/bash

CURRENT_DIR=$PWD
SRC_DIR=$PWD
BUILD_DIR=$SRC_DIR/build
DEPENDS_DIR=$BUILD_DIR/depends
DOWNLOAD_DIR=$DEPENDS_DIR/download
INCLUDE_DIR=$DEPENDS_DIR/`uname -s`

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
    cmake -DCMAKE_CXX_FLAGS_RELEASE="-fpermissive -O3" -DCMAKE_BUILD_TYPE=Release ..
    make CXX=g++-5.4 CC=gcc-5.4 -j$(nproc) creativecoind
    make CXX=g++-5.4 CC=gcc-5.4 -j$(nproc) cli_wallet
}

makeDir $DOWNLOAD_DIR
git submodule update --init --recursive
buildBoost
buildOpenssl
buildSource
