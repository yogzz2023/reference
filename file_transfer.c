#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

int main(int argc, char* argv[])
{
    DIR* source_dir;
    struct dirent* source_file;
    char* source_dir_path = "/home/yogesh/Music/source";
    char* dest_dir_path = "/home/yogesh/Music/destination";

    // Open the source directory
    source_dir = opendir(source_dir_path);
    if (source_dir == NULL) {
        printf("Error: Could not open source directory.\n");
        return 1;
    }

    // Loop through all files in the source directory
    while ((source_file = readdir(source_dir)) != NULL) 
    {
        if (strcmp(source_file->d_name, ".") != 0 && strcmp(source_file->d_name, "..") != 0) {
            // Build the paths to the source and destination files
            char* source_file_path = malloc(strlen(source_dir_path) + strlen(source_file->d_name) + 2);
            sprintf(source_file_path, "%s/%s", source_dir_path, source_file->d_name);
            char* dest_file_path = malloc(strlen(dest_dir_path) + strlen(source_file->d_name) + 2);
            sprintf(dest_file_path, "%s/%s", dest_dir_path, source_file->d_name);

            // Open the source and destination files
            FILE* source_file = fopen(source_file_path, "rb");
            FILE* dest_file = fopen(dest_file_path, "wb");

            // Copy the contents of the source file to the destination file
            if (source_file != NULL && dest_file != NULL) 
            {
                int ch;
                while ((ch = fgetc(source_file)) != EOF) 
                {
                    fputc(ch, dest_file);
                }
                fclose(source_file);
                fclose(dest_file);
                printf("Copied %s to %s\n", source_file_path, dest_file_path);
            } else {
                printf("Error: Could not copy %s to %s\n", source_file_path, dest_file_path);
            }

            // Free memory used by the file paths
            free(source_file_path);
            free(dest_file_path);
        }
    }

    // Close the source directory
    closedir(source_dir);

    return 0;
}
