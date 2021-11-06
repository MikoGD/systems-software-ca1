#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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