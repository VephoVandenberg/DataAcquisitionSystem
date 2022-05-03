#!/bin/sh
cd /opt/LiME/src && make

VARIABLE_TO_ANALYZE=$(ls /opt/LiME/src | grep .ko)

insmod $VARIABLE_TO_ANALYZE "path=/home/vepho/Working/Projects/UniversityProject/build/memory format=raw"
