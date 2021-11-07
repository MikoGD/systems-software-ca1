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
    openlog("file management", LOG_PID | LOG_CONS, LOG_DAEMON);
    syslog(LOG_DEBUG, "killing parent");
    closelog();
    exit(EXIT_SUCCESS);
  }
  else if (main_pid == 0)
  {
    if (main_pid == 0)
    {
      openlog("file management", LOG_PID | LOG_CONS, LOG_DAEMON);
      syslog(LOG_DEBUG, "creating daemon");
      if (setsid() < 0)
      {
        exit(EXIT_FAILURE);
      }

      umask(0);

      if (chdir("/") < 0)
      {
        exit(EXIT_FAILURE);
      }

      for (int x = sysconf(_SC_OPEN_MAX); x >= 0; x--)
      {
        close(x);
      }

      syslog(LOG_DEBUG, "starting daemon loop");
      closelog();

      while (1)
      {
        wait_until(18, 53, 55);
        check_for_empty_folders("/srv/reports\0");
        lock_folder_to_readonly("/srv\0");
        backup_folder("/srv/backups/\0", "srv/*\0");
        transfer_files("/srv/dashboard/\0", "/srv/reports/*\0");
        create_aureport("monitor-example\0", "/var/file_management_reports\0");
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