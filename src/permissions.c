#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "permissions.h"

int lock_folder(char *folder_path)
{
  char *command = "chgrp 1001 \0";
  int folder_path_length = strlen(folder_path);
  int command_length = strlen(command);
  char *execute = (char *)malloc((folder_path_length + command_length) * sizeof(char));
  strcat(execute, command);
  strcat(execute, folder_path);
  int status = system(execute);

  return status;
}

int unlock_folder(char *folder_path)
{
  char *command = "chgrp 1002 \0";
  int folder_path_length = strlen(folder_path);
  int command_length = strlen(command);
  char *execute = (char *)malloc((folder_path_length + command_length) * sizeof(char));
  strcat(execute, command);
  strcat(execute, folder_path);
  int status = system(execute);

  return status;
}