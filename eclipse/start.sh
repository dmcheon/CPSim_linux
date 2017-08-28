#!/bin/sh
cd ~/eclipse
cp design.xml ../Simulator/parser/.
cd workspace/$1
cp *.h ../../../Simulator/parser/.
cd ../../../Simulator/parser
make
./parser
cd ../engine
cp auto_generated_files/* .
make
torcs &
./real-time_simulator &
