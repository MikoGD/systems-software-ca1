#ifndef FILE_MANAGEMENT_MESSAGE

#define FILE_MANAGEMENT_MESSAGE
#define PERMS 0644
#define MESSAGE_LENGTH 200

enum method
{
  TRANSFER,
  BACKUP
};

typedef struct MsgBuf
{
  long mtype;
  char mtext[200];
} MsgBuf;

int start_message_listen();

#endif