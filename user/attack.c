#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"

int
main(int argc, char *argv[])
{
  // Your code here.
  char *p;

  if(argc != 1){
    printf("Usage: attack\n");
    exit(1);
  }

  p = sbrk(PGSIZE * 18);
  if(p == SBRK_ERROR)
    exit(1);

  p += PGSIZE * 17;
  printf("%s\n", p + 32);

  exit(1);
}
