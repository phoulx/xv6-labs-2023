#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"

#define BUF_SIZE 1024
#define LINE_MAX 50

// read()一次最多读50字符，无法先全部读出再分割
// 所以需要手动读行（不包括\n）
int readline(char *buf, int len) {
  int i = 0;
  char c;
  while (i < len - 1) {
    if (read(0, &c, 1) != 1) {
      break;
    }
    if (c == '\n') {
      break;
    }
    buf[i++] = c;
  }
  buf[i] = '\0';
  return i;
}

int main(int argc, char *argv[])
{
  if(argc < 2){
    fprintf(2, "Usage: ... | xargs <command>\n");
    exit(1);
  }

  // 从标准输入读取参数，然后执行命令
  char *cmd = argv[1];
  char *args[MAXARG];

  // 先把xargs后面的参数放到args中
  int i;
  for (i = 0; i < argc - 1; i++) {
    args[i] = argv[i + 1];
  }
  for (; i < MAXARG; i++) {
    args[i] = malloc(LINE_MAX * sizeof(char));
    if (readline(args[i], LINE_MAX) == 0) {
      break;
    }
  }

  if (fork() == 0) {
    exec(cmd, args);
    fprintf(2, "xargs: exec error\n");
    exit(1);
  } else {
    wait(0);
  }

  for (int j = 0; j < i; j++) {
    free(args[j]);
  }

  exit(0);
}