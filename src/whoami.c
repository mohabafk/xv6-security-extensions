#include "types.h"
#include "user.h"

int main(void){
  int uid = whoami();
  if(uid == 0) printf(1, "admin (uid=0)\n");
  else if(uid == 1) printf(1, "patient (uid=1)\n");
  else if(uid == 2) printf(1, "doctor (uid=2)\n");
  else printf(1, "uid=%d\n", uid);
  exit();
}
