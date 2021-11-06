#include <stdio.h>
// #include "permissions.h"
// #include "file_management.h"
// #include "message.h"
#include "backup.h"

int main()
{
  // transfer_files("/home/mikael/ca1/example/dashboard", "/home/mikael/ca1/example/reports/*");
  // start_message_listen();
  backup_timer();
  return 0;
}