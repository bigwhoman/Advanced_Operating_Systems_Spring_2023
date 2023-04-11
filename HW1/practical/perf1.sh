gcc ./protect.c -o myprogram -pg
sudo perf stat -e faults,page-faults,dTLB-loads,dTLB-load-misses,iTLB-loads,iTLB-load-misses,context-switches ./myprogram
sleep 1
sudo perf stat -e faults,page-faults,dTLB-loads,dTLB-load-misses,iTLB-loads,iTLB-load-misses,context-switches ./myprogram D_write
sleep 1
sudo perf stat -e faults,page-faults,dTLB-loads,dTLB-load-misses,iTLB-loads,iTLB-load-misses,context-switches ./myprogram C_write D_write