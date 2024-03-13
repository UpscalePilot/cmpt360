#!/bin/bash

clear
make clean
make all

current_dir=$(pwd)
pSimulator="${current_dir::-3}"

newProc="newProc"
newProcPath="$pSimulator$newProc"
newProcB="newProcB"
newProcPathB="$pSimulator$newProcB"

if [ ! -d "$newProcPathB" ]; then
    mkdir -p "$newProcPathB"
fi

current_date=$(date +%m-%d-%Y)

cp -r "$newProcPathB"/* "$newProcPath"
./bin/fifo
./bin/plotData log-$current_date.txt
rm "$pSimulator/log/log-$current_date.txt"



cp -r "$newProcPathB"/* "$newProcPath"
./bin/sjf
./bin/plotData log-$current_date.txt
rm "$pSimulator/log/log-$current_date.txt"



cp -r "$newProcPathB"/* "$newProcPath"
./bin/rr
./bin/plotData log-$current_date.txt
rm "$pSimulator/log/log-$current_date.txt"


cp -r "$newProcPathB"/* "$newProcPath"
./bin/mlfq
./bin/plotData log-$current_date.txt
rm "$pSimulator/log/log-$current_date.txt"

make clean