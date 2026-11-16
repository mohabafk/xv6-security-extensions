#include "types.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "x86.h"
#include "spinlock.h"
#include "proc.h"
#include "audit.h"

static struct audit_buf abuf;
static struct spinlock audit_lock;

void
audit_init(void)
{
  initlock(&audit_lock, "audit");
  abuf.head  = 0;
  abuf.count = 0;
}

void
audit_log(int syscall_num, int result)
{
  struct proc *p = myproc();
  if(p == 0) return;

  acquire(&audit_lock);

  int idx = abuf.head % AUDIT_BUFFER_SIZE;
  abuf.entries[idx].pid         = p->pid;
  abuf.entries[idx].uid         = p->uid;
  abuf.entries[idx].syscall_num = syscall_num;
  abuf.entries[idx].tick        = ticks;
  abuf.entries[idx].result      = result;
  abuf.head++;
  if(abuf.count < AUDIT_BUFFER_SIZE)
    abuf.count++;

  release(&audit_lock);
}

int
audit_read(char *dst, int max)
{
  struct proc *p = myproc();
  if(p == 0 || p->uid != 0)
    return -1;

  acquire(&audit_lock);

  int n = abuf.count;
  if(n > max / (int)sizeof(struct audit_entry))
    n = max / (int)sizeof(struct audit_entry);

  int start = 0;
  if(abuf.count == AUDIT_BUFFER_SIZE)
    start = abuf.head % AUDIT_BUFFER_SIZE;

  for(int i = 0; i < n; i++){
    int idx = (start + i) % AUDIT_BUFFER_SIZE;
    memmove(dst + i * sizeof(struct audit_entry),
            &abuf.entries[idx],
            sizeof(struct audit_entry));
  }

  release(&audit_lock);
  return n * sizeof(struct audit_entry);
}
