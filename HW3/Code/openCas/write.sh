if [ "$EUID" -ne 0 ]
then
  echo "Please run this script as root"
  exit
fi

echo -e "this is my partition, if you want yours change below\n" 
partition='5ba9ddfc-07de-4eda-baef-39de9b4798a9'
echo -e "----------------------------------- Write ------------------------------\n\n"
sudo sync; echo 3 > /proc/sys/vm/drop_caches
sudo time fio --filename=/mnt/$partition/file.txt --direct=1 --rw=write --bs=4M --ioengine=libaio --iodepth=8 --numjobs=1 --group_reporting --io_size=20G --size=20G --name=iops-test-job --eta-newline=1 --output=seq-write-20.txt