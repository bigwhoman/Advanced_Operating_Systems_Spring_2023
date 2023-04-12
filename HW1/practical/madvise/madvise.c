#include <sys/types.h>
#include <sys/mman.h>
#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/sysinfo.h>

#define PAGE_SIZE 4096
#define NUMBER_OF_ITTERATIONS 100000

pthread_barrier_t barrier;
// int ret;

void* thread_function() {
    for (int i = 0; i < NUMBER_OF_ITTERATIONS; i++) {
        void* ptr;
        size_t size = 2 * PAGE_SIZE; // allocate two pages of memory
        int ret;

        // Allocate memory using mmap()
        ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
        if (ptr == MAP_FAILED) {
            perror("mmap failed");
            exit(EXIT_FAILURE);
        }

        // Use madvise() to advise the kernel about memory usage
        ret = madvise(ptr, size, MADV_SEQUENTIAL);
        if (ret == -1) {
            perror("madvise failed");
            exit(EXIT_FAILURE);
        }

        // Deallocate the memory using munmap()
        if (munmap(ptr, size) == -1) {
            perror("munmap failed");
            exit(EXIT_FAILURE);
        }
    }
    pthread_barrier_wait(&barrier);
}


int main(int argc, char** argv) {
    int number_of_threads = 1;

    for (int i = 0;i < argc;i++) {
        if (strcmp(argv[i], "--number_of_threads") == 0 && i < argc - 1) {
            // Extract the number of threads from the next argument
            number_of_threads = atoi(argv[i + 1]);
            printf("number of threads : %d\n", number_of_threads);  // Output: the number of threads
            if (number_of_threads > get_nprocs_conf()) {
                printf("You have chosen more threads than what you have \n exiting ...\n");
                exit(EXIT_FAILURE);
            }
            break;
        }
    }
    if (pthread_barrier_init(&barrier, NULL, number_of_threads) != 0) {
        perror("pthread_barrier_init failed");
        exit(EXIT_FAILURE);
    }
    pthread_t threads[number_of_threads];
    for (int i = 0; i < number_of_threads; i++) {
        if (pthread_create(&threads[i], NULL, thread_function, NULL) != 0) {
            perror("pthread_create failed");
            exit(EXIT_FAILURE);
        }
    }
    if (pthread_join(threads[0], NULL) != 0) {
        perror("pthread_join failed");
        exit(EXIT_FAILURE);
    }
    pthread_barrier_destroy(&barrier);
    return (EXIT_SUCCESS);
}