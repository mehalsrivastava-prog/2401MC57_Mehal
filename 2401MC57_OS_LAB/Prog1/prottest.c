#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  char *p;
  
  printf(1, "Start\n");

  p = sbrk(4096);
  if((uint)p == 0xffffffff){
    printf(1, "sbrk failed\n");
    exit();
  }

  *p = 'A';
  printf(1, "Initial read/write successful. Value: %c\n", *p);

  if(mprotect(p, 1) < 0){
    printf(1, "mprotect failed\n");
    exit();
  }
  printf(1, "After mprotect, read value: %c (Success)\n", *p);

  int pid = fork();
  if(pid < 0){
    printf(1, "fork failed\n");
    exit();
  }
  if(pid == 0){
    printf(1, "Child trying to write to protected page...\n");
    *p = 'B';
    printf(1, "Child write succeeded unexpectedly!\n");
    exit();
  } else {
    int status;
    wait();
    printf(1, "Parent continues. Parent still reads: %c (Success)\n", *p);
  }

  if(munprotect(p, 1) < 0){
    printf(1, "munprotect failed\n");
    exit();
  }
  *p = 'C';
  printf(1, "After munprotect, write successful. New value: %c (Success)\n", *p);

  printf(1, "Testing invalid arguments:\n");
  if(mprotect(p + 1, 1) == -1)
    printf(1, "  - Unaligned address test passed (-1)\n");
  else
    printf(1, "  - Unaligned address test failed\n");

  if(mprotect(p, 0) == -1)
    printf(1, "  - Zero length test passed (-1)\n");
  else
    printf(1, "  - Zero length test failed\n");

  if(mprotect((void*)((uint)sbrk(0) + 4096), 1) == -1)
    printf(1, "  - Beyond process memory test passed (-1)\n");
  else
    printf(1, "  - Beyond process memory test failed\n");

  printf(1, "prottest completed successfully.\n");
  exit();
}
