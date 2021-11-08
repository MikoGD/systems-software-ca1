#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>
#include "file_management.h"
#include "utils.h"

void transfer_files(char *dest_path, char *src_path)
{
  char *command = "cp -r %s/. %s/. && rm -f %s/**/*.xml\0";
  size_t command_length = strlen(command);
  size_t dest_path_length = strlen(dest_path);
  size_t src_path_length = strlen(src_path);

  // +1 for \0
  char *execute = (char *)calloc((command_length + dest_path_length + (src_path_length * 2) + 1), sizeof(char));
  sprintf(execute, command, src_path, dest_path, src_path);

  int status = system(execute);

  openlog("file management", LOG_PID | LOG_CONS, LOG_DAEMON);

  if (status < 0)
  {
    syslog(LOG_ERR, "error transferring files: %s", strerror(errno));
  }
  else if (WIFEXITED(status))
  {
    if (WEXITSTATUS(status) == 0)
    {
      syslog(LOG_INFO, "exiting transferring files from %s to %s with status %d", src_path, dest_path,
             WEXITSTATUS(status));
    }
    else
    {
      syslog(LOG_ERR, "exiting transferring files from %s to %s with status %d", src_path, dest_path,
             WEXITSTATUS(status));
    }
  }

  closelog();
}

void backup_folder(char *backup_path, char *folder_to_backup_path)
{
  char *command = "zip -r %s%s.zip %s\0";
  char *datetime = get_local_datetime_str();
  size_t command_length = strlen(command);
  size_t dest_path_length = strlen(backup_path);
  size_t src_path_length = strlen(folder_to_backup_path);
  size_t datetime_length = strlen(datetime);

  // +4 for space between src and dest and length of .zip and \0
  char *execute = (char *)calloc((command_length + dest_path_length + src_path_length + datetime_length + 1), sizeof(char));

  sprintf(execute, command, backup_path, datetime, folder_to_backup_path);

  int status = system(execute);

  openlog("file management", LOG_PID | LOG_CONS, LOG_DAEMON);

  if (status < 0)
  {
    syslog(LOG_ERR, "error backing up files: %s", strerror(errno));
  }
  else if (WIFEXITED(status))
  {
    if (WEXITSTATUS(status) == 0)
    {
      syslog(LOG_INFO, "exiting backuping files from %s to %s with status %d", folder_to_backup_path, backup_path,
             WEXITSTATUS(status));
    }
    else
    {
      syslog(LOG_ERR, "exiting backuping files from %s to %s with status %d", folder_to_backup_path, backup_path,
             WEXITSTATUS(status));
    }
  }

  closelog();
}

void check_for_empty_folders(char *folder_path)
{
  char *command = "find %s -type d -empty\0";
  size_t command_length = strlen(command);
  size_t folder_path_length = strlen(folder_path);

  char *execute = (char *)calloc(command_length + folder_path_length + 1, sizeof(char));
  sprintf(execute, command, folder_path);

  FILE *f = popen(execute, "r\0");

  openlog("file management", LOG_PID | LOG_CONS, LOG_DAEMON);

  if (f == NULL)
  {
    syslog(LOG_ERR, "error checking for empty folders: %s", strerror(errno));
  }

  char *line = NULL;
  size_t length;
  ssize_t read = getline(&line, &length, f);
  size_t line_length = strlen(line);
  line[line_length - 1] = '\0';

  if (read == -1)
  {
    syslog(LOG_INFO, "All reports uploaded");
    return;
  }

  do
  {
    syslog(LOG_INFO, "Report for %s is missing", line);
  } while ((read = getline(&line, &length, f)) != -1);

  fclose(f);
  closelog();
}

void check_for_missing_reports()
{
  check_for_empty_folders("/srv/reports\0");
}

void backup_srv_folder()
{
  backup_folder("/var/backups/file_management/\0", "/srv/*\0");
}

void transfer_reports_to_dashbaord()
{
  transfer_files("/srv/dashboard/\0", "/srv/reports/*\0");
}