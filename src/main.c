#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

void move_files(char *dest_path, char *src_path);

int main()
{
  // if (chmod("./example/reports", 01770) != 0 || chmod("./example/dashboard", 01770) != 0)
  // {
  //   perror("locking chmod");
  //   exit(1);
  // }

  if (chown("./example/reports", -1, 1001) != 0 || chown("./example/dashboard", -1, 1001) != 0)
  {
    perror("locking chown");
    exit(1);
  }
  printf("uploading...\n");
  sleep(2);
  // move_files("./example/dashboard/", "./example/reports/");
  move_files("./example/reports/", "./example/dashboard/");

  printf("done\n");

  // if (chmod("./example/reports", 01770) != 0 || chmod("./example/dashboard", 01770) != 0)
  // {
  //   perror("unlocking chmod");
  // }

  if (chown("./example/reports", -1, 1002) != 0 || chown("./example/dashboard", -1, 1002) != 0)
  {
    perror("unlocking chown");
  }

  return 0;
}

void move_files(char *dest_path, char *src_path)
{
  DIR *d;
  struct dirent *dir;
  d = opendir(src_path);

  if (d)
  {
    while ((dir = readdir(d)) != NULL)
    {
      if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0)
      {
        char curr_file_path[strlen(src_path) + 256];
        char new_file_path[strlen(dest_path) + 256];

        strcpy(curr_file_path, src_path);
        strcpy(new_file_path, dest_path);

        strcat(curr_file_path, dir->d_name);
        strcat(new_file_path, dir->d_name);

        printf("src: %s\n", curr_file_path);
        printf("dst: %s\n", new_file_path);

        int status = rename(curr_file_path, new_file_path);
        if (status == -1)
        {
          perror("rename");
          printf("something went wrong\n");
          exit(1);
        }
      }
    }
  }
  else
  {
    printf("Directory doesn't exist");
  }

  closedir(d);
}
