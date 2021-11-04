#include <stdio.h>
#include "permissions.h"

int main()
{
  printf("locking\n");
  int status = unlock_folder("/home/mikael/ca1/example/reports");

  printf("status: %d\n", status);
  return 0;
}