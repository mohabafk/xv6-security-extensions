#include "types.h"
#include "user.h"
#include "password.h"

#define MAX_INPUT 32

static void
gets_noecho(char *buf, int max)
{
  int i = 0;
  char c;
  while(i < max - 1){
    if(read(0, &c, 1) <= 0) break;
    if(c == '\n' || c == '\r') break;
    buf[i++] = c;
  }
  buf[i] = '\0';
}

int
main(void)
{
  char username[MAX_USERNAME];
  char password[MAX_PASSWORD];
  int uid;

  while(1){
    // Print login prompt
    printf(1, "\n=== xv6 Medical Device Login ===\n");
    printf(1, "Username: ");
    gets(username, MAX_USERNAME);
    // Remove newline
    for(int i = 0; username[i]; i++)
      if(username[i] == '\n') { username[i] = 0; break; }

    printf(1, "Password: ");
    gets_noecho(password, MAX_PASSWORD);
    printf(1, "\n");

    uid = login(username, password);
    if(uid < 0){
      printf(1, "Login failed: invalid credentials\n");
      continue;
    }

    // Success — show role
    if(uid == UID_ADMIN)
      printf(1, "Logged in as ADMIN\n");
    else if(uid == UID_PATIENT)
      printf(1, "Logged in as PATIENT\n");
    else if(uid == UID_DOCTOR)
      printf(1, "Logged in as DOCTOR\n");

    // Exec shell
    char *argv[] = { "sh", 0 };
    exec("sh", argv);
    printf(1, "exec sh failed\n");
    exit();
  }
  exit();
}
