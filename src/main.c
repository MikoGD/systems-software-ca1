#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "permissions.h"
#include "message.h"
#include "report.h"
#include "utils.h"

int main()
{
  pid_t main_pid = fork();
  pid_t message_pid;

  if (main_pid > 0)
  {
    exit(EXIT_SUCCESS);
  }
  else if (main_pid == 0)
  {
    sleep(2);

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

    message_pid = fork();

    while (1)
    {
      wait_until(1, 0, 0);
      check_for_empty_folders("/home/mikael/ca1/example/reports\0");
      lock_folder_to_readonly("/home/mikael/ca1/example/reports\0");
      backup_folder("/home/mikael/ca1/example/example_backups/\0", "/home/mikael/ca1/example/*\0");
      transfer_files("/home/mikael/ca1/example/dashboard/\0", "/home/mikael/ca1/example/reports/*\0");
      create_aureport("monitor-example\0", "/var/file_management_reports\0");
      unlock_folder_from_readonly("/home/mikael/ca1/example/reports\0");
    }
  }
  else if (message_pid == 0)
  {
    start_message_listen();
  }

  return 0;
}