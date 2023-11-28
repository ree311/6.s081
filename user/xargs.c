#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define SIZE 512

int
main(int argc, char *argv[])
{
  char stdIn[SIZE];
  read(0, stdIn, sizeof stdIn);

  char *xargv[MAXARG];
  int xargc = 0;

  for (int i = 1; i < argc; i++)
  {
    xargv[xargc++] =  argv[i];
  }

  char *p = stdIn;

  for (int i = 0; i < SIZE; i++)
  {
    if(stdIn[i] == '\n'){
      int pid = fork();
      if(pid > 0){
        wait(0);
        //printf("ppp");
        p = &stdIn[i+1];
      }else{
        stdIn[i] = 0;
        xargv[xargc++] = p;
        xargv[xargc++] = 0;
        //printf("cccs");
        exec(xargv[0], xargv);
        exit(0);
      }
    }
  }
  exit(0);
}
