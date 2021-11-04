#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include "utils.h"
#include "file_management.h"

int transfer_files(char *dest_path, char *src_path)
{
  char *command = "mv -f \0";
  size_t command_length = strlen(command);
  size_t dest_path_length = strlen(dest_path);
  size_t src_path_length = strlen(src_path);

  // +1 for space between src and dest
  char *execute = (char *)malloc((command_length + dest_path_length + src_path_length + 1) * sizeof(char));

  strcat(execute, command);
  strcat(execute, src_path);
  strcat(execute, " \0");
  strcat(execute, dest_path);

  printf("execute: %s\n", execute);

  int status = system(execute);

  return status;
}

int backup_folder(char *backup_path, char *folder_to_backup_path)
{
  char *command = "zip -r -j ";
  char *datetime = get_local_datetime_str();
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

int watch_directory(char *directory_path)
{
  int length, i = 0;
  int fd;
  int wd;
  char buffer[BUF_LEN];

  fd = inotify_init();

  if (fd < 0)
  {
    perror("inotify_init");
  }

  wd = inotify_add_watch(fd, directory_path, IN_CREATE | IN_DELETE | IN_MOVED_FROM | IN_MOVED_TO);
  length = read(fd, buffer, BUF_LEN);

  if (length < 0)
  {
    perror("read");
  }

  while (i < length)
  {
    struct inotify_event *event =
        (struct inotify_event *)&buffer[i];
    if (event->len)
    {
      if (event->mask & IN_CREATE || event->mask & IN_MOVED_TO)
      {
        size_t directory_path_length = strlen(directory_path);
        size_t file_name_length = strlen(event->name);

        char *file_path = (char *)calloc((directory_path_length + file_name_length), sizeof(char));

        strcat(file_path, directory_path);
        strcat(file_path, event->name);

        // 100 for size of command i.e. auditctl -w ...
        char *execute = (char *)calloc(directory_path_length + file_name_length + 100, sizeof(char));

        sprintf(execute, "auditctl -w %s -p wa -k %s", file_path, event->name);
        system(execute);
        // printf("File - %s - created or moved into at\n", file_path);
        // printf("execute command: %s\n", execute);
      }
      else if (event->mask & IN_DELETE || event->mask & IN_MOVED_FROM)
      {
        size_t directory_path_length = strlen(directory_path);
        size_t file_name_length = strlen(event->name);

        char *file_path = (char *)calloc((directory_path_length + file_name_length), sizeof(char));

        strcat(file_path, directory_path);
        strcat(file_path, event->name);

        // 100 for size of command i.e. auditctl -w ...
        char *execute = (char *)calloc(directory_path_length + file_name_length + 100, sizeof(char));

        sprintf(execute, "auditctl -W %s -p wa -k %s", file_path, event->name);
        system(execute);
        // printf("File - %s - deleted or moved from at\n", file_path);
        // printf("execute command: %s\n", execute);
      }
    }
    i += EVENT_SIZE + event->len;
  }

  inotify_rm_watch(fd, wd);
  close(fd);

  return 0;
}