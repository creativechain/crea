#!/bin/bash

if [[ ! -z "$NODE_UPDATER" ]]; then
    NODE_UPDATER=$NODE_UPDATER
else
    NODE_UPDATER=https://nodes.creary.net
fi

crea-witness update-price --node $NODE_UPDATER $CREAD_PRIVATE_KEY $CREAD_WITNESS_NAME
