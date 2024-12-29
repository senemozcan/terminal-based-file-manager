#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "../include/logger.h"

int create_file(int argc, char *argv[]) {
  if (argc < 2) {
    perror("Folder name must be given.");
    return 1;
  }

  char *file_name = argv[argc - 1];

  struct stat st;
  if (stat(file_name, &st) == 0) {
    perror("This file already exists.");
    return 1;
  }

  int fd = creat(file_name, 0644);
  if (fd == -1) {
    perror("Error: Failed to create file.");
    return 1;
  }

  char *successMessage = "File creation was successful!\n";
  printf("%s", successMessage);
  print_log("create_file", successMessage);
  close(fd);
  return 0;
}

int remove_file(int argc, char *argv[]) {
  if (argc < 2) {
    perror("File name must be given.");
    return 1;
  }

  char *file_name = argv[argc - 1];

  if (unlink(file_name) == -1) {
    perror("Error: Failed to remove the file.");
    return 1;
  }

  char *successMessage = "File removal was successful!\n";
  printf("%s", successMessage);
  print_log("remove_file", successMessage);
  return 0;
}

int copy_file(int argc, char *argv[]) {
  if (argc < 3) {
    perror("Source and destination file names must be given.");
    return 1;
  }

  char *source_file = argv[argc - 2];
  char *destination_file = argv[argc - 1];

  int src_fd = open(source_file, O_RDONLY);
  if (src_fd == -1) {
    perror("Source file open failed.");
    return 1;
  }

  int dest_fd = creat(destination_file, 0644);
  if (dest_fd == -1) {
    perror("Error: Destination file creation failed.");
    return 1;
  }

  char buffer[1000];
  int read_bytes;
  while ((read_bytes = read(src_fd, buffer, sizeof(buffer))) > 0) {
    if (write(dest_fd, buffer, read_bytes) == -1) {
      perror("Error: Copying file failed.");
      return 1;
    }
  }

  char *successMessage = "File copied successfully!\n";
  printf("%s", successMessage);
  print_log("copy_file", successMessage);
  close(src_fd);
  close(dest_fd);
  return 0;
}

int move_file(int argc, char *argv[]) {
  if (argc < 3) {
    perror("Source and destination file names must be given.");
    return 1;
  }

  char *source_file = argv[argc - 2];
  char *destination_file = argv[argc - 1];

  if (rename(source_file, destination_file) == -1) {
    perror("Error: File move failed.");
    return 1;
  }

  char *successMessage = "File moved successfully!\n";
  printf("%s", successMessage);
  print_log("move_file", successMessage);
  return 0;
}

int pring_file(int argc, char *argv[]) {
  if (argc < 2) {
    perror("Please, file name must be given.");
    return 1;
  }

  char *file_name = argv[argc - 1];

  int fd = open(file_name, O_RDONLY);
  if (fd == -1) {
    perror("Error: Failed to open the file.");
    return 1;
  }

  char buffer[1000];
  int read_bytes;
  while ((read_bytes = read(fd, buffer, sizeof(buffer))) > 0) {
    if (write(STDOUT_FILENO, buffer, read_bytes) == -1) {
      perror("Error: File read failed.");
      return 1;
    }
    print_log("pring_file", buffer);
  }

  close(fd);
  return 0;
}

int search_in_file(int argc, char *argv[]) {
  if (argc < 3) {
    perror("Search string and file name must be given.");
    return 1;
  }

  char *search_string = argv[argc - 2];
  char *file_name = argv[argc - 1];

  int fd = open(file_name, O_RDONLY);
  if (fd == -1) {
    perror("Error: Failed to open the file.");
    return 1;
  }

  char buffer[1000];
  int read_bytes;
  while ((read_bytes = read(fd, buffer, sizeof(buffer))) > 0) {
    if (strstr(buffer, search_string) != NULL) {
      if (write(STDOUT_FILENO, buffer, read_bytes) == -1) {
        perror("Error: Failed to read the file.");
        return 1;
      }
      print_log("search_in_file", buffer);
    }
  }

  close(fd);
  return 0;
}
