#!/bin/sh

## script to upload compiled code to an omega

if [ "$1" == "" ]; then
	echo "ERROR: expecting Omega IP address or mdns name"
	exit
fi

echo "Compiling go program"
GOOS=linux GOARCH=mipsle go build -o ./sparkfun-qwiic

if [ $? -eq 0 ]; then
	# thermal script
	rsync -va --progress sparkfun-qwiic root@"$1":/root/sparkfun-qwiic
	#ssh -t root@$1 'python3 /root/thermal/scan.py'
	ssh -t root@"$1" 'cd /root; ./sparkfun-qwiic'
fi
