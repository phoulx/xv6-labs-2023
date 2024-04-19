#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"


void find(char *dir, char *file);

int main(int argc, char *argv[])
{
  if(argc != 3){
    fprintf(2, "Usage: find <dir> <file>\n");
    exit(1);
  }

  char *dir = argv[1];
  char *file = argv[2];

  find(dir, file);

  exit(0);
}

void find(char *path, char *file) {
  char buf[128], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if ((fd = open(path, O_RDONLY)) < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch (st.type) {
  case T_DEVICE:
  case T_FILE:
    for(p=path+strlen(path); p >= path && *p != '/'; p--)
      ;
    p++;
    if (strcmp(p, file) == 0) {
      printf("%s\n", path);
    }
    break;
  
  case T_DIR:
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)) {
      if (de.inum == 0) {
        continue;
      }
      if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) {
        continue;
      }
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      find(buf, file);
    }
    break;
  }
  // 可能因为fd数量有限，打开几个文件且不close，就无法打开新文件
  close(fd);
}