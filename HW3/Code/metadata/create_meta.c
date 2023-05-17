#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <uuid/uuid.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>


#define MAX_NAME_LENGTH 10
#define MIN_NAME_LENGTH 5
#define BLOCK_SIZE 512
#define BUFFER_SIZE 4096
#define OUTPUT_DIRECTORY "outputs"
#ifndef O_DIRECT
#define O_DIRECT 040000
#endif

void read_metadata(const char*);
void create_files();
void read_directory(const char*);



int use_cache = 1; // default is true
int verbose = 0;
struct timeval tv;

int main(int argc, char *argv[]) {
    
    // check if "--cache=false" is present in the command-line arguments
    int num_of_files = 1000;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--cache=false") == 0) {
            use_cache = 0;
        }
        if (strcmp(argv[i], "--verbose") == 0) {
            verbose = 1;
        }
        if (strncmp(argv[i], "--num_of_files=", 15) == 0) {
             num_of_files = atoi(argv[i] + 15);
        }
    }
    // do something based on the value of use_cache
    if (use_cache) {
        printf("Cache is enabled.\n");
    } else {
        printf("Cache is disabled.\n");
    }
    
    if(verbose){
        printf("using verbose mode \n");
    } else {
        printf("use --verbose to see full details \n");
    }

    if (num_of_files == 0){
        return 0;
    }

    srand(time(NULL));
    printf("\n---------------- writing file metadatas in output directory ----------------\n\n\n");

    if (gettimeofday(&tv, NULL) == -1) {
        perror("gettimeofday");
        return 1;
    }
    long time_start_create = tv.tv_usec;
    for (size_t i = 0; i < num_of_files; i++){
       create_files();
    }

    if (gettimeofday(&tv, NULL) == -1) {
        perror("gettimeofday");
        return 1;
    }
    long time_end_create = tv.tv_usec;

    printf("Done creating files and writing metadatas\n");
    printf("Total time for %d writes (usec): %ld\n",num_of_files,time_end_create - time_start_create);
    printf("Average time for 1 write (usec): %ld\n",(time_end_create - time_start_create)/num_of_files);



    printf("\n---------------- reading file metadatas in output directory ----------------\n\n\n");
    read_directory(OUTPUT_DIRECTORY);
    return 0;
}

void create_files(){
    const char *dir_name = OUTPUT_DIRECTORY;
    char *path;
    int fd;
    off_t offset;
    char *buffer;
    int name_length = MIN_NAME_LENGTH + rand() % (MAX_NAME_LENGTH - MIN_NAME_LENGTH + 1);

    char name[name_length + 1];

    // Seed the random number generator with the current time
    uuid_t uuid;
    char uuid_str[37];



    uuid_generate(uuid);
    uuid_unparse(uuid, uuid_str);
    
    // print_uuid(uuid);
    // Generate a random name consisting of NAME_LENGTH letters
    for (int i = 0; i < name_length; i++) {
        name[i] = 'a' + rand() % 26;
    }
    name[name_length] = '\0';
    path = malloc(strlen(dir_name) + name_length + 2);
    sprintf(path, "%s/%s", dir_name, name);


    if (posix_memalign((void **)&buffer, BLOCK_SIZE, BUFFER_SIZE) != 0) {
        perror("posix_memalign");
        return;
    }
    memcpy(buffer, uuid_str, sizeof(uuid_str)-1);
    // printf("Buffer contents: ");
    // for (size_t i = 0; i < sizeof(uuid_str)-1; i++) {
    //     printf("%c", buffer[i]);
    // }
    // printf("\n");
    // Open the file according to the cache parameter
    if (use_cache)
        fd = open(path, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);
    else
        fd = open(path, O_WRONLY | O_TRUNC | O_CREAT | O_DIRECT, S_IRUSR | S_IWUSR);

    if (fd == -1) {
        printf("Error opening file\n");
        return;
    }

    offset = lseek(fd, 0, SEEK_END);
    if (offset == -1) {
        printf("Error seeking to end of file\n");
        close(fd);
        return ;
    }

    // Write the metadata string to the end of the file
    if (write(fd, buffer, BUFFER_SIZE) == -1) {
        printf("Error writing metadata to file\n");
        close(fd);
        return;
    }
    free(buffer);
    // Close the file
    close(fd);
    if (verbose){
    printf("File %s new uuid is : %s \n",name,uuid_str);  
    printf("Random name: %s\n", name);
    printf("Metadata added successfully\n");
    }
    
}

void read_metadata(const char *filename){
    uuid_t uuid;
    char uuid_str[37];
    int fd;
    off_t offset;
    ssize_t nread;

    // Open the file for reading
    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return;
    }

    // Move the file position indicator to the end of the file
    offset = lseek(fd, 0, SEEK_END);
    if (offset == -1) {
        perror("lseek");
        close(fd);
        return;
    }

    // Move the file position indicator back by the size of the UUID
    offset = lseek(fd, -(off_t)sizeof(uuid), SEEK_CUR);
    if (offset == -1) {
        perror("lseek");
        close(fd);
        return;
    }

    // Read the UUID from the file
    nread = read(fd, uuid, sizeof(uuid));
    if (nread == -1 || nread != sizeof(uuid)) {
        perror("read");
        close(fd);
        return;
    }

    // Convert the UUID to a string
    uuid_unparse(uuid, uuid_str);

    // Print the UUID
    printf("UUID of file %s is %s\n", filename, uuid_str);

    // Close the file
    close(fd);
}

// void read_metadata(const char *filename){
//     uuid_t uuid;
//     char uuid_str[37];
//     int fd;
//     off_t offset;
//     ssize_t nread;
//     char *buffer;

//     // Open the file for reading
//     if (use_cache)
//         fd = open(filename, O_RDONLY, S_IRUSR | S_IWUSR);
//     else
//         fd = open(filename, O_RDONLY | O_DIRECT, S_IRUSR | S_IWUSR);
//     if (fd == -1) {
//         perror("open");
//         return;
//     }

//     if (posix_memalign((void **)&buffer, BLOCK_SIZE, BUFFER_SIZE) != 0) {
//         perror("posix_memalign");
//         close(fd);
//         return;
//     }

//     // Move the file position indicator to the beginning of the last block
//     offset = lseek(fd, -BLOCK_SIZE, SEEK_END);
//     if (offset == -1) {
//         perror("lseek");
//         close(fd);
//         free(buffer);
//         return;
//     }

//     // Read the entire block 
//     nread = read(fd, buffer, BLOCK_SIZE);
//     if (nread == -1 || nread != BLOCK_SIZE) {
//         perror("read");
//         close(fd);
//         free(buffer);
//         return;
//     }

//     // The UUID is stored at the end of the block, so extract it from there
//     memcpy(uuid, buffer + BLOCK_SIZE - sizeof(uuid), sizeof(uuid)); 
//     uuid_unparse(uuid, uuid_str);

//     // Print the UUID
//     if(verbose){
//         printf("UUID of file %s is %s\n", filename, uuid_str);
//     }

//     // Close the file and free the resources
//     close(fd);
//     free(buffer);
// }


void read_directory(const char *dirname) {
    DIR *dir;
    struct dirent *entry;
    int num_of_enties = 0;
    long total_time_read = 0;
    long time_start;
    long time_end;
    // Open the directory
    dir = opendir(dirname);
    if (dir == NULL) {
        perror("opendir");
        return;
    }

    // Iterate over the directory entries and read the UUID from each file
    while ((entry = readdir(dir)) != NULL) {
        // Skip the "." and ".." entries
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        num_of_enties ++;
        // Construct the full path to the file
        char path[PATH_MAX];
        snprintf(path, sizeof(path), "%s/%s", dirname, entry->d_name);

        // Read the UUID from the file
        if (gettimeofday(&tv, NULL) == -1) {
            perror("gettimeofday");
            return ;
        }
        time_start = tv.tv_usec;
        read_metadata(path);
        if (gettimeofday(&tv, NULL) == -1) {
            perror("gettimeofday");
            return ;
        }
        time_end = tv.tv_usec;
        total_time_read += (time_end - time_start);
    }
    printf("Done reading file metadatas \n");
    printf("Total time for %d reads (usec): %ld\n",num_of_enties,total_time_read);
    printf("Average time for 1 read (usec): %ld\n",total_time_read/num_of_enties);
    // Close the directory
    closedir(dir);
}