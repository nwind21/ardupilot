#!/bin/sh

/usr/local/bin/mavproxy.py --master=tcp:127.0.0.1:5760 --out=127.0.0.1:14550 --aircraft=test.ArduCopter --sitl=127.0.0.1:5501 --out=127.0.0.1:19550 --quadcopter --streamrate=5