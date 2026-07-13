#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int i;

  if(argc < 2){
    fprintf(2, "usage: sleep ticks\n");
    exit(1);
  }

  i = atoi(argv[1]);
  pause(i);
  exit(0);
}

