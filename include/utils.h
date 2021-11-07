#ifndef FILE_MANAGEMENT_UTILS

#include <time.h>

#define FILE_MANAGEMENT_UTILS
#define TIME_STR_LENGTH 50

char *get_local_date_str_system_format();
char *get_local_datetime_str();
time_t get_local_datetime_epoch();
void wait_until();

#endif