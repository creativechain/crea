#!/bin/bash

export PATH="/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/usr/local/nvm/versions/node/v10.15.3/bin"

export NVM_DIR="/usr/local/nvm"
[ -s "$NVM_DIR/nvm.sh" ] && \. "$NVM_DIR/nvm.sh"


WITNESS=
PRIVATE_KEY=
NODE_UPDATER=
crea-witness update-price $PRIVATE_KEY $WITNESS --node $NODE_UPDATER
