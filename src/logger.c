#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>

void ensure_folder_exists(const char *folder_path) {
  char path[256];
  snprintf(path, sizeof(path), "%s", folder_path);
  char *p = path;

  // Iterates through the path and create directories
  for (p = path + 1; *p; p++) {
    if (*p == '/') {
      *p = '\0'; // Temporarily terminates the string at the current level
      if (mkdir(path, 0755) == -1 && errno != EEXIST) {
        perror("Error: Failed to create folder.");
        return;
      }
      *p = '/'; // Restores the slash
    }
  }

  // Creates the final directory
  if (mkdir(path, 0755) == -1 && errno != EEXIST) {
    perror("Error: Failed to create final folder.");
  }
}

void get_current_time(char *buffer, size_t size) {
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", local);
}

void print_log(char *op_type, char *text) {
  ensure_folder_exists("sp-logs");
  int fd = open("sp-logs/output.log", O_WRONLY | O_CREAT | O_APPEND, 0644);
  if (fd == -1) {
    perror("Error: Failed to open log file.");
    return;
  }

  char log_result[1000];
  char timestamp[100];
  get_current_time(timestamp, sizeof(timestamp));

  if (text == NULL) {
    snprintf(log_result, sizeof(log_result), "%s %s\n", timestamp, op_type);
  } else {
    snprintf(log_result, sizeof(log_result), "%s %s - %s", timestamp, op_type, text);
  }

  if (write(fd, log_result, strlen(log_result)) == -1) {
    perror("Error: Faile dto write to log file.");
  }
}
