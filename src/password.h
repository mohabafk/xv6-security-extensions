#ifndef PASSWORD_H
#define PASSWORD_H

#define MAX_USERS 8
#define MAX_USERNAME 16
#define MAX_PASSWORD 32

// Role UIDs
#define UID_ADMIN   0
#define UID_PATIENT 1
#define UID_DOCTOR  2

// Simple hash: XOR + multiply (good enough for xv6)
static inline uint
hash_password(const char *pwd)
{
  uint h = 5381;
  while(*pwd)
    h = ((h << 5) + h) ^ (uint)(*pwd++);
  return h;
}

// User entry
struct user_entry {
  char  username[MAX_USERNAME];
  uint  pw_hash;
  int   uid;
  int   gid;
  int   valid;  // 1 = slot in use
};

#endif
