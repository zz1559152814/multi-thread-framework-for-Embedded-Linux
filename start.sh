#!/bin/bash
./main
nice -n -20 ./main
#ps | grep syslogd | awk '{print $1}' | head -1 | xargs kill -9
#ps | grep /opt/Qt | awk '{print $1}' | head -1 | xargs kill -9
