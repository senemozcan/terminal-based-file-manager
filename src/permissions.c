#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include "../include/logger.h"

int set_file_permissions(int argc, char *argv[]) {
  // Ensures both permission and file name are provided
  if (argc < 3) {
    fprintf(stderr, "Error: Both permission and file name must be provided.\n");
    return 1;  
  }

  // Gets the permission and file name from arguments
  char *permission = argv[argc - 2];
  char *file_name = argv[argc - 1];

  // Checks if the file exists
  struct stat st;
  if (stat(file_name, &st) == -1) {
    fprintf(stderr, "Error: File '%s' does not exist.\n", file_name);
    return 1;  // Exit if the file doesn't exist
  }

  // Ensures the permission is exactly 3 digits
  if (strlen(permission) != 3) {
    fprintf(stderr, "Error: Permission must be exactly 3 digits long.\n");
    return 1;  
  }
  
  // Checks if the permission digits are in the valid octal range (0-7)
  if (permission[0] < '0' || permission[0] > '7' ||
      permission[1] < '0' || permission[1] > '7' ||
      permission[2] < '0' || permission[2] > '7') {
    fprintf(stderr, "Error: Permission must be in octal format (0-7).\n");
    return 1;  
  }

  // Converts the permission string to an octal number
  int permission_code = strtol(permission, NULL, 8);

  // Changes the file's permissions
  if (chmod(file_name, permission_code) == -1) {
    perror("Error: Permission change failed");
    return 1;  
  }

  // Prints success message and log the result
    
}
