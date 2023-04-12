import re
import matplotlib.pyplot as plt
# Prompt user for input file path

def draw(x_list,y_list,test_parameter,directory):
    plt.plot(x_list, y_list)
    plt.ylabel(test_parameter)
    plt.xlabel('number of threads')
    # plt.show()
    plt.savefig(f'{directory}/{test_parameter}.png')
    plt.clf()

tests = []
num_of_threads = [i for i in range(1,13)]
for i in range(1,13):
    input_file = f'./outputs/thread_count_{i}.txt'
    # Read input from file
    with open(input_file, "r") as f:
        input_str = f.read()

    # Extract performance metrics using regular expressions
    cache_misses = int(re.search(r"([\d,]+) *cache-misses", input_str).group(1).replace(",", ""))
    page_faults = int(re.search(r"([\d,]+) *page-faults", input_str).group(1).replace(",", ""))
    dtlb_load_misses = int(re.search(r"([\d,]+) *dTLB-load-misses", input_str).group(1).replace(",", ""))
    itlb_load_misses = int(re.search(r"([\d,]+) *iTLB-load-misses", input_str).group(1).replace(",", ""))
    elapsed_time = float(re.search(r"([\d.]+) seconds time elapsed", input_str).group(1))

    metrics = {'cache_misses':cache_misses,'page_faults':page_faults,'dtlb_load_misses':dtlb_load_misses,'itlb_load_misses':itlb_load_misses,'elapsed_time':elapsed_time,'number_of_threads':i}
    tests.append(metrics)

cache_misses = [test['cache_misses'] for test in tests]
page_faults = [test['page_faults'] for test in tests]
dtlb_load_misses = [test['dtlb_load_misses'] for test in tests]
itlb_load_misses = [test['itlb_load_misses'] for test in tests]
elapsed_time = [test['elapsed_time'] for test in tests]

draw(num_of_threads,cache_misses,'cache misses','./test_results')
draw(num_of_threads,page_faults,'page faults','./test_results')
draw(num_of_threads,dtlb_load_misses,'dTLB misses','./test_results')
draw(num_of_threads,itlb_load_misses,'iTLB misses','./test_results')
draw(num_of_threads,elapsed_time,'elapsed time','./test_results')


