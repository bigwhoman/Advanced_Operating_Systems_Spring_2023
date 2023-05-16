#!/bin/bash

if [ "$EUID" -ne 0 ]
then
  echo "Please run this script as root"
  exit
fi

#compile the program and remove the file if it exists
gcc create_read.c -o create_read.o
gcc create_write.c -o create_write.o
sudo rm -rf '10gbfile.bin'
echo -e "-------------------------------- TESTING READ --------------------------\n\n\n\n"



echo -e "------------making the file------------- \n"
./create_read.o

echo -e "------------test with cache - drop cache------------- \n"
time ./create_read.o  
sleep 1

echo -e "------------test without cache - drop cache------------- \n"
sudo sync; echo 3 > /proc/sys/vm/drop_caches
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


echo -e "-------------------------------- TESTING WRITE --------------------------\n\n\n\n"

echo -e "------------test with cache - drop cache------------- \n"
time ./create_write.o  
sleep 1

echo -e "------------test without cache - drop cache------------- \n"
sudo sync; echo 3 > /proc/sys/vm/drop_caches
time ./create_write.o --cache=false
sleep 1

echo -e "------------test with cache - no drop cache------------- \n"
time ./create_write.o  
sleep 1

echo -e "------------test without cache - no drop cache------------- \n"
time ./create_write.o --cache=false
sleep 1


echo -e "----------------------- TEST DONE  :D ------------------ \n"