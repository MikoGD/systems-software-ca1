#ifndef FILE_MANAGEMENT

#define FILE_MANAGEMENT

void transfer_files(char *dest_path, char *src_path);
void backup_folder(char *backup_path, char *folder_to_backup_path);
void check_for_empty_folders(char *folder_path);

#endif
