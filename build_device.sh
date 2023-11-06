#!/bin/bash
sudo docker run --rm -ti --user "$(id -u):$(id -g)" \
 --privileged -v "/dev/bus/usb:/dev/bus/usb" \
 -v "$(realpath .):/app" ghcr.io/ledgerhq/ledger-app-builder/ledger-app-dev-tools:latest \
 /bin/bash -c "make load"

