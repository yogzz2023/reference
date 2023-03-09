#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

int main() {
    // Define the source and destination directories
    char src_dir[] = "/home/yogesh/Music/source";
    char dest_dir[] = "/home/yogesh/Music/destination";
    
    // Open the source directory
    DIR *dir = opendir(src_dir);
    if (dir == NULL) 
    {
        perror("opendir");
        exit(EXIT_FAILURE);
    }
    
    // Create the destination directory if it doesn't exist
    if (mkdir(dest_dir, 0755) == -1) 
    {
        if (errno != EEXIST) 
        {
            perror("mkdir");
            exit(EXIT_FAILURE);
        }
    }
    
    // Loop indefinitely, checking for new files in the source directory
    while (1) {
        // Read the contents of the source directory
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) 
        {
            // Ignore "." and ".." directories
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) 
            {
                continue;
            }
            
            // Construct the full path of the source file
            char src_path[BUFFER_SIZE];
            snprintf(src_path, BUFFER_SIZE, "%s/%s", src_dir, entry->d_name);
            
            // Check if the file is a regular file
            struct stat st;
            if (stat(src_path, &st) == -1) 
            {
                perror("stat");
                continue;
            }
            if (!S_ISREG(st.st_mode)) 
            {
                continue;
            }
            
            // Construct the full path of the destination file
            char dest_path[BUFFER_SIZE];
            snprintf(dest_path, BUFFER_SIZE, "%s/%s", dest_dir, entry->d_name);
            
            // Open the source file
            int src_fd = open(src_path, O_RDONLY);
            if (src_fd == -1) {
                perror("open");
                continue;
            }
            
            // Open the destination file
            int dest_fd = open(dest_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (dest_fd == -1) {
                perror("open");
                close(src_fd);
                continue;
            }
            
            // Transfer the file contents to the destination file
            char buf[BUFFER_SIZE];
            ssize_t bytes_read;
            while ((bytes_read = read(src_fd, buf, BUFFER_SIZE)) > 0) {
                if (write(dest_fd, buf, bytes_read) != bytes_read) {
                    perror("write");
                    break;
                }
            }
            
            // Close the files
            close(src_fd);
            close(dest_fd);
            
            // Remove the source file
            if (unlink(src_path) == -1) {
                perror("unlink");
            }
        }
        
        // Reset the directory stream
        rewinddir(dir);
        
        // Wait for a short period of time before checking for new files again
        sleep(1);
    }
    
    // Close the directory
    closedir(dir);
    
    return 0;
}
