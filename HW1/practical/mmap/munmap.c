#include <sys/types.h>
#include <sys/mman.h>
#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define PAGE_SIZE 4096

void* ptr;
int* int_ptr;
int C_read = 1;
int D_read = 1;
char* exchanged;
int fd = -1;
// int ret;

void* thread_A() {
    if ((fd = open("/dev/zero", O_RDWR, 0)) == -1)
        err(1, "open");
    exchanged = (char*)mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, fd, 0);
}

void* thread_B() {
    munmap(exchanged, 4096);
}

void* thread_C() {
    if (C_read)
        for (int i = 0;i < 100;i++) {
            printf("%s", exchanged);
            fflush(stdout);
            // sleep(0.3);
        }
    else {
        for (int i = 0;i < 100;i++) {
            strcpy(exchanged, "hello world C");
        }
    }
}


void* thread_D() {
    if (D_read)
        for (int i = 0;i < 100;i++) {
            printf("%s", exchanged);
            fflush(stdout);
            // sleep(0.3);
        }
    else {
        for (int i = 0;i < 100;i++) {
            strcpy(exchanged, "hello world D");
        }
    }
}


int main(int argc, char** argv) {
    if (argv[1] != NULL)
        if (strcmp(argv[1], "C_write") == 0)
            C_read = 0;
        else if (strcmp(argv[1], "D_write") == 0)
            D_read = 0;

    if (argv[2] != NULL)
        if (strcmp(argv[2], "C_write") == 0)
            C_read = 0;
        else if (strcmp(argv[2], "D_write") == 0)
            D_read = 0;
    printf("this is C_write : %d, and D_write : %d\n", C_read, D_read);
    pthread_t A;
    pthread_t B;
    pthread_t C;
    pthread_t D;

    pthread_create(&A, NULL, thread_A, NULL);
    pthread_join(A, NULL);
    pthread_create(&C, NULL, thread_C, NULL);
    pthread_create(&D, NULL, thread_D, NULL);
    pthread_create(&B, NULL, thread_B, NULL);
    pthread_join(B, NULL);
    sleep(1);
    pthread_join(C, NULL);
    pthread_join(D, NULL);
    // free(ptr);
    // munmap(anon, 4096);
    // munmap(exchanged, 4096);
    close(fd);
    return (EXIT_SUCCESS);
}