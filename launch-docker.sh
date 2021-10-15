#!/bin/bash

set -euxo pipefail
cd "$(dirname "$0")"

#docker run --rm -it ytaki0801/flex-bison /bin/sh
docker run \
    --rm -it \
    --platform linux/amd64 \
    -v $PWD/AES:/home \
    exp6_aes /bin/bash
