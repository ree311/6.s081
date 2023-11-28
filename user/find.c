#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void 
find(char *path, char *filename){
  char buf[512], *p;
  int fd, fd1;
  struct dirent de;
  struct stat st, st1;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  //printf("111111111\n");

  switch (st.type)
  {
  case T_FILE:
    /* code */
    fprintf(2, "Path Error!");
    return;
  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("find: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0 || !strcmp(de.name, ".") || !strcmp(de.name, "..")){
        //printf("222222222\n");
        continue;
      }
      memmove(p, de.name, DIRSIZ);
      
      if((fd1 = open(buf, 0))){
        if(fstat(fd1, &st1) < 0){
          fprintf(2, "find: cannot stat %s\n", buf);
          continue;
        }
        switch (st1.type)
        {
        case T_DIR:
          close(fd1);
          //printf("dir current path:%s\n", buf);
          find(buf, filename);
          break;
        case T_FILE:
          //printf("file current path:%s\n", buf);
          if(!strcmp(de.name, filename)){
            printf("%s\n", buf);
          }
          close(fd1);
          break;
        default:
          printf("unknow type\n");
          close(fd1);
          break;
        }
      }else{
        fprintf(2, "find: can't open %s\n", buf);
        continue;
      }
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  if(argc != 3){
    fprintf(2, "Usage: find path filename\n");
    exit(1);
  }

  find(argv[1], argv[2]);
  exit(0);
}
