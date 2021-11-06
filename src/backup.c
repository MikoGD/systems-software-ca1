#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "file_management.h"

void backup()
{
  time_t curr_time;
  time_t target_time;
  struct tm target_time_temp;

  while (1)
  {
    time(&curr_time);
    printf("t: %ld\n", curr_time);

    target_time_temp = *localtime(&curr_time);

    target_time_temp.tm_hour = 15;
    target_time_temp.tm_min += 1;
    target_time_temp.tm_sec = 0;

    target_time = mktime(&target_time_temp);

    time(&curr_time);
    double seconds = difftime(target_time, curr_time);
    sleep(seconds);

    printf("backing up...\n");
    backup_folder("/home/mikael/ca1/example/example_backups/", "/home/mikael/ca1/example/*");
  }
}