#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TIME_STR_LENGTH 50

char *get_local_datetime()
{
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  char *time_str = (char *)malloc(TIME_STR_LENGTH * sizeof(char));

  sprintf(time_str, "%d-%02d-%02d_%02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

  return time_str;
}