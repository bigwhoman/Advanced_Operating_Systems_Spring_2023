#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>


#define FILENAME "10gbfile.bin"
#define FILE_SIZE 10000000000L
#define BUFFER_SIZE 1024 * 1024

void create_file(char *buffer,FILE* file);
void read_file(char *buffer,FILE* file);


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

    
    FILE *file;
    char buffer[BUFFER_SIZE];
    long long int i, j;

    // Check if the file already exists
    file = fopen(FILENAME, "r");
    if (!file)
    {
        create_file(buffer,file);
    }
    else
    {
        fclose(file);
    }
    // Read the file 10 times
    read_file(buffer,file);
    return 0;
}

void create_file(char *buffer,FILE* file)
{
    // Create the 10GB file
    file = open(FILENAME, "w");
    if (!file)
    {
        perror("Error creating file");
        exit(1);
    }

    // Write 10GB of data
    for (int i = 0; i < FILE_SIZE / BUFFER_SIZE; i++)
    {
        fwrite(buffer, 1, BUFFER_SIZE, file);
    }
    close(file);
}

void read_file(char *buffer, FILE* file)
{
    for (int j = 0; j < 10; j++)
    {
        if (use_cache)
            file = open(FILENAME, O_RDONLY);
        else 
            file = open(FILENAME, O_RDONLY | O_DIRECT);
        if (!file)
        {
            perror("Error opening file");
            exit(1);
        }

        while (read(file,buffer, BUFFER_SIZE) == BUFFER_SIZE)
        {
            //
        }
        close(file);
    }
}
