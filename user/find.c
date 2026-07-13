#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"

char*
fmtname(char *path)
{
  char *p;
  for (p = path + strlen(path); p >= path && *p != '/'; p--)
    ;
  return p + 1;
}

void
run_exec(char *path, int argc, char *argv[])
{
  char *args[MAXARG];
  int i;

  for(i = 4; i < argc; i++)
    args[i-4] = argv[i];

  args[i-4] = path;
  args[i-3] = 0;

  if(fork() == 0){
    exec(args[0], args);

    fprintf(2, "find: exec failed\n");
    exit(1);
  }

  wait(0);
}

void
find(char *path, char *name, int argc, char *argv[])
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, O_RDONLY)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    if(strcmp(fmtname(path), name) == 0){
      if(argc > 3 && strcmp(argv[3], "-exec") == 0){
        run_exec(path, argc, argv);
      }
      else{
        printf("%s\n", path);
      }
    }
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)){
      fprintf(2, "find: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;

      if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
        continue;

      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      find(buf, name, argc, argv);
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  if(argc < 3){
    fprintf(2, "usage: find path name [-exec cmd]\n");
    exit(1);
  }

  find(argv[1], argv[2], argc, argv);
  exit(0);
}

