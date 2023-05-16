#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FILENAME "10gbfile.bin"
#define FILE_SIZE (10 * (1LL << 30)) // 10 GB
#define BUFFER_SIZE 4096
#define NUM_OF_WRITE 10
#ifndef O_DIRECT
#define O_DIRECT 040000
#endif


void create_file(char *buffer,int *file);


int use_cache = 1; // default is true

int main(int argc, char *argv[])
{

    // check if "--cache=false" is present in the command-line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--cache=false") == 0) {
            use_cache = 0;
            break;
        }
    }
    // do something based on the value of use_cache
    if (use_cache) {
        printf("Cache is enabled.\n");
    } else {
        printf("Cache is disabled.\n");
    }

    
    char *buffer;
    int file = 0;

    // Allocate aligned memory for the buffer
    if (posix_memalign((void**)&buffer, 512, BUFFER_SIZE) != 0)
    {
        perror("Error allocating buffer");
        return 1;
    }
 
    // Check if the file already exists
    for (size_t i = 0; i < NUM_OF_WRITE; i++)
    {
        create_file(buffer,&file);
    }
    return 0;
}

void create_file(char *buffer,int *file)
{
    // Create the 10GB file
    if  (use_cache == 0) 
        *file = open(FILENAME, O_WRONLY | O_CREAT | O_DIRECT, 0644);
    else 
        *file = open(FILENAME, O_WRONLY | O_CREAT, 0644);
    if (*file < 0)
    {
        perror("Error creating file");
        exit(1);
    }

    // Write 10GB of data
    
    for (int i = 0; i < FILE_SIZE / BUFFER_SIZE; i++)
    {
        write(*file,buffer, BUFFER_SIZE);
    }
    // printf("file created\n");
    close(*file);
    if (remove(FILENAME) != 0)
    {
        perror("Error deleting file");
        exit(1);
    }
}
