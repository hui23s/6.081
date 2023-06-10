#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void prime(int parent_fd){

  int num;
  if(read(parent_fd, &num, 4) != 4){
    //fprintf(2, "child read error\n");
    exit(1);
  }

  printf("prime %d\n", num);

  int p[2];
  pipe(p);
  if(fork() == 0){
    close(p[1]);
    prime(p[0]);
  } else{
    close(p[0]);
    int n;
    int flag = 1;

    while (flag)
    {
      flag = read(parent_fd, &n, 4);
      if(n % num != 0){
        write(p[1], &n, 4);
      }
    }
    close(p[1]);
  }
  wait(0);
  exit(0);
}

int 
main(int argc, char *argv[]){

  int parent_p[2];
  pipe(parent_p);

  if(fork() == 0){
    close(parent_p[1]);
    prime(parent_p[0]);
  } else{
    int i;
    for(i = 2; i <= 35; i++){
      write(parent_p[1], &i, 4);
    }
    close(parent_p[1]);
  }
  
  wait(0);
  exit(0);
}