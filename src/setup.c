#include "types.h"
#include "user.h"
#include "fcntl.h"

static void
mkdirp(char *path)
{
  mkdir(path);
}

static void
mkfile(char *path, char *content, int uid, int gid, int mode)
{
  int fd;
  fd = open(path, O_CREATE | O_WRONLY);
  if(fd < 0){
    printf(1, "setup: cannot create %s\n", path);
    return;
  }
  if(content)
    write(fd, content, strlen(content));
  close(fd);
  int r1 = chown(path, uid, gid);
  int r2 = chmod(path, mode);
  printf(1, "  %s: chown=%d chmod=%d mode=%d\n", path, r1, r2, mode);
}

int
main(void)
{
  if(whoami() != 0){
    printf(1, "setup: must run as admin\n");
    exit();
  }

  printf(1, "Setting up medical filesystem...\n");

  mkdirp("patient");
  mkdirp("dosage");
  mkdirp("device");
  mkdirp("audit");

  mkfile("patient/records",
         "Patient: John Doe\nDOB: 1990-01-01\nDiagnosis: Type 1 Diabetes\n",
         1, 1, 0444);

  mkfile("dosage/insulin.log",
         "2024-01-01 08:00 - 10 units rapid\n2024-01-01 12:00 - 8 units rapid\n",
         2, 2, 0644);

  mkfile("device/config",
         "pump_rate=5\nmax_dose=20\nalarm_threshold=180\n",
         0, 0, 0600);

  mkfile("audit/syscall.log",
         "AUDIT LOG INITIALIZED\n",
         0, 0, 0600);

  printf(1, "Done! uid=%d\n", whoami());
  exit();
}
