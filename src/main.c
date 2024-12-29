#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include "../include/directory_ops.h"
#include "../include/file_operations.h"
#include "../include/permissions.h"
#include "../include/logger.h"

int get_program_name(const char *str, const char *suffix) {
  size_t str_len = strlen(str);
  size_t suffix_len = strlen(suffix);

  return (str_len >= suffix_len) &&
         (!memcmp(str + str_len - suffix_len, suffix, suffix_len));
}

int main(int argc, char *argv[]) {
  if (argc < 1) {
    perror("No Argument\n");
    return 1;
  }

  if (get_program_name(argv[0], "listdir")) {
    return dir_list(argc, argv);
  }
  if (get_program_name(argv[0], "createdir")) {
    return create_dir(argc, argv);
  }
  if (get_program_name(argv[0], "removedir")) {
    return remove_dir(argc, argv);
  }
  if (get_program_name(argv[0], "createfile")) {
    return create_file(argc, argv);
  }
  if (get_program_name(argv[0], "removefile")) {
    return remove_file(argc, argv);
  }
  if (get_program_name(argv[0], "copyfile")) {
    return copy_file(argc, argv);
  }
  if (get_program_name(argv[0], "movefile")) {
    return move_file(argc, argv);
  }
  if (get_program_name(argv[0], "printfile")) {
    return pring_file(argc, argv);
  }
  if (get_program_name(argv[0], "searchfile")) {
    return search_in_file(argc, argv);
  }
  if (get_program_name(argv[0], "setpermission")) {
    return set_file_permissions(argc, argv);
  }

  perror("The program cannot be callded directly. Use soft links instead.\n");
  return 1;
}
