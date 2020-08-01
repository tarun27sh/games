#!/bin/env bash
make clean && make
#valgrind --tool=massif ./snake 2>/tmp/err.log 
#gdb ./snake 2>/tmp/err.log 
./snake 2>/tmp/err.log 
