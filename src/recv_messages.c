#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "message.h"
#include "file_management.h"

void start_message_listen()
{
  MsgBuf buf;
  int msqid;
  key_t key;

  openlog("file management", LOG_PID | LOG_CONS, LOG_DAEMON);

  if ((key = ftok("/etc/msgq.conf", 42)) == -1)
  {
    syslog(LOG_ERR, "error getting key for receiving message queue: %s", strerror(errno));
    exit(EXIT_FAILURE);
  }

  if ((msqid = msgget(key, PERMS | IPC_CREAT)) == -1)
  {
    syslog(LOG_ERR, "error getting queue for receiving: %s", strerror(errno));
    exit(EXIT_FAILURE);
  }

  closelog();

  while (1)
  {
    if (msgrcv(msqid, &buf, sizeof(buf.mtext), 0, 0) == -1)
    {
      openlog("file management", LOG_PID | LOG_CONS, LOG_DAEMON);
      syslog(LOG_ERR, "error on receiving message: %s", strerror(errno));
      closelog();
      exit(EXIT_FAILURE);
    }

    if (atoi(buf.mtext) == TRANSFER)
    {
      transfer_reports_to_dashbaord();
    }
    else if (atoi(buf.mtext) == BACKUP)
    {
      backup_srv_folder();
    }
  }
}