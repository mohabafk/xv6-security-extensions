# xv6 Security Extensions

An academic operating systems security project documenting
user authentication, file access control, and syscall auditing in xv6.

## Repository Contents
This repository contains the xv6 security source code in [src/](src/),
along with an academic project report.

The report describes an earlier version. Some implementation details
differ from the included source code.
## Security Features Documented

### User Authentication
- User and group identifiers associated with processes.
- Login before accessing the shell.
- User-management and identity-related system calls.
- Inheritance of process identity during fork.

### File Access Control
- Ownership and permission fields added to inodes.
- Access checks in the file-open path.
- Administrative operations for changing ownership and permissions.
- Access tests using different user identities.

### Syscall Auditing
- A kernel ring buffer for recording syscall activity.
- Audit entries containing process ID, user ID, syscall number,
  timestamp, and a description.
- An administrator-only interface for reading audit records.

## Technologies
- C
- xv6
- QEMU
- Linux
- Operating system system calls and filesystem internals

## Skills Practiced
- Kernel programming
- Authentication and access control
- Filesystem permissions
- Security logging
- Testing and technical documentation

## Limitations
This is an educational prototype, not a production security system.

The source code uses a simple non-cryptographic password hash.
It is unsuitable for protecting real passwords.

The audit log uses a fixed-size in-memory buffer, so older
entries can be overwritten and records do not persist across reboot.

## Project Report
[Download the project report](https://github.com/mohabafk/xv6-security-extensions/raw/refs/heads/main/xv6-security-report.docx)

## Source Code and Credits
Browse the implementation in [src/](src/).

Based on MIT's x86 xv6-public teaching operating system.
The original license is preserved in [src/LICENSE](src/LICENSE).

## Verification Status
Build and runtime verification of this published source copy
are pending.

## Author
Mohab Amr Elmansoury
