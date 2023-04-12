gcc ./madvise.c -o madvise -pg
for VAR in {1..12}
do
    sudo perf stat -e cache-references,cache-misses,faults,page-faults,dTLB-loads,dTLB-load-misses,iTLB-loads,iTLB-load-misses,context-switches ./madvise --number_of_threads $VAR &> ./outputs/"thread_count_$VAR".txt
    sleep 1
done