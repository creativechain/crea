#!/bin/bash

CURRENT_DIR=$PWD
SRC_DIR=$PWD
BUILD_DIR=$SRC_DIR/build
DEPENDS_DIR=$BUILD_DIR/depends
DOWNLOAD_DIR=$DEPENDS_DIR/download
LIBRARIES_DIR=$DEPENDS_DIR/`uname -s`

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
        ./bootstrap.sh "--prefix=$LIBRARIES_DIR"
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
        ./config --prefix=$LIBRARIES_DIR
        make
        make install
        touch $file
     else
        echo "OpenSSL already built..."
     fi
}

function buildZlib {
    local file=$DEPENDS_DIR/.zlib
    if [ ! -f $file ]; then
        downloadDependency "https://zlib.net/zlib-1.2.11.tar.gz" "zlib-1.2.11.tar.gz" "zlib-1.2.11"
        printMsg "Building ZLib 1.2.11"
        cd $DOWNLOAD_DIR/zlib-1.2.11
        make distclean
        ./configure --prefix=$LIBRARIES_DIR
        make
        make install
        touch $file
     else
        echo "ZLib already built..."
     fi
}

function buildSnappy {
    local file=$DEPENDS_DIR/.snappy
    if [ ! -f $file ]; then
        downloadDependency "https://github.com/google/snappy/archive/1.1.7.tar.gz" "snappy-1.1.7.tar.gz" "snappy-1.1.7"
        printMsg "Building Snappy 1.1.7"
        cd $DOWNLOAD_DIR/snappy-1.1.7
        cmake -DCMAKE_INSTALL_PREFIX=$LIBRARIES_DIR
        make all install
        touch $file
     else
        echo "Snappy already built..."
     fi
}

function buildNCurses {
    local file=$DEPENDS_DIR/.ncurses
    if [ ! -f $file ]; then
        downloadDependency "https://ftp.gnu.org/pub/gnu/ncurses/ncurses-6.1.tar.gz" "ncurses-6.1.tar.gz" "ncurses-6.1"
        printMsg "Building NCurses 6.1"
        cd $DOWNLOAD_DIR/ncurses-6.1
        ./configure --prefix=$LIBRARIES_DIR
        make
        make install
        touch $file
     else
        echo "Ncurses already built..."
     fi
}

function buildReadline {

    local file=$DEPENDS_DIR/.readline
    if [ ! -f $file ]; then
        downloadDependency "http://git.savannah.gnu.org/cgit/readline.git/snapshot/readline-6.3.tar.gz" "readline-6.3.tar.gz" "readline-6.3"
        printMsg "Building Readline 6.3"
        cd $DOWNLOAD_DIR/readline-6.3
        ./configure --prefix=$LIBRARIES_DIR
        make
        make install
        touch $file
     else
        echo "Readline already built..."
     fi
}

function buildBZip2 {

    local file=$DEPENDS_DIR/.bzip2
    if [ ! -f $file ]; then
        downloadDependency "https://api.github.com/repos/enthought/bzip2-1.0.6/tarball/master" "bzip2-1.0.6.tar.gz" "bzip2-1.0.6"
        mkdir $DOWNLOAD_DIR/bzip2-1.0.6
        mv $DOWNLOAD_DIR/enthought-bzip2-1.0.6-288acf9/* $DOWNLOAD_DIR/bzip2-1.0.6/
        rm -rf $DOWNLOAD_DIR/enthought-bzip2-1.0.6-288acf9
        printMsg "Building BZip2 1.0.6"
        cd $DOWNLOAD_DIR/bzip2-1.0.6
        make install PREFIX=$LIBRARIES_DIR
        touch $file
     else
        echo "BZip2 already built..."
     fi

}

function buildSource {
    buildBoost
    buildOpenssl
    buildZlib
    buildSnappy
    buildNCurses
    buildReadline
    buildBZip2

    printMsg "Building source"
    cd $BUILD_DIR

    export BOOST_ROOT=$LIBRARIES_DIR
    export OPENSSL_ROOT_DIR=$LIBRARIES_DIR
    export ZLIB_ROOT=$LIBRARIES_DIR
    export SNAPPY_LIBRARIES=$LIBRARIES_DIR/lib
    export SNAPPY_INCLUDE_DIR=$LIBRARIES_DIR/include
    export CURSES_ROOT=$LIBRARIES_DIR
    export READLINE_ROOT=$LIBRARIES_DIR
    export BZIP2_ROOT=$LIBRARIES_DIR

    cmake -DCMAKE_CXX_FLAGS_RELEASE="-fpermissive -O3" -DENABLE_CONTENT_PATCHING=OFF -DLOW_MEMORY_NODE=ON -DSKIP_BY_TX_ID=ON -DCMAKE_BUILD_TYPE=Release ..

    export PERL_USE_UNSAFE_INC=1

    make -j$(nproc) creativecoind
    make -j$(nproc) cli_wallet
}

makeDir $DOWNLOAD_DIR
git submodule update --init --recursive

buildSource
