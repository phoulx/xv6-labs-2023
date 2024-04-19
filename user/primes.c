#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
  int p[2];
  pipe(p);
  for (int i = 2; i <= 100; i++) {
    write(p[1], &i, sizeof(i));
  }
  close(p[1]);
  int curr, num;
  while (read(p[0], &curr, sizeof(curr)) > 0) {
    printf("prime %d\n", curr);
    int newp[2];
    pipe(newp);
    int rc = fork();
    if (rc == 0) { // child
      close(p[0]);
      close(newp[1]);
      p[0] = newp[0];
      p[1] = newp[1];
    } else { // parent
      close(newp[0]);
      while (read(p[0], &num, sizeof(num)) > 0) {
        if (num % curr != 0) {
          write(newp[1], &num, sizeof(num));
        }
      }
      close(p[0]);
      close(newp[1]);
      wait(0);
    }
  }

  exit(0);
}
