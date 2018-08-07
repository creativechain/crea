#!/bin/bash

echo /tmp/core | tee /proc/sys/kernel/core_pattern
ulimit -c unlimited

# if we're not using PaaS mode then start creativecoind traditionally with sv to control it
if [[ ! "$USE_PAAS" ]]; then
  mkdir -p /etc/service/creativecoind
  cp /usr/local/bin/creativecoin-sv-run.sh /etc/service/creativecoind/run
  chmod +x /etc/service/creativecoind/run
  runsv /etc/service/creativecoind
elif [[ "$IS_TESTNET" ]]; then
  /usr/local/bin/testnetinit.sh
else
  /usr/local/bin/startpaascreativecoind.sh
fi
