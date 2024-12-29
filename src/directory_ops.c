#include <stdio.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "../include/logger.h"

// This function returns a string representing the file type based on the given code.
char *get_file_type_string(int code) {
  if (code == DT_UNKNOWN) {
    return "Unknown";
  } else if (code == DT_FIFO) {
    return "Fifo";
  } else if (code == DT_CHR) {
    return "Chr";
  } else if (code == DT_DIR) {
    return "Directory";
  } else if (code == DT_BLK) {
    return "Blk";
  } else if (code == DT_REG) {
    return "File";
  } else if (code == DT_LNK) {
    return "Link";
  } else if (code == DT_SOCK) {
    return "Sock";
  } else if (code == DT_WHT) {
    return "Wht";
  } else {
    fprintf(stderr, "Undefined file type code: %d\n", code);
    return "";
  }
}

// This function converts file permissions from a code into a string representation.
char *get_permission_string(int code) {
  char *result = malloc(sizeof(char) * 10);  // Allocates memory for the result string.
  snprintf(result, 10, "%c%c%c%c%c%c%c%c%c",
    (code & S_IRUSR) ? 'r' : '-',
    (code & S_IWUSR) ? 'w' : '-',
    (code & S_IXUSR) ? 'x' : '-',
    (code & S_IRGRP) ? 'r' : '-',
    (code & S_IWGRP) ? 'w' : '-',
    (code & S_IXGRP) ? 'x' : '-',
    (code & S_IROTH) ? 'r' : '-',
    (code & S_IWOTH) ? 'w' : '-',
    (code & S_IXOTH) ? 'x' : '-');
  return result;
}

// This function lists the contents of a directory, showing file type, permissions, size, and name.
int dir_list(int argc, char *argv[]) {
  char *dir_name = ".";  // Default to the current directory.
  if (argc > 1) dir_name = argv[1];  // If a directory is passed as argument, use it.

  DIR *dir_ptr = opendir(dir_name);
  if (dir_ptr == NULL) {
    perror("Error: Failed to open directory.\n");
    return 1;
  }

  struct dirent *read_dir;
  struct stat *stat_buffer = malloc(sizeof(struct stat));
  char result[500];
  char path_buffer[500] = "";
  char *permission;

  // Loops through the directory contents and print details for each file.
  while ((read_dir = readdir(dir_ptr)) != NULL) {
    strcpy(path_buffer, "");
    strcat(path_buffer, dir_name);
    strcat(path_buffer, "/");
    strcat(path_buffer, read_dir->d_name);
    stat(path_buffer, stat_buffer);
    char *file_type = get_file_type_string(read_dir->d_type);
    permission = get_permission_string(stat_buffer->st_mode);
    snprintf(result, sizeof(result), "%-12s %s %12d %s\n",
        file_type,
        permission,
        stat_buffer->st_size,
        read_dir->d_name);

    printf("%s", result);
    print_log("dir_list", result);  // Logs the result.
  }
  if (closedir(dir_ptr) == -1) {
    perror("Error: Failed to close directory.\n");
    return 1;
  }
  return 0;
}

// This function creates a new directory.
int create_dir(int argc, char *argv[]) {
  if (argc < 2) {
    perror("Error: Folder name must be given.");
    return 1;
  }

  char *folder_name = argv[argc - 1];

  if (mkdir(folder_name, 0755) == -1) {
    if (errno == EEXIST) {
      printf("Error: Directory '%s' already exists.\n", folder_name);
      return EXIT_FAILURE;
    } else {
      perror("Error: Failed to create directory.");
      return EXIT_FAILURE;
    }
  }

  char output[500];
  snprintf(output, sizeof(output), "Folder '%s' created successfully!\n", folder_name);
  printf("%s", output);
  print_log("create_dir", output);  // Log success.
  return 0;
}

// This function removes a directory.
int remove_dir(int argc, char *argv[]) {
  if (argc < 2) {
    perror("Error: Folder name must be given.");
    return 1;
  }

  char *folder_name = argv[argc - 1];

  if (rmdir(folder_name) == -1) {
    if (errno == ENOENT) {
      printf("Error: Directory '%s' does not exist.\n", folder_name);
    } else if (errno == ENOTDIR) {
      printf("Error: '%s' is not a directory.\n", folder_name);
    } else {
      perror("Error: Failed to remove directory.");
      exit(EXIT_FAILURE);
    }
  }

  char output[500];
  snprintf(output, sizeof(output), "Directory '%s' removed successfully!\n", folder_name);
  printf("%s", output);
  print_log("remove_dir", output);  // Log success.
  return 0;
}
