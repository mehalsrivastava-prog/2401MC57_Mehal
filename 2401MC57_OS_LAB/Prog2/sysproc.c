#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int n;
  uint addr;
  struct proc *curproc = myproc();

  if(argint(0, &n) < 0)
    return -1;

  addr = curproc->sz;

  if(n > 0) {
    // Q2: Reserve virtual address space without allocating pages.
    if(curproc->sz + n < curproc->sz)
      return -1;
    curproc->sz += n;
  } else if(n < 0) {
    // Preserve normal shrinking behavior.
    if(growproc(n) < 0)
      return -1;
  }

  return addr;
}


int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_vmprint(void)
{
  vmprint();
  return 0;
}

// Q2: Return the number of lazy page faults handled.
int
sys_getfaults(void)
{
  return myproc()->pagefaults;
}

int
sys_memstats(void)
{
  uint allocated, freed, current, available;

  getmemstats(&allocated, &freed, &current, &available);

  cprintf("Total allocated pages: %d\n", allocated);
  cprintf("Total freed pages: %d\n", freed);
  cprintf("Currently allocated pages: %d\n", current);
  cprintf("Available physical pages: %d\n", available);

  return 0;
}
int
sys_vtop(void)
{
  int va;
  uint pa;

  if(argint(0, &va) < 0)
    return -1;

  pa = vtop(myproc()->pgdir, (uint)va);

  if(pa == 0)
    return -1;

  return pa;
}
int
sys_pageinfo(void)
{
  int va;

  if(argint(0, &va) < 0)
    return -1;

  return pageinfo(myproc()->pgdir, (uint)va);
}

int
sys_setreadonly(void)
{
  int va;

  if(argint(0, &va) < 0)
    return -1;

  return setreadonly(myproc()->pgdir, (uint)va);
}

int
sys_mprotect(void)
{
  void *addr;
  int len;
  if(argptr(0, (char**)&addr, sizeof(void*)) < 0 || argint(1, &len) < 0)
    return -1;
  return mprotect(addr, len);
}

int
sys_munprotect(void)
{
  void *addr;
  int len;
  if(argptr(0, (char**)&addr, sizeof(void*)) < 0 || argint(1, &len) < 0)
    return -1;
  return munprotect(addr, len);
}

int
sys_set_priority(void)
{
  int pid, priority;
  if(argint(0, &pid) < 0 || argint(1, &priority) < 0)
    return -1;
  return set_priority(pid, priority);
}