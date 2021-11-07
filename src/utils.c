#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <time.h>
#include <unistd.h>
#include "utils.h"

char *get_local_date_str_system_format()
{
  FILE *f = popen("date --date=\"yesterday\" '+\%x'", "r\0");

  char *line;
  size_t length;
  ssize_t read = getline(&line, &length, f);

  openlog("file_management\0", LOG_PID | LOG_CONS, LOG_DAEMON);
  
  if (read == -1)
  {
    syslog(LOG_ERR, "Failed to read date");
    return "\0";
  }

  int line_length = strlen(line);
  line[line_length - 1] = '\0';

  printf("line: %s\n", line);

  fclose(f);

  return line;
}

char *get_local_datetime_str()
{
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  char *time_str = (char *)calloc(TIME_STR_LENGTH, sizeof(char));

  sprintf(time_str, "%d-%02d-%02d_%02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min,
          tm.tm_sec);

  return time_str;
}

void wait_until(int hour, int min, int sec)
{
  time_t curr_time;
  time_t target_time;
  struct tm target_time_temp;

  time(&curr_time);

  target_time_temp = *localtime(&curr_time);

  target_time_temp.tm_hour = hour;
  target_time_temp.tm_min = min;
  target_time_temp.tm_sec = sec;

  target_time = mktime(&target_time_temp);

  time(&curr_time);
  double seconds = difftime(target_time, curr_time);

  if (seconds <= 0)
  {
    target_time += 24 * 60 * 60;
    seconds = difftime(target_time, curr_time);
  }

  sleep(seconds);
}