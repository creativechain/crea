#!/bin/bash

echo /tmp/core | tee /proc/sys/kernel/core_pattern
ulimit -c unlimited

# Enable CRON
service cron start

# Enable price updater
cp /etc/cread/witness-price-updater.sh /usr/local/bin/witness-price-updater

if [[ ! -z "$NODE_UPDATER" ]]; then
   sed -i "s|NODE_UPDATER=|NODE_UPDATER=${NODE_UPDATER}|g" /usr/local/bin/witness-price-updater
else
   NODE="https://nodes.creary.net"
   sed -i 's|NODE_UPDATER=|NODE_UPDATER=${NODE}|g' /usr/local/bin/witness-price-updater

fi

sed -i "s|WITNESS=|WITNESS=${CREAD_WITNESS_NAME}|g" /usr/local/bin/witness-price-updater
sed -i "s|PRIVATE_KEY=|PRIVATE_KEY=${CREAD_PRIVATE_KEY}|g" /usr/local/bin/witness-price-updater

chmod +x /usr/local/bin/witness-price-updater

# Update price
/usr/local/bin/witness-price-updater

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

