#!/bin/bash

if [ "$EUID" -ne 0 ]
then
  echo "Please run this script as root"
  exit
fi

#compile the program
sudo rm -rf '10gbfile.bin'
gcc create_read.c -o create_read.o

echo -e "------------making the file------------- \n"
./create_read.o

echo -e "------------test with cache - drop cache------------- \n"
time ./create_read.o  
sleep 1

echo -e "------------test without cache - drop cache------------- \n"
sudo sync;echo 3 > /proc/sys/vm/drop_caches
time ./create_read.o --cache=false
sleep 1

echo -e "------------test with cache - no drop cache------------- \n"
time ./create_read.o  
sleep 1

echo -e "------------test without cache - no drop cache------------- \n"
time ./create_read.o --cache=false
sleep 1

echo -e "-----------removing the file------------\n"


sudo rm -rf '10gbfile.bin'
