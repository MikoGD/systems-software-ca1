#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utils.h"

void create_aureport(char *key, char *report_folder_path)
{
  char *command = "ausearch -k %s | aureport -i -k > %s\0";
  char *datetime = get_local_datetime_str();
  char *report_name = "%s/report_%s.txt\0";
  size_t command_length = strlen(command);
  size_t key_length = strlen(key);
  size_t report_folder_path_length = strlen(report_folder_path);
  size_t datetime_length = strlen(datetime);
  size_t report_name_length = strlen(report_name);

  char *execute = (char *)calloc(command_length + key_length + report_folder_path_length + datetime_length + report_name_length, sizeof(char));
  char *full_report_path = (char *)calloc(report_name_length + datetime_length + report_folder_path_length, sizeof(char));

  sprintf(full_report_path, report_name, report_folder_path, datetime);
  sprintf(execute, command, key, full_report_path);

  printf("execute: %s\n", execute);

  system(execute);
}