#ifndef FILE_MANAGEMENT
#include <sys/inotify.h>

#define FILE_MANAGEMENT
#define EVENT_SIZE (sizeof(struct inotify_event))
#define BUF_LEN (1024 * (EVENT_SIZE + 16))

int transfer_files(char *dest_path, char *src_path);
int backup_folder(char *backup_path, char *folder_to_backup_path);
int watch_directory(char *directory_path);

#endif