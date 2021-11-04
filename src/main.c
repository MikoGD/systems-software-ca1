#include <stdio.h>
// #include "permissions.h"
#include "file_management.h"

int main()
{
  // int status = transfer_files("/home/mikael/ca1/example/reports", "/home/mikael/ca1/example/dashboard/*.xml");
  // printf("status: %d\n", status);
  int status = backup_folder("/home/mikael/ca1/example/reports_backup/\0", "/home/mikael/ca1/example/reports\0");
  printf("status: %d\n", status);

  return 0;
}