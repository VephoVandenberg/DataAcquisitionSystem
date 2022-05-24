#!/bin/sh

cd /opt/volatility/tools/linux/ && make

CURRENT_KERNEL_VERSION=$(uname -r)
CURRENT_KERNEL=$(basename System.map-${CURRENT_KERNEL_VERSION})


sudo zip /opt/volatility/volatility/plugins/overlays/linux/Current.zip \
     /opt/volatility/tools/linux/module.dwarf /boot/${CURRENT_KERNEL}


