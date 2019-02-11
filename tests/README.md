# Automated Testing Documentation

## To Create Test Environment Container

From the root of the repository:

    docker build --rm=false \
        -t creainc/ci-test-environment:latest \
        -f tests/scripts/Dockerfile.testenv .

## To Run The Tests

(Also in the root of the repository.)

    docker build --rm=false \
        -t creainc/crea-test \
        -f Dockerfile.test .

## To Troubleshoot Failing Tests

    docker run -ti \
        creainc/ci-test-environment:latest \
        /bin/bash

Then, inside the container:

(These steps are taken from `/Dockerfile.test` in the
repository root.)

    git clone https://github.com/creativechain/crea.git \
        /usr/local/src/crea
    cd /usr/local/src/crea
    git checkout <branch> # e.g. 123-feature
    git submodule update --init --recursive
    mkdir build
    cd build
    cmake \
        -DCMAKE_BUILD_TYPE=Debug \
        -DBUILD_CREA_TESTNET=ON \
        -DLOW_MEMORY_NODE=OFF \
        -DCLEAR_VOTES=ON \
        ..
    make -j$(nproc) chain_test
    ./tests/chain_test
    cd /usr/local/src/crea
    doxygen
    programs/build_helpers/check_reflect.py
