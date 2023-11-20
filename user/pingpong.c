#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  char buf0[1] = "s";
  char buf1[1];
  char buf2[1];
  int p0[2], p1[2];

  if(argc != 1){
    fprintf(2, "Usage: pingpong\n");
    exit(1);
  }

  pipe(p0);
  pipe(p1);

  if(fork() == 0){
    //child
    close(0);
    dup(p0[0]);
    close(1);
    dup(p1[1]);

    if(read(0, buf1, 1) != 1){
        fprintf(2, "child read error!");
        exit(1);
    }

    fprintf(2, "%d: received ping\n", getpid());

    if(write(1, buf1, 1) != 1){
      fprintf(2, "child write error!");
      exit(1);
    }

    close(p0[0]);
    close(p1[1]);
  }else{
    //parent
    close(0);
    dup(p1[0]);
    close(1);
    dup(p0[1]);

    if(write(1, buf0, 1) != 1){
      fprintf(2, "parent write error!");
      exit(1);
    }

    wait(0);

    if(read(0, buf2, 1) != 1){
      fprintf(2, "parent read error!");
      exit(1);
    }

    fprintf(2, "%d: received pong\n", getpid());
    
    close(p0[1]);
    close(p1[0]);
  }

  

  exit(0);
}
