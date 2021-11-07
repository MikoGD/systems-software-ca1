#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "file_management.h"
#include "utils.h"

void transfer_timer()
{
  while (1)
  {
    wait_until(1, 0, 0);
    backup_folder("/home/mikael/ca1/example/example_backups/\0", "/home/mikael/ca1/example/*\0");
    transfer_files("/home/mikael/ca1/example/dashboard/\0", "/home/mikael/ca1/example/reports/*\0");
  }
}

void upload_check_timer()
{
  while (1)
  {
    wait_until(14, 11, 45);
    check_for_empty_folders("/home/mikael/ca1/example/reports");
  }
}