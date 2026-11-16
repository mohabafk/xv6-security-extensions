#include "types.h"
#include "user.h"
#include "audit.h"

static char*
syscall_name(int num)
{
  switch(num){
    case 1:  return "fork";
    case 2:  return "exit";
    case 3:  return "wait";
    case 4:  return "pipe";
    case 5:  return "read";
    case 6:  return "kill";
    case 7:  return "exec";
    case 8:  return "fstat";
    case 9:  return "chdir";
    case 10: return "getpid";
    case 11: return "sbrk";
    case 12: return "sleep";
    case 13: return "uptime";
    case 14: return "open";
    case 15: return "write";
    case 16: return "mknod";
    case 17: return "unlink";
    case 18: return "link";
    case 19: return "mkdir";
    case 20: return "close";
    case 21: return "login";
    case 22: return "whoami";
    case 23: return "useradd";
    case 24: return "userdel";
    case 25: return "passwd";
    case 26: return "chmod";
    case 27: return "chown";
    case 28: return "audit_read";
    default: return "unknown";
  }
}

int
main(void)
{
  if(whoami() != 0){
    printf(1, "auditlog: permission denied (admin only)\n");
    exit();
  }

  char buf[128 * sizeof(struct audit_entry)];
  int n = audit_read(buf, sizeof(buf));
  if(n < 0){
    printf(1, "auditlog: read failed\n");
    exit();
  }

  int count = n / sizeof(struct audit_entry);
  printf(1, "\n=== SYSCALL AUDIT LOG ===\n");
  printf(1, "TICK   PID    UID    SYSCALL        RESULT\n");
  printf(1, "------------------------------------------\n");

  struct audit_entry *e = (struct audit_entry*)buf;
  for(int i = 0; i < count; i++){
    printf(1, "%d\t%d\t%d\t%s\t%d\n",
           e[i].tick,
           e[i].pid,
           e[i].uid,
           syscall_name(e[i].syscall_num),
           e[i].result);
  }
  printf(1, "------------------------------------------\n");
  printf(1, "Total entries: %d\n", count);
  exit();
}
