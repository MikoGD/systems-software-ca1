#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "message.h"

int start_message_listen()
{
  MsgBuf buf;
  int msqid;
  key_t key;

  if ((key = ftok("/etc/msgq.conf", 'B')) == -1)
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
    }
    else if (atoi(buf.mtext) == BACKUP)
    {
      printf("backup\n");
    }
    else
    {
      printf("uknown method\n");
      return 1;
    }
  }

  return 0;
}