#include <errno.h>
#include <pthread.h>
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
  pid_t pid = fork();

  if (pid == 0)
  {
    openlog("file management", LOG_PID | LOG_CONS, LOG_DAEMON);
    syslog(LOG_DEBUG, "starting file management daemon");

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

    pthread_t id;

    syslog(LOG_INFO, "starting message listening thread");
    pthread_create(&id, NULL, start_message_listen, NULL);

    closelog();

    while (1)
    {
      wait_until(1, 0, 0);
      create_aureport_for_reports_folder();
      check_for_missing_reports();
      make_srv_readonly();
      backup_srv_folder();
      transfer_reports_to_dashbaord();
      remove_readonly_from_srv();
    }
  }
  else
  {
    exit(EXIT_SUCCESS);
  }

  return 0;
}