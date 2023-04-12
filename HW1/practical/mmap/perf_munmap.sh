gcc ./munmap.c -o munmap -pg
sudo perf stat -e faults,page-faults,dTLB-loads,dTLB-load-misses,iTLB-loads,iTLB-load-misses,context-switches ./munmap
sleep 1
sudo perf stat -e faults,page-faults,dTLB-loads,dTLB-load-misses,iTLB-loads,iTLB-load-misses,context-switches ./munmap D_write
sleep 1
sudo perf stat -e faults,page-faults,dTLB-loads,dTLB-load-misses,iTLB-loads,iTLB-load-misses,context-switches ./munmap C_write D_write