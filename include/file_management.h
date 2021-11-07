#ifndef FILE_MANAGEMENT

#define FILE_MANAGEMENT

int transfer_files(char *dest_path, char *src_path);
int backup_folder(char *backup_path, char *folder_to_backup_path);
void check_for_empty_folders(char *folder_path);

#endif
