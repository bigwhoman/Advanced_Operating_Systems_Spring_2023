#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>
#include <string.h>

#define PAGE_SIZE 4096
#define REGION_SIZE 4096
void* ptr;
int* int_ptr;
int C_read = 1;
int D_read = 1;
// int ret;

void* thread_A() {
    int ret = posix_memalign(&ptr, REGION_SIZE, PAGE_SIZE);
    if (ret != 0 || ptr == NULL) {
        printf("Error: failed to allocate memory\n");
        exit(1);
    }
}

void* thread_B() {
    if (mprotect(ptr, PAGE_SIZE, PROT_READ | PROT_WRITE) != 0) {
        printf("Error: failed to set memory protection\n");
        exit(1);
    }

    // use the memory here
    int_ptr = (int*)ptr;
    *int_ptr = 12345;

    // set the protection of the memory to read only
    if (mprotect(ptr, PAGE_SIZE, PROT_READ) != 0) {
        printf("Error: failed to set memory protection\n");
        exit(1);
    }
}

void* thread_C() {
    if (C_read)
        for (int i = 0;i < 100;i++) {
            printf("C : %d", *int_ptr);
        }
    else {
        for (int i = 0;i < 100;i++) {
            *int_ptr = i;
        }
    }
}

void* thread_D() {
    if (D_read)
         for (int i = 0;i < 100;i++) {
            printf("D : %d", *int_ptr);
        }
    else {
        for (int i = 0;i < 100;i++) {
            *int_ptr = i;
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
    printf("this is C_write : %d, and D_write : %d\n",C_read,D_read);
    // allocate a page size of 4kB with malloc
    // printf("this is the process : %d", getpid());
    pthread_t A;
    pthread_t B;
    pthread_t C;
    pthread_t D;

    pthread_create(&A, NULL, thread_A, NULL);
    sleep(1);
    pthread_create(&B, NULL, thread_B, NULL);
    sleep(1);
    pthread_create(&C, NULL, thread_C, NULL);
    pthread_create(&D, NULL, thread_D, NULL);

    pthread_join(A, NULL);
    pthread_join(B, NULL);
    pthread_join(C, NULL);
    pthread_join(D, NULL);
    free(ptr);

    return 0;
}