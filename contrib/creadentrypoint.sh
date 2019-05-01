#!/bin/bash

echo /tmp/core | tee /proc/sys/kernel/core_pattern
ulimit -c unlimited

# if we're not using PaaS mode then start cread traditionally with sv to control it
if [[ ! "$USE_PAAS" ]]; then
  mkdir -p /etc/service/cread
  cp /usr/local/bin/crea-sv-run.sh /etc/service/cread/run
  chmod +x /etc/service/cread/run
  runsv /etc/service/cread
elif [[ "$IS_TESTNET" ]]; then
  /usr/local/bin/pulltestnetscripts.sh
else
  /usr/local/bin/startpaascread.sh
fi


if [[ ! -z "$NODE_UPDATER" ]]; then
   sed 's/NODE_UPDATER=/NODE_UPDATER=${NODE_UPDATER}/g' /etc/cread/witness-price-updater.sh > /tmp/witness-price-updater
else
   NODE="https://nodes.creary.net"
   sed 's/NODE_UPDATER=/NODE_UPDATER=${NODE}g' /etc/cread/witness-price-updater.sh > /tmp/witness-price-updater

fi

sed 's/WITNESS=/WITNESS=${CREAD_WITNESS_NAME}/g' /tmp/witness-price-updater > /tmp/witness-price-updater2
sed 's/PRIVATE_KEY=/PRIVATE_KEY=${CREAD_PRIVATE_KEY}/g' /tmp/witness-price-updater2 > /usr/local/bin/witness-price-updater

chmod +x /usr/local/bin/witness-price-updater