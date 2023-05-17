if [ "$EUID" -ne 0 ]
then
  echo "Please run this script as root"
  exit
fi

echo -e "making sure you have uuid library...\n"

sudo apt install uuid-dev

gcc -g ./create_meta.c -o create_meta.o -luuid


echo -e "------------------------------- TEST WITHOUT BUFFER CACHE ---------------------------\n\n\n"
./create_meta.o --cache=false

sudo rm -rf ./outputs/*

echo -e "------------------------------- TEST WITH BUFFER CACHE ---------------------------\n\n\n"
./create_meta.o

sudo rm -rf ./outputs/*