#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <syslog.h>
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

  openlog("file management", LOG_PID | LOG_CONS, LOG_DAEMON);

  if (argc != 2)
  {
    printf("error: not enough arguments\n");
    syslog(LOG_ERR, "error sending message: not enough arguments");
    exit(EXIT_FAILURE);
  }

  if (atoi(argv[1]) > 1 || atoi(argv[1]) < 0)
  {
    printf("error: invalid argument %s\n", argv[1]);
    syslog(LOG_ERR, "error sending message: invalid arguments");
    exit(EXIT_FAILURE);
  }

  if ((key = ftok("/etc/msgq.conf", 42)) == -1)
  {
    perror("ftok");
    syslog(LOG_ERR, "error getting key for sending message queue: %s", strerror(errno));
    exit(EXIT_FAILURE);
  }

  if ((msqid = msgget(key, PERMS | IPC_CREAT)) == -1)
  {
    syslog(LOG_ERR, "error getting queue for sending: %s", strerror(errno));
    exit(EXIT_FAILURE);
  }

  buf.mtype = 1;
  strcpy(buf.mtext, argv[1]);
  len = strlen(buf.mtext);

  if (msgsnd(msqid, &buf, len + 1, 0) == -1)
  {
    syslog(LOG_ERR, "error on sending message: %s", strerror(errno));
    exit(EXIT_FAILURE);
  }
  else
  {
    printf("message sent\n");
    syslog(LOG_INFO, "sent message of %s", buf.mtext, strerror(errno));
  }

  closelog();

  return 0;
}