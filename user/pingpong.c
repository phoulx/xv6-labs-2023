#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
  int p[2];
  char buf[1];
  pipe(p);
  int rc = fork();
  if (rc == 0) { // child
    read(p[0], buf, 1);
    printf("%d: received ping\n", getpid());
    write(p[1], "a", 1);
    close(p[0]);
    close(p[1]);
    exit(0);
  } else { // parent
    write(p[1], "a", 1);
    wait(0);
    read(p[0], buf, 1);
    printf("%d: received pong\n", getpid());
    close(p[0]);
    close(p[1]);
  }

  exit(0);
}
