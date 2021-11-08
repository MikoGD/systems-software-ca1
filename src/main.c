#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <sys/stat.h>
#include <unistd.h>
#include "file_management.h"
#include "message.h"
#include "permissions.h"
#include "report.h"
#include "utils.h"

int main()
{
  openlog("file management", LOG_PID | LOG_CONS, LOG_DAEMON);
  syslog(LOG_DEBUG, "starting file management daemon");
  closelog();

  pid_t main_pid = fork();

  if (main_pid > 0)
  {
    exit(EXIT_SUCCESS);
  }
  else if (main_pid == 0)
  {
    if (main_pid == 0)
    {
      openlog("file management", LOG_PID | LOG_CONS, LOG_DAEMON);
      syslog(LOG_INFO, "starting file management daemon");
      closelog();

      if (setsid() < 0)
      {
        syslog(LOG_ERR, "Failed to create a session");
        exit(EXIT_FAILURE);
      }

      umask(0);

      if (chdir("/") < 0)
      {
        syslog(LOG_ERR, "Failed to set current working directory");
        exit(EXIT_FAILURE);
      }

      for (int x = sysconf(_SC_OPEN_MAX); x >= 0; x--)
      {
        close(x);
      }

      closelog();

      while (1)
      {
        wait_until(21, 32, 5);
        check_for_empty_folders("/srv/reports\0");
        lock_folder_to_readonly("/srv\0");
        backup_folder("/var/backups/file_management/\0", "srv/*\0");
        transfer_files("/srv/dashboard/\0", "/srv/reports/*\0");
        create_aureport("monitor-files\0", "/var/file_management_reports\0");
        unlock_folder_from_readonly("/srv\0");
      }
    }
  }
  else
  {
    pid_t message_pid = fork();
    if (message_pid == 0)
    {
      openlog("file management", LOG_PID | LOG_CONS, LOG_DAEMON);
      syslog(LOG_DEBUG, "starting message listening");
      closelog();
      start_message_listen();
    }
  }

  return 0;
}