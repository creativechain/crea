#!/bin/bash

if [[ ! -z "$HTTP_PORT" ]]; then
    HTTP_PORT=$HTTP_PORT
else
    HTTP_PORT=1996
fi

crea-witness update-price --node http://localhost:$HTTP_PORT $CREAD_PRIVATE_KEY $CREAD_WITNESS_NAME
