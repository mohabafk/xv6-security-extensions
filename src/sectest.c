#include "types.h"
#include "user.h"
#include "fcntl.h"
#include "audit.h"

static int passed = 0;
static int failed = 0;

static void
check(char *name, int condition)
{
  if(condition){
    printf(1, "  [PASS] %s\n", name);
    passed++;
  } else {
    printf(1, "  [FAIL] %s\n", name);
    failed++;
  }
}

static int
can_read(char *path)
{
  int fd = open(path, 0);
  if(fd < 0) return 0;
  close(fd);
  return 1;
}

static int
can_write(char *path)
{
  int fd = open(path, O_WRONLY);
  if(fd < 0) return 0;
  close(fd);
  return 1;
}

int
main(void)
{
  printf(1, "\n=========================================\n");
  printf(1, "  MEDICAL DEVICE SECURITY COMPLIANCE TEST\n");
  printf(1, "  xv6 Security - CCY4304\n");
  printf(1, "=========================================\n\n");

  printf(1, "[PHASE 1] User Authentication\n");
  check("whoami returns valid uid", whoami() >= 0);
  check("Wrong password rejected", login("admin","wrongpass") < 0);
  check("Admin login accepted",    login("admin","admin") == 0);
  check("Patient login accepted",  login("patient","pass") == 1);
  check("Doctor login accepted",   login("doctor","password") == 2);
  login("admin","admin");

  printf(1, "\n[PHASE 2] File Access Control\n");
  printf(1, "  --- As ADMIN ---\n");
  check("Admin reads device/config",     can_read("device/config"));
  check("Admin reads patient/records",   can_read("patient/records"));
  check("Admin reads insulin.log",       can_read("dosage/insulin.log"));

  login("patient","pass");
  printf(1, "  --- As PATIENT (uid=%d) ---\n", whoami());
  check("Patient reads patient/records", can_read("patient/records"));
  check("Patient blocked device/config", !can_read("device/config"));
  check("Patient cannot write records",  !can_write("patient/records"));
  check("Patient reads insulin.log",     can_read("dosage/insulin.log"));

  login("doctor","password");
  printf(1, "  --- As DOCTOR (uid=%d) ---\n", whoami());
  check("Doctor reads patient/records",  can_read("patient/records"));
  check("Doctor writes insulin.log",     can_write("dosage/insulin.log"));
  check("Doctor blocked device/config",  !can_read("device/config"));

  login("admin","admin");
  printf(1, "  --- As ADMIN (uid=%d) ---\n", whoami());
  check("Admin bypasses all - device/config", can_read("device/config"));

  printf(1, "\n[PHASE 3] Syscall Audit Log\n");
  char buf[128 * 20];
  int n = audit_read(buf, sizeof(buf));
  check("Admin reads audit log", n > 0);
  check("Audit log has entries", n > 0);
  login("patient","pass");
  n = audit_read(buf, sizeof(buf));
  check("Patient CANNOT read audit log", n < 0);
  login("admin","admin");
  n = audit_read(buf, sizeof(buf));
  check("Admin audit_read succeeds", n > 0);

  printf(1, "\n=========================================\n");
  printf(1, "  RESULTS: %d passed, %d failed\n", passed, failed);
  if(failed == 0)
    printf(1, "  STATUS: ALL TESTS PASSED ✓\n");
  else
    printf(1, "  STATUS: %d TEST(S) FAILED\n", failed);
  printf(1, "=========================================\n\n");
  exit();
}
