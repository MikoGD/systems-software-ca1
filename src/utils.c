#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define TIME_STR_LENGTH 50

char *get_local_date_str_system_format()
{
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  char *time_str = (char *)calloc(TIME_STR_LENGTH, sizeof(char));

  sprintf(time_str, "%02d/%02d/%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900 - 2000);

  return time_str;
}

char *get_local_datetime_str()
{
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  char *time_str = (char *)calloc(TIME_STR_LENGTH, sizeof(char));

  sprintf(time_str, "%d-%02d-%02d_%02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

  return time_str;
}

time_t get_local_datetime_epoch()
{
  time_t t = time(NULL);
  return t;
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

  printf("seconds: %f\n", seconds);
  sleep(seconds);
}