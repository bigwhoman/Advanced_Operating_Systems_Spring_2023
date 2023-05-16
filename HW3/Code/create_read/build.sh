#!/bin/bash

if [ "$EUID" -ne 0 ]
then
  echo "Please run this script as root"
  exit
fi

gcc create_read.c -o create_read.o
gcc create_read.c -o create_read.o
echo "test with cache"

time create_read.o  

echo "Runtime: $runtime seconds"
