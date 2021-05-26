#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
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

//  uint64 fp = r_fp();
//  printf("backtrace1:\n");
//  backtrace1(fp);
  printf("backtrace:\n");
  backtrace();

  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_sigalarm(void) {
  struct proc *p = myproc();

  int n;
  if (argint(0, &n) < 0)
    return -1;
  p->trigger = n;

  uint64 pointer;
  if (argaddr(1, &pointer) <0)
    return -1;
  p->handler_p = pointer;

  return 0;
}

uint64
sys_sigreturn(void) {
  struct proc *p = myproc();
  p->trapframe->ra = p->saved->ra;
  p->trapframe->sp = p->saved->sp;
  p->trapframe->gp = p->saved->gp;
  p->trapframe->tp = p->saved->tp;
  p->trapframe->t0 = p->saved->t0;
  p->trapframe->t1 = p->saved->t1;
  p->trapframe->t2 = p->saved->t2;
  p->trapframe->s0 = p->saved->s0;
  p->trapframe->s1 = p->saved->s1;
  p->trapframe->a0 = p->saved->a0;
  p->trapframe->a1 = p->saved->a1;
  p->trapframe->a2 = p->saved->a2;
  p->trapframe->a3 = p->saved->a3;
  p->trapframe->a4 = p->saved->a4;
  p->trapframe->a5 = p->saved->a5;
  p->trapframe->a6 = p->saved->a6;
  p->trapframe->a7 = p->saved->a7;
  p->trapframe->s2 = p->saved->s2;
  p->trapframe->s3 = p->saved->s3;
  p->trapframe->s4 = p->saved->s4;
  p->trapframe->s5 = p->saved->s5;
  p->trapframe->s6 = p->saved->s6;
  p->trapframe->s7 = p->saved->s7;
  p->trapframe->s8 = p->saved->s8;
  p->trapframe->s9 = p->saved->s9;
  p->trapframe->s10 = p->saved->s10;
  p->trapframe->s11 = p->saved->s11;
  p->trapframe->t3 = p->saved->t3;
  p->trapframe->t4 = p->saved->t4;
  p->trapframe->t5 = p->saved->t5;
  p->trapframe->t6 = p->saved->t6;
  p->trapframe->epc = p->saved->ppc;
  p->tickscnt = 0;
  return 0;
}
