#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

void
sixfive(int fd)
{
  char c;
  int num, in_num, valid;

  num = 0, in_num = 0, valid = 1;
  while(read(fd, &c, 1) > 0){
    if(c >= '0' && c <= '9'){
      num = num * 10 + (c - '0');
      in_num = 1;
    }
    else if(strchr(" -\r\t\n./,", c)){
      if(in_num && valid){
        if(num % 5 == 0 || num % 6 == 0){
          printf("%d\n", num);
	}
      }
      num = 0;
      in_num = 0;
      valid = 1;
    }
    else{
      num = 0;
      in_num = 0;
      valid = 0;
    }
  }

  if(in_num && valid){
    if(num % 5 == 0 || num % 6 == 0){
      printf("%d\n", num);
    }
  }
}

int
main(int argc, char *argv[])
{
  int fd, i;

  if(argc < 2){
    fprintf(2, "usage: sixfive file ...\n");
    exit(1);
  }

  for(i = 1; i < argc; i++){
    if((fd = open(argv[i], O_RDONLY)) < 0){
      fprintf(2, "sixfive: cannot open %s\n", argv[i]);
      exit(1);
    }

    sixfive(fd);
    close(fd);
  }

  exit(0);
}
