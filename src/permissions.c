#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>
#include "permissions.h"

void lock_folder_to_readonly(char *folder_path)
{
  openlog("file management", LOG_PID | LOG_CONS, LOG_DAEMON);

  char *command = "chgrp -R managers %s && chown -R root %s && chmod -R g-w %s\0";
  int folder_path_length = strlen(folder_path);
  int command_length = strlen(command);
  // +1 for \0
  char *execute = (char *)calloc((folder_path_length * 3) + command_length + 1, sizeof(char));

  sprintf(execute, command, folder_path, folder_path, folder_path);

  int status = system(execute);

  if (status < 0)
  {
    syslog(LOG_ERR, "error locking folder to readonly: %s", strerror(errno));
  }
  else if (WIFEXITED(status))
  {
    if (WEXITSTATUS(status) == 0)
    {
      syslog(LOG_INFO, "exiting locking folder of %s to readonly with status %d", folder_path, WEXITSTATUS(status));
    }
    else
    {
      syslog(LOG_ERR, "exiting locking folder of %s to readonly with status %d", folder_path, WEXITSTATUS(status));
    }
  }

  closelog();
}

void unlock_folder_from_readonly(char *folder_path)
{
  openlog("file management", LOG_PID | LOG_CONS, LOG_DAEMON);

  char *command = "chgrp -R managers %s && chown -R root %s && chmod -R g+rwx %s\0";
  int folder_path_length = strlen(folder_path);
  int command_length = strlen(command);
  // +1 for \0
  char *execute = (char *)calloc((folder_path_length * 3) + command_length + 1, sizeof(char));

  sprintf(execute, command, folder_path, folder_path, folder_path);

  int status = system(execute);

  if (status < 0)
  {
    syslog(LOG_ERR, "error unlocking folder from readonly: %s", strerror(errno));
  }
  else if (WIFEXITED(status))
  {
    if (WEXITSTATUS(status) == 0)
    {
      syslog(LOG_INFO, "exiting unlocking folder of %s from readonly with status %d", folder_path, WEXITSTATUS(status));
    }
    else
    {
      syslog(LOG_ERR, "exiting unlocking folder of %s from readonly with status %d", folder_path, WEXITSTATUS(status));
    }
  }

  closelog();
}

void make_srv_readonly()
{
  lock_folder_to_readonly("/srv\0");
}

void remove_readonly_from_srv()
{
  unlock_folder_from_readonly("/srv\0");
}