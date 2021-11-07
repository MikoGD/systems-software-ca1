#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <syslog.h>
#include <time.h>
#include <unistd.h>
#include "file_management.h"

int transfer_files(char *dest_path, char *src_path)
{
  char *command = "mv -f \0";
  size_t command_length = strlen(command);
  size_t dest_path_length = strlen(dest_path);
  size_t src_path_length = strlen(src_path);

  // +1 for space between src and dest and for \0
  char *execute = (char *)calloc((command_length + dest_path_length + src_path_length + 2), sizeof(char));

  strcpy(execute, command);
  strcat(execute, src_path);
  strcat(execute, " \0");
  strcat(execute, dest_path);

  printf("execute: %s\n", execute);

  int status = system(execute);

  return status;
}

int backup_folder(char *backup_path, char *folder_to_backup_path)
{
  char *command = "zip -r \0";
  char *datetime = get_local_datetime_str();
  size_t command_length = strlen(command);
  size_t dest_path_length = strlen(backup_path);
  size_t src_path_length = strlen(folder_to_backup_path);
  size_t datetime_length = strlen(datetime);

  // +4 for space between src and dest and length of .zip and \0
  char *execute = (char *)calloc((command_length + dest_path_length + src_path_length + datetime_length + 5), sizeof(char));

  strcpy(execute, command);
  strcat(execute, backup_path);
  strcat(execute, datetime);
  strcat(execute, ".zip\0");
  strcat(execute, " \0");
  strcat(execute, folder_to_backup_path);

  printf("execute: %s\n", execute);

  int status = system(execute);

  return status;

  // return 0;
}

void check_for_empty_folders(char *folder_path)
{
  char *command = "find %s -type d -empty\0";
  size_t command_length = strlen(command);
  size_t folder_path_length = strlen(folder_path);

  char *execute = (char *)calloc(command_length + folder_path_length + 1, sizeof(char));
  sprintf(execute, command, folder_path);

  FILE *f = popen(execute, "r\0");

  if (f == NULL)
  {
    perror("popen");
    exit(1);
  }

  char *line = NULL;
  size_t length;
  ssize_t read = getline(&line, &length, f);
  size_t line_length = strlen(line);
  line[line_length - 1] = '\0';

  openlog("Upload checker", LOG_PID | LOG_CONS, LOG_DAEMON);

  if (read == -1)
  {
    syslog(LOG_INFO, "All reports uploaded");
    return;
  }

  do
  {
    syslog(LOG_INFO, "%s is empty", line);
  } while ((read = getline(&line, &length, f)) != -1);

  closelog();
}