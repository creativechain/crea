FROM phusion/baseimage:0.9.19

#ARG CREAD_BLOCKCHAIN=https://example.com/cread-blockchain.tbz2

ARG CREA_STATIC_BUILD=ON
ENV CREA_STATIC_BUILD ${CREA_STATIC_BUILD}
ARG BUILD_STEP
ENV BUILD_STEP ${BUILD_STEP}

ENV LANG=en_US.UTF-8

ENV NVM_DIR /usr/local/nvm
ENV NODE_VERSION 10.15.3

RUN mkdir -p $NVM_DIR

RUN \
    apt-get update && \
    apt-get install -y \
	apt-transport-https \
        autoconf \
        automake \
        autotools-dev \
        bsdmainutils \
        build-essential \
	ca-certificates \
        cmake \
	curl \
        doxygen \
        gdb \
        git \
        libboost-all-dev \
        libyajl-dev \
        libreadline-dev \
        libssl-dev \
        libtool \
        liblz4-tool \
        ncurses-dev \
        pkg-config \
        python3 \
        python3-dev \
        python3-jinja2 \
        python3-pip \
        nginx \
        fcgiwrap \
        awscli \
        jq \
        wget \
        virtualenv \
        gdb \
        libgflags-dev \
        libsnappy-dev \
        zlib1g-dev \
        libbz2-dev \
        liblz4-dev \
        libzstd-dev \
    && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/* && \
    pip3 install gcovr

# Install nvm with node and npm
RUN curl https://raw.githubusercontent.com/creationix/nvm/v0.34.0/install.sh | bash \
    && . $NVM_DIR/nvm.sh \
    && nvm install $NODE_VERSION \
    && nvm alias default $NODE_VERSION \
    && nvm use default

ENV NODE_PATH $NVM_DIR/versions/node/v$NODE_VERSION/lib/node_modules
ENV PATH      $NVM_DIR/versions/node/v$NODE_VERSION/bin:$PATH

# Install Creary Tools Commands
RUN npm install -g @creativechain-fdn/creary-tools

ADD . /usr/local/src/crea

RUN \
    if [ "$BUILD_STEP" = "1" ] || [ ! "$BUILD_STEP" ] ; then \
    cd /usr/local/src/crea && \
    rm -rf /usr/local/src/crea/build && \
    git submodule update --init --recursive && \
    mkdir build && \
    cd build && \
    cmake \
        -DCMAKE_BUILD_TYPE=Release \
        -DBUILD_CREA_TESTNET=ON \
        -DLOW_MEMORY_NODE=OFF \
        -DCLEAR_VOTES=ON \
        -DSKIP_BY_TX_ID=ON \
        .. && \
    make -j$(nproc) chain_test test_fixed_string plugin_test && \
#    ./tests/chain_test && \
#    ./tests/plugin_test && \
#    ./programs/util/test_fixed_string && \
    cd /usr/local/src/crea && \
    doxygen && \
#    PYTHONPATH=programs/build_helpers \
#    python3 -m crea_build_helpers.check_reflect && \
#    programs/build_helpers/get_config_check.sh && \
    rm -rf /usr/local/src/crea/build ; \
    fi

RUN \
    if [ "$BUILD_STEP" = "2" ] || [ ! "$BUILD_STEP" ] ; then \
    cd /usr/local/src/crea && \
    git submodule update --init --recursive && \
    mkdir build && \
    cd build && \
    cmake \
        -DCMAKE_INSTALL_PREFIX=/usr/local/cread-testnet \
        -DCMAKE_BUILD_TYPE=Release \
        -DBUILD_CREA_TESTNET=ON \
        -DLOW_MEMORY_NODE=OFF \
        -DCLEAR_VOTES=ON \
        -DSKIP_BY_TX_ID=ON \
        -DENABLE_SMT_SUPPORT=ON \
        -DCREA_STATIC_BUILD=${CREA_STATIC_BUILD} \
        .. && \
    make -j$(nproc) chain_test test_fixed_string plugin_test && \
    make install && \
#    ./tests/chain_test && \
#    ./tests/plugin_test && \
#    ./programs/util/test_fixed_string && \
    cd /usr/local/src/crea && \
    doxygen && \
#    PYTHONPATH=programs/build_helpers \
#    python3 -m crea_build_helpers.check_reflect && \
#    programs/build_helpers/get_config_check.sh && \
    rm -rf /usr/local/src/crea/build ; \
    fi

RUN \
    if [ "$BUILD_STEP" = "1" ] || [ ! "$BUILD_STEP" ] ; then \
    cd /usr/local/src/crea && \
    git submodule update --init --recursive && \
    mkdir build && \
    cd build && \
    cmake \
        -DCMAKE_BUILD_TYPE=Debug \
        -DENABLE_COVERAGE_TESTING=ON \
        -DBUILD_CREA_TESTNET=ON \
        -DLOW_MEMORY_NODE=OFF \
        -DCLEAR_VOTES=ON \
        -DSKIP_BY_TX_ID=ON \
        -DCHAINBASE_CHECK_LOCKING=OFF \
        .. && \
    make -j$(nproc) chain_test plugin_test && \
#    ./tests/chain_test && \
#    ./tests/plugin_test && \
    mkdir -p /var/cobertura && \
    gcovr --object-directory="../" --root=../ --xml-pretty --gcov-exclude=".*tests.*" --gcov-exclude=".*fc.*" --gcov-exclude=".*app*" --gcov-exclude=".*net*" --gcov-exclude=".*plugins*" --gcov-exclude=".*schema*" --gcov-exclude=".*time*" --gcov-exclude=".*utilities*" --gcov-exclude=".*wallet*" --gcov-exclude=".*programs*" --gcov-exclude=".*vendor*" --output="/var/cobertura/coverage.xml" && \
    cd /usr/local/src/crea && \
    rm -rf /usr/local/src/crea/build ; \
    fi

RUN \
    if [ "$BUILD_STEP" = "2" ] || [ ! "$BUILD_STEP" ] ; then \
    cd /usr/local/src/crea && \
    git submodule update --init --recursive && \
    mkdir build && \
    cd build && \
    cmake \
        -DCMAKE_INSTALL_PREFIX=/usr/local/cread-default \
        -DCMAKE_BUILD_TYPE=Release \
        -DLOW_MEMORY_NODE=ON \
        -DCLEAR_VOTES=ON \
        -DSKIP_BY_TX_ID=OFF \
        -DBUILD_CREA_TESTNET=OFF \
        -DCREA_STATIC_BUILD=${CREA_STATIC_BUILD} \
        .. \
    && \
    make -j$(nproc) && \
    make install && \
    cd .. && \
    ( /usr/local/cread-default/bin/cread --version \
      | grep -o '[0-9]*\.[0-9]*\.[0-9]*' \
      && echo '_' \
      && git rev-parse --short HEAD ) \
      | sed -e ':a' -e 'N' -e '$!ba' -e 's/\n//g' \
      > /etc/creadversion && \
    cat /etc/creadversion && \
    rm -rfv build && \
    mkdir build && \
    cd build && \
    cmake \
        -DCMAKE_INSTALL_PREFIX=/usr/local/cread-full \
        -DCMAKE_BUILD_TYPE=Release \
        -DLOW_MEMORY_NODE=OFF \
        -DCLEAR_VOTES=OFF \
        -DSKIP_BY_TX_ID=ON \
        -DBUILD_CREA_TESTNET=OFF \
        -DCREA_STATIC_BUILD=${CREA_STATIC_BUILD} \
        .. \
    && \
    make -j$(nproc) && \
    make install && \
    rm -rf /usr/local/src/crea ; \
    fi

RUN \
    apt-get remove -y \
        automake \
        autotools-dev \
        cmake \
        doxygen \
        dpkg-dev \
        libboost-all-dev \
        libc6-dev \
        libexpat1-dev \
        libgcc-5-dev \
        libhwloc-dev \
        libibverbs-dev \
        libicu-dev \
        libltdl-dev \
        libncurses5-dev \
        libnuma-dev \
        libopenmpi-dev \
        libpython-dev \
        libpython2.7-dev \
        libreadline-dev \
        libreadline6-dev \
        libstdc++-5-dev \
        libtinfo-dev \
        libtool \
        linux-libc-dev \
        m4 \
        make \
        python-dev \
        python2.7-dev \
        python3-dev \
    && \
    apt-get autoremove -y

RUN useradd -s /bin/bash -m -d /var/lib/cread cread

RUN mkdir /var/cache/cread && \
    chown cread:cread -R /var/cache/cread

# add blockchain cache to image
#ADD $CREAD_BLOCKCHAIN /var/cache/cread/blocks.tbz2

ENV HOME /var/lib/cread
RUN chown cread:cread -R /var/lib/cread

VOLUME ["/var/lib/cread"]

# http rpc service:
EXPOSE 1996
# ws rpc service:
EXPOSE 1886
# p2p service:
EXPOSE 1776

# add seednodes from documentation to image
ADD doc/seednodes.txt /etc/cread/seednodes.txt

# the following adds lots of logging info to stdout
ADD contrib/config-for-docker.ini /etc/cread/config.ini
ADD contrib/fullnode.config.ini /etc/cread/fullnode.config.ini
ADD contrib/fullnode.opswhitelist.config.ini /etc/cread/fullnode.opswhitelist.config.ini
ADD contrib/config-for-broadcaster.ini /etc/cread/config-for-broadcaster.ini
ADD contrib/config-for-ahnode.ini /etc/cread/config-for-ahnode.ini

# add normal startup script that starts via sv
ADD contrib/cread.run /usr/local/bin/crea-sv-run.sh
RUN chmod +x /usr/local/bin/crea-sv-run.sh

# add nginx templates
ADD contrib/cread.nginx.conf /etc/nginx/cread.nginx.conf
ADD contrib/healthcheck.conf.template /etc/nginx/healthcheck.conf.template

# add PaaS startup script and service script
ADD contrib/startpaascread.sh /usr/local/bin/startpaascread.sh
ADD contrib/pulltestnetscripts.sh /usr/local/bin/pulltestnetscripts.sh
ADD contrib/paas-sv-run.sh /usr/local/bin/paas-sv-run.sh
ADD contrib/sync-sv-run.sh /usr/local/bin/sync-sv-run.sh
ADD contrib/healthcheck.sh /usr/local/bin/healthcheck.sh
ADD contrib/witness-price-updater.sh /etc/cread/witness-price-updater.sh
RUN chmod +x /usr/local/bin/startpaascread.sh
RUN chmod +x /usr/local/bin/pulltestnetscripts.sh
RUN chmod +x /usr/local/bin/paas-sv-run.sh
RUN chmod +x /usr/local/bin/sync-sv-run.sh
RUN chmod +x /usr/local/bin/healthcheck.sh

# Add cron for update witness feed automatically
ADD contrib/crontab /etc/cread/crontab
RUN crontab /etc/cread/crontab

# new entrypoint for all instances
# this enables exitting of the container when the writer node dies
# for PaaS mode (elasticbeanstalk, etc)
# AWS EB Docker requires a non-daemonized entrypoint
ADD contrib/creadentrypoint.sh /usr/local/bin/creadentrypoint.sh
RUN chmod +x /usr/local/bin/creadentrypoint.sh
CMD /usr/local/bin/creadentrypoint.sh


