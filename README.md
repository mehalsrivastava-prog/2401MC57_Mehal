# 2401MC57_Mehal_Set-2 : 

Question 1: Page Protection with mprotect() and munprotect()
- vm.c & defs.h: Added a helper function getpte() to expose the static walkpgdir() function and implemented mprotect() and munprotect() to validate page alignment, address bounds, and page presence before clearing or setting the PTE_W write bit and flushing the TLB via lcr3().
- syscall.h, syscall.c, sysproc.c, user.h, usys.S: Registered the new system call numbers (SYS_mprotect, SYS_munprotect) and added kernel wrappers (sys_mprotect, sys_munprotect) along with user-space stubs to safely pass user pointers and lengths to the kernel.
- prottest.c & Makefile: Created a user test program to verify read-only protection, confirm child process termination on write violations, restore write access via munprotect(), and check error handling for invalid inputs; added _prottest to UPROGS in the Makefile.

Question 2: Priority-Based CPU Scheduler
- proc.h & proc.c: Added an integer priority field (1 to 20) to struct proc and initialized it to 10 in allocproc() for all newly created processes.
- proc.c: Implemented set_priority() to safely update a target process's priority under ptable.lock and modified scheduler() to scan the process table and always select the RUNNABLE process with the smallest priority value.
- syscall.h, syscall.c, sysproc.c, user.h, usys.S: Wired up the set_priority(int pid, int priority) system call across kernel and user space so processes can dynamically modify priorities.
- schedtest.c & Makefile: Wrote a test program where a high-priority parent spawns three CPU-bound children with priorities 5, 10, and 15 to verify strict priority execution order; added _schedtest to UPROGS in the Makefile.
