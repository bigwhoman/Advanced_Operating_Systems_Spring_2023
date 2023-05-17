if [ "$EUID" -ne 0 ]
then
  echo "Please run this script as root"
  exit
fi

echo -e "this is my partition, if you want yours change below\n" 
partition='5ba9ddfc-07de-4eda-baef-39de9b4798a9'



echo -e "--------------------- Random Reading ---------------------------------\n\n"
sudo sync; echo 3 > /proc/sys/vm/drop_caches
sudo time fio --filename=/mnt/$partition/file.txt --random_distribution=zipf:1.2 --direct=1 --rw=randread --bs=4k --ioengine=libaio --iodepth=16 --numjobs=16 --group_reporting --size=20G --name=iops-test-job --eta-newline=1 --output=random-read.txt