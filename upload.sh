#!/bin/sh

## script to upload compiled code to an omega

if [ "$1" == "" ]; then
	echo "ERROR: expecting Omega IP address or mdns name"
	exit
fi

rsync -va omega/www/ root@"$1":/www/apps/oos-app-qwiic/
