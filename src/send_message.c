#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "message.h"

int main(int argc, char **argv)
{
  MsgBuf buf;
  int msqid;
  key_t key;
  size_t len;

  if (argc != 2)
  {
    printf("error: not enough arguments\n");
    exit(1);
  }

  if (atoi(argv[1]) > 1 || atoi(argv[1]) < 0)
  {
    printf("error: invalid argument %s\n", argv[1]);
    exit(1);
  }

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

  buf.mtype = 1;
  strcpy(buf.mtext, argv[1]);
  len = strlen(buf.mtext);

  if (msgsnd(msqid, &buf, len + 1, 0) == -1)
  {
    perror("msgsnd");
  }
  else
  {
    printf("message sent\n");
  }

  return 0;
}