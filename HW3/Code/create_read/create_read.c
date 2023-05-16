#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FILENAME "10gbfile.bin"
#define FILE_SIZE (10 * (1LL << 30)) // 10 GB
#define BUFFER_SIZE 4096
#ifndef O_DIRECT
#define O_DIRECT 040000
#endif


void create_file(char *buffer,int *file);
void read_file(char *buffer,int *file);


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
    file = open(FILENAME, O_RDONLY);
    if (file < 0)
    {
        printf("creating file \n");
        create_file(buffer,&file);
    }
    else
    {
        close(file);
    }
    // Read the file 10 times
    read_file(buffer,&file);
    return 0;
}

void create_file(char *buffer,int *file)
{
    // Create the 10GB file
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
    printf("file created\n");
    close(*file);
}

void read_file(char *buffer, int *file)
{
    for (int j = 0; j < 10; j++)
    {
        if (use_cache)
            *file = open(FILENAME, O_RDONLY);
        else 
            *file = open(FILENAME, O_RDONLY | O_DIRECT);
        if (*file < 0)
        {
            perror("Error opening file");
            exit(1);
        }

        ssize_t bytes_read;
        while ((bytes_read = read(*file,buffer, BUFFER_SIZE)) == BUFFER_SIZE)
        {
        
        }
        if (bytes_read < 0)
        {
            perror("Error reading file");
            exit(1);
        }
        close(*file);
    }
}
