#!/bin/bash

VERSION=`cat /etc/creadversion`

if [[ "$IS_BROADCAST_NODE" ]]; then
  CREAD="/usr/local/cread-default/bin/cread"
elif [[ "$IS_AH_NODE" ]]; then
  CREAD="/usr/local/cread-default/bin/cread"
else
  CREAD="/usr/local/cread-full/bin/cread"
fi

chown -R cread:cread $HOME

# clean out data dir since it may be semi-persistent block storage on the ec2 with stale data
rm -rf $HOME/*

ARGS=""

# if user did pass in desired seed nodes, use
# the ones the user specified:
if [[ ! -z "$CREAD_SEED_NODES" ]]; then
    for NODE in $CREAD_SEED_NODES ; do
        ARGS+=" --p2p-seed-node=$NODE"
    done
fi

NOW=`date +%s`
CREAD_FEED_START_TIME=`expr $NOW - 1209600`

ARGS+=" --follow-start-feeds=$CREAD_FEED_START_TIME"

CREAD_PROMOTED_START_TIME=`expr $NOW - 604800`
ARGS+=" --tags-start-promoted=$CREAD_PROMOTED_START_TIME"

if [[ ! "$DISABLE_BLOCK_API" ]]; then
   ARGS+=" --plugin=block_api"
fi

# overwrite local config with image one
if [[ "$IS_BROADCAST_NODE" ]]; then
  cp /etc/cread/config-for-broadcaster.ini $HOME/config.ini
elif [[ "$IS_AH_NODE" ]]; then
  cp /etc/cread/config-for-ahnode.ini $HOME/config.ini
elif [[ "$IS_OPSWHITELIST_NODE" ]]; then
  cp /etc/cread/fullnode.opswhitelist.config.ini $HOME/config.ini
else
  cp /etc/cread/fullnode.config.ini $HOME/config.ini
fi

chown cread:cread $HOME/config.ini

cd $HOME

mv /etc/nginx/nginx.conf /etc/nginx/nginx.original.conf
cp /etc/nginx/cread.nginx.conf /etc/nginx/nginx.conf

# get blockchain state from an S3 bucket
echo cread: beginning download and decompress of s3://$S3_BUCKET/blockchain-$VERSION-latest.tar.lz4
finished=0
count=1
if [[ "$USE_RAMDISK" ]]; then
  mkdir -p /mnt/ramdisk
  mount -t ramfs -o size=${RAMDISK_SIZE_IN_MB:-51200}m ramfs /mnt/ramdisk
  ARGS+=" --shared-file-dir=/mnt/ramdisk/blockchain"
  # try five times to pull in shared memory file
  while [[ $count -le 5 ]] && [[ $finished == 0 ]]
  do
    rm -rf $HOME/blockchain/*
    rm -rf /mnt/ramdisk/blockchain/*
    if [[ "$IS_BROADCAST_NODE" ]]; then
      aws s3 cp s3://$S3_BUCKET/broadcast-$VERSION-latest.tar.lz4 - | lz4 -d | tar x --wildcards 'blockchain/block*' -C /mnt/ramdisk 'blockchain/shared*'
    elif [[ "$IS_AH_NODE" ]]; then
      aws s3 cp s3://$S3_BUCKET/ahnode-$VERSION-latest.tar.lz4 - | lz4 -d | tar x --wildcards 'blockchain/block*' 'blockchain/*rocksdb-storage*' -C /mnt/ramdisk 'blockchain/shared*'
    else
      aws s3 cp s3://$S3_BUCKET/blockchain-$VERSION-latest.tar.lz4 - | lz4 -d | tar x --wildcards 'blockchain/block*' -C /mnt/ramdisk 'blockchain/shared*'
    fi
    if [[ $? -ne 0 ]]; then
      sleep 1
      echo notifyalert cread: unable to pull blockchain state from S3 - attempt $count
      (( count++ ))
    else
      finished=1
    fi
  done
  chown -R cread:cread /mnt/ramdisk/blockchain
else
  while [[ $count -le 5 ]] && [[ $finished == 0 ]]
  do
    rm -rf $HOME/blockchain/*
    if [[ "$IS_BROADCAST_NODE" ]]; then
      aws s3 cp s3://$S3_BUCKET/broadcast-$VERSION-latest.tar.lz4 - | lz4 -d | tar x
    elif [[ "$IS_AH_NODE" ]]; then
      aws s3 cp s3://$S3_BUCKET/ahnode-$VERSION-latest.tar.lz4 - | lz4 -d | tar x
    else
      aws s3 cp s3://$S3_BUCKET/blockchain-$VERSION-latest.tar.lz4 - | lz4 -d | tar x
    fi
    if [[ $? -ne 0 ]]; then
      sleep 1
      echo notifyalert cread: unable to pull blockchain state from S3 - attempt $count
      (( count++ ))
    else
      finished=1
    fi
  done
fi
if [[ $finished == 0 ]]; then
  if [[ ! "$SYNC_TO_S3" ]]; then
    echo notifyalert cread: unable to pull blockchain state from S3 - exiting
    exit 1
  else
    echo notifycreadsync creadsync: shared memory file for $VERSION not found, creating a new one by replaying the blockchain
    if [[ "$USE_RAMDISK" ]]; then
      mkdir -p /mnt/ramdisk/blockchain
      chown -R cread:cread /mnt/ramdisk/blockchain
    else
      mkdir blockchain
    fi
    aws s3 cp s3://$S3_BUCKET/block_log-latest blockchain/block_log
    if [[ $? -ne 0 ]]; then
      echo notifycreadsync creadsync: unable to pull latest block_log from S3, will sync from scratch.
    else
      ARGS+=" --replay-blockchain --force-validate"
    fi
    touch /tmp/isnewsync
  fi
fi

# for appbase tags plugin loading
ARGS+=" --tags-skip-startup-update"

cd $HOME

if [[ "$SYNC_TO_S3" ]]; then
  touch /tmp/issyncnode
  chown www-data:www-data /tmp/issyncnode
fi

chown -R cread:cread $HOME/*

# let's get going
cp /etc/nginx/healthcheck.conf.template /etc/nginx/healthcheck.conf
echo server 127.0.0.1:1996\; >> /etc/nginx/healthcheck.conf
echo } >> /etc/nginx/healthcheck.conf
rm /etc/nginx/sites-enabled/default
cp /etc/nginx/healthcheck.conf /etc/nginx/sites-enabled/default
/etc/init.d/fcgiwrap restart
service nginx restart
exec chpst -ucread \
    $CREAD \
        --webserver-ws-endpoint=127.0.0.1:1996 \
        --webserver-http-endpoint=127.0.0.1:1996 \
        --p2p-endpoint=0.0.0.0:1776 \
        --data-dir=$HOME \
        $ARGS \
        $CREAD_EXTRA_OPTS \
        2>&1&
SAVED_PID=`pgrep -f p2p-endpoint`
echo $SAVED_PID >> /tmp/creadpid
mkdir -p /etc/service/cread
if [[ ! "$SYNC_TO_S3" ]]; then
  cp /usr/local/bin/paas-sv-run.sh /etc/service/cread/run
else
  cp /usr/local/bin/sync-sv-run.sh /etc/service/cread/run
fi
chmod +x /etc/service/cread/run
runsv /etc/service/cread
