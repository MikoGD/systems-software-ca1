#ifndef MANAGEMENT_PERMISSIONS

#define MANAGEMENT_PERMISSIONS

void lock_folder_to_readonly(char *folder_path);
void unlock_folder_from_readonly(char *folder_path);
void make_srv_readonly();
void remove_readonly_from_srv();

#endif
