#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>

void display_attributes(const char *filename, int depth)
{
  struct stat s;
  if (lstat(filename, &s) < 0)
  {
    perror("lstat");
    return;
  }

  for (int i = 0; i < depth; i++)
  {
    printf("   "); // Indentação
  }

  // Convertendo o tempo de acesso para uma representação legível
  char time_str[20];
  strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&s.st_atime));

  printf("%s\t%ld\t%lu\t%s\t", strrchr(filename, '/') + 1, s.st_ino, s.st_size, time_str);

  struct passwd *passwd;
  passwd = getpwuid(s.st_uid);
  if (passwd)
  {
    printf("%s", passwd->pw_name);
  }
  printf("\n");
}

void display_directory_contents(const char *dirname, int depth)
{
  DIR *dp = opendir(dirname);
  if (!dp)
  {
    perror("opendir");
    return;
  }

  struct dirent *de;
  while ((de = readdir(dp)))
  {
    if (de->d_name[0] == '.')
      continue;

    char path[1024];
    snprintf(path, sizeof(path), "%s/%s", dirname, de->d_name);

    // Se é um diretório e estamos no nível principal (depth == 0), explore-o,
    if (de->d_type == DT_DIR && depth == 0)
    {
      display_attributes(path, depth);

      DIR *sub_dp = opendir(path);
      if (sub_dp)
      {
        struct dirent *sub_de;
        while ((sub_de = readdir(sub_dp)))
        {
          if (sub_de->d_name[0] == '.' || sub_de->d_type == DT_DIR)
            continue;

          char sub_path[2048];
          snprintf(sub_path, sizeof(sub_path), "%s/%s", path, sub_de->d_name);
          display_attributes(sub_path, depth + 1);
        }
        closedir(sub_dp);
      }
    }
    else // Se é um arquivo ou subdiretório no segundo nível
    {
      display_attributes(path, depth);
    }
  }
  closedir(dp);
}

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    printf("[ ERRO ] Uso: %s <path>\n", argv[0]);
    return -1;
  }

  struct stat s;
  if (lstat(argv[1], &s) < 0)
  {
    perror("lstat");
    return -1;
  }

  if (S_ISLNK(s.st_mode))
  {
    printf("Tipo de arquivo não suportado.\n");
    return -1;
  }
  else if (S_ISDIR(s.st_mode))
  {
    display_directory_contents(argv[1], 0);
  }

  else if (S_ISREG(s.st_mode))
  {
    display_attributes(argv[1], 0);
  }
  else
  {
    printf("Tipo de arquivo não suportado.\n");
    return -1;
  }

  return 0;
}
