#!/bin/bash

all_elapsed=0
for i in `seq 1 5`
do
	echo $i
	dt_start=$( date +"%s%3N" )
	./client ::1 8080 0 100
	dt_end=$( date +"%s%3N" )
	elapsed=$(( dt_end - dt_start ))
	all_elapsed=`expr $all_elapsed + $elapsed`
done

result=`expr $all_elapsed / 5`
echo "elapsed: " ${result}"[ms]"
