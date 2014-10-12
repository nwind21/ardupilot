#!/bin/sh

echo "REMOVING PRIOR TGZ"
rm *.tgz
rm *.log

iterations=10
if [ $# -eq 1 -a $1 -gt 0 ]
then
  iterations=$1
fi

echo "RUNNING FOR: "$iterations


i=0
while [ $i -lt $iterations ]
do
   i=`expr $i + 1`
   echo "$(tput setaf 3)============== ITERATION " $i " BEGIN ========================$(tput sgr 0)"
   date
   ./autotest_n21.py --mode fly-quadcopter > flight.log
#   ./autotest_n21.py --mode fly-quadcopter
   grep -Hn "FAILED STEP: fly." flight.log
   if [ $? -eq 0 ]
   then
       echo "$(tput setaf 1)FAILED @ ITERATION " $i "$(tput sgr 0)"
       break
   fi
   cp flight.log ~/buildlogs/flight.log
   cp flight.log flight_$i.log
   tar -cvf buildlogs_$i.tgz ~/buildlogs
   rm ~/buildlogs/*
   sleep 3
   date
   echo "$(tput setaf 2)============== ITERATION " $i " END ========================$(tput sgr 0)"
done
