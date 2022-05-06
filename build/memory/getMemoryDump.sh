#!/bin/sh
cd /opt/LiME/src && make

VARIABLE_TO_ANALYZE=$(basename lime-*.ko)

sudo insmod $VARIABLE_TO_ANALYZE "path=${1}memoryDump.memLinux format=lime"

sudo rmmod lime
