#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define SIZE 35

void
recur(int p[]){
  int p0[2];
  int prime, num;

  close(0);
  dup(p[0]);
  close(p[0]);
  close(p[1]);

  //printf("test before read");
  if(read(0, &prime, 4) <= 0){
    return;
  }
  printf("prime %d\n", prime);

  pipe(p0);

  // close(1);
  // dup(p0[1]);
  int pid = fork();//notice that fork must be done after pipe(p0), else parent and child would get different p0 fd;

  if(pid > 0){
    //parent
    while(read(0, &num, 4)){
      if(num % prime){
        write(p0[1], &num, 4);
        //printf("parent%d writing%d;\n", getpid(), num);
      }
    }
    close(p0[1]);
    //close(p[0]);
    close(0);
    wait(0);
  }else{
    //child
    //printf("child%d writing\n", getpid());
    recur(p0);
  }
  exit(0);
}

int
main(int argc, char *argv[])
{
  int i = 2;
  int p[2];

  if(argc != 1){
    fprintf(2, "Usage: primes\n");
    exit(1);
  }

  pipe(p);

  for ( i = 2; i <= SIZE; i++)
  {
    write(p[1], &i, 4);
  }
  
  int pid = fork();

  if(pid > 0){
    //parent
    close(p[1]);
    wait(0);
  }else{
    //child
    recur(p);
  }

  exit(0);
}

