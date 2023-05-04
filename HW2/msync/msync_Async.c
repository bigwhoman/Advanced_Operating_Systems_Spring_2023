#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define FILE_SIZE 100
char *mmapped_file;

void *write_to_file(void *arg) {
    int thread_id = *((int *)arg);
    for (int i = 0; i < FILE_SIZE; i++) {
        mmapped_file[i] = (char)(65 + thread_id);
        msync(mmapped_file, FILE_SIZE, MS_ASYNC);
    }
    
    return NULL;
}

int main() {
    int fd = open("example_file", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(fd, FILE_SIZE) == -1) {
        perror("Error truncating file");
        exit(EXIT_FAILURE);
    }

    mmapped_file = mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mmapped_file == MAP_FAILED) {
        perror("Error mmaping file");
        exit(EXIT_FAILURE);
    }

    pthread_t thread1, thread2;
    int thread_id1 = 0, thread_id2 = 1;

    pthread_create(&thread1, NULL, write_to_file, (void *)&thread_id1);
    pthread_create(&thread2, NULL, write_to_file, (void *)&thread_id2);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    munmap(mmapped_file, FILE_SIZE);
    close(fd);
    return 0;
}
