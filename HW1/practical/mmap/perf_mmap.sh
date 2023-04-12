gcc ./mmap.c -o mmap -pg
sudo perf stat -e faults,page-faults,dTLB-loads,dTLB-load-misses,iTLB-loads,iTLB-load-misses,context-switches ./mmap
sleep 1
sudo perf stat -e faults,page-faults,dTLB-loads,dTLB-load-misses,iTLB-loads,iTLB-load-misses,context-switches ./mmap D_write
sleep 1
sudo perf stat -e faults,page-faults,dTLB-loads,dTLB-load-misses,iTLB-loads,iTLB-load-misses,context-switches ./mmap C_write D_write