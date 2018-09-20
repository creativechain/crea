#!/bin/bash

echo /tmp/core | tee /proc/sys/kernel/core_pattern
ulimit -c unlimited

# if we're not using PaaS mode then start cread traditionally with sv to control it
if [[ ! "$USE_PAAS" ]]; then
  mkdir -p /etc/service/cread
  cp /usr/local/bin/creativecoin-sv-run.sh /etc/service/cread/run
  chmod +x /etc/service/cread/run
  runsv /etc/service/cread
elif [[ "$IS_TESTNET" ]]; then
  /usr/local/bin/testnetinit.sh
else
  /usr/local/bin/startpaascread.sh
fi
