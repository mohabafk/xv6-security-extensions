#ifndef AUDIT_H
#define AUDIT_H

#define AUDIT_BUFFER_SIZE 128

struct audit_entry {
  int  pid;
  int  uid;
  int  syscall_num;
  uint tick;
  int  result;
};

struct audit_buf {
  struct audit_entry entries[AUDIT_BUFFER_SIZE];
  int head;
  int count;
};

#endif
