#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "message.h"
#include "file_management.h"

int start_message_listen()
{
  MsgBuf buf;
  int msqid;
  key_t key;

  if ((key = ftok("/etc/msgq.conf", 42)) == -1)
  {
    perror("ftok");
    exit(1);
  }

  printf("key: %d\n", key);

  if ((msqid = msgget(key, PERMS | IPC_CREAT)) == -1)
  {
    perror("msgget");
    exit(1);
  }

  printf("message queue: ready to receive messages.\n");

  while (1)
  {
    if (msgrcv(msqid, &buf, sizeof(buf.mtext), 0, 0) == -1)
    {
      perror("msgrcv");
      exit(1);
    }

    printf("message recieved\n");

    if (atoi(buf.mtext) == TRANSFER)
    {
      printf("transfer\n");
      transfer_files("/home/mikael/ca1/example/dashboard/\0", "/home/mikael/ca1/example/reports/*\0");
    }
    else if (atoi(buf.mtext) == BACKUP)
    {
      printf("backup\n");
      backup_folder("/home/mikael/ca1/example/example_backups/", "/home/mikael/ca1/example/*");
    }
  }

  return 0;
}