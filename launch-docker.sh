#!/bin/bash

set -euxo pipefail
cd "$(dirname "$0")"

docker-compose run aesdev

#docker run \
#    --rm -it \
#    --platform linux/amd64 \
#    -v $PWD/AES:/home \
#    exp6_aes /bin/bash
