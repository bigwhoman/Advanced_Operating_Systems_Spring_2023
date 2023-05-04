gcc ./madvise.c -o madvise -pg
for VAR in {1..12}
do
sudo perf stat -e cache-references,cache-misses,faults,page-faults,dTLB-load-misses,dTLB-loads,dTLB-store-misses,dTLB-stores,iTLB-load-misses,iTLB-loads,dtlb_load_misses.miss_causes_a_walk,dtlb_load_misses.stlb_hit,dtlb_load_misses.walk_completed,dtlb_load_misses.walk_completed_1g,dtlb_load_misses.walk_completed_2m_4m,dtlb_load_misses.walk_completed_4k,dtlb_store_misses.miss_causes_a_walk,dtlb_store_misses.stlb_hit,dtlb_store_misses.walk_completed,dtlb_store_misses.walk_completed_1g,dtlb_store_misses.walk_completed_2m_4m,dtlb_store_misses.walk_completed_4k,itlb.itlb_flush,itlb_misses.miss_causes_a_walk,itlb_misses.stlb_hit,itlb_misses.walk_completed,itlb_misses.walk_completed_1g,itlb_misses.walk_completed_2m_4m,itlb_misses.walk_completed_4k,tlb_flush.dtlb_thread,tlb_flush.stlb_any,swiotlb:swiotlb_bounced,tlb:tlb_flush,context-switches ./madvise --number_of_threads $VAR
sleep 1
done
