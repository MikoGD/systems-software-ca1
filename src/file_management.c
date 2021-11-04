#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utils.h"

int transfer_files(char *dest_path, char *src_path)
{
  char *command = "mv -f ";
  size_t command_length = strlen(command);
  size_t dest_path_length = strlen(dest_path);
  size_t src_path_length = strlen(src_path);

  // +1 for space between src and dest
  char *execute = (char *)malloc((command_length + dest_path_length + src_path_length + 1) * sizeof(char));

  strcat(execute, command);
  strcat(execute, src_path);
  strcat(execute, " ");
  strcat(execute, dest_path);

  printf("execute: %s\n", execute);

  int status = system(execute);

  return status;
}

int backup_folder(char *backup_path, char *folder_to_backup_path)
{
  char *command = "zip -r -j ";
  char *datetime = get_local_datetime();
  size_t command_length = strlen(command);
  size_t dest_path_length = strlen(backup_path);
  size_t src_path_length = strlen(folder_to_backup_path);
  size_t datetime_length = strlen(datetime);

  // +4 for space between src and dest and length of .zip
  char *execute = (char *)calloc((command_length + dest_path_length + src_path_length + datetime_length + 4), sizeof(char));

  strcat(execute, command);
  strcat(execute, backup_path);
  strcat(execute, datetime);
  strcat(execute, ".zip");
  strcat(execute, " ");
  strcat(execute, folder_to_backup_path);

  printf("execute: %s\n", execute);

  int status = system(execute);

  return status;

  // return 0;
}