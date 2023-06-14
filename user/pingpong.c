#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int 
main(int argc, char *argv[]){
  int p1[2], p2[2];
  pipe(p1);
  pipe(p2);
  char buf[1];

  int pid;
  if(fork() == 0){
    pid = getpid();
    
    close(p1[1]);
    if(read(p1[0], buf, 1) != 1){
      fprintf(2, "child read error\n");
      exit(1);
    }

    close(p1[0]);
    printf("%d: received ping\n", pid);

    close(p2[0]);
    if(write(p2[1], buf, 1) != 1){
      fprintf(2, "child write error\n");
      exit(1);
    }

    close(p2[1]);
    exit(0);

  } else{
    pid = getpid();
    buf[0] = 'a';

    close(p1[0]);
    if(write(p1[1], buf, 1) != 1){
      fprintf(2, "parent write error\n");
      exit(1);
    }
    
    close(p1[1]);
    wait(0);

    close(p2[1]);
    if(read(p2[0], buf, 1) != 1){
      fprintf(2, "parent read error\n");
      exit(1);
    }
    close(p2[0]);
    printf("%d: received pong\n", pid);
  }

  exit(0);
}