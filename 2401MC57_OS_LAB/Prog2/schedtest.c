#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  int i, pid;
  
  printf(1, "schedtest starting: Setting parent priority to 1\n");
  if(set_priority(getpid(), 1) < 0){
    printf(1, "Failed to set parent priority\n");
    exit();
  }
  int priorities[3] = {5, 10, 15};
  for(i = 0; i < 3; i++){
    pid = fork();
    if(pid < 0){
      printf(1, "fork failed\n");
      exit();
    }
    if(pid == 0){
      set_priority(getpid(), priorities[i]);
      printf(1, "Child [PID: %d] started with priority %d\n", getpid(), priorities[i]);
      
      volatile int j, k;
      for(j = 0; j < 40000; j++){
        for(k = 0; k < 1000; k++){
        }
      }
      printf(1, "Child [PID: %d] with priority %d finished\n", getpid(), priorities[i]);
      exit();
    }
  }

  for(i = 0; i < 3; i++){
    wait();
  }

  printf(1, "Done\n");
  exit();
}