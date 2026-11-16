struct file {
  enum { FD_NONE, FD_PIPE, FD_INODE } type;
  int ref; // reference count
  char readable;
  char writable;
  struct pipe *pipe;
  struct inode *ip;
  uint off;
};


// in-memory copy of an inode
struct inode {
  uint dev;
  uint inum;
  int ref;
  struct sleeplock lock;
  int valid;
  short type;
  short major;
  short minor;
  short nlink;
  uint size;
  short uid;          // owner user ID
  short gid;          // owner group ID
  short mode;         // permission bits
  uint addrs[NDIRECT+1];
};
// table mapping major device number to
// device functions
struct devsw {
  int (*read)(struct inode*, char*, int);
  int (*write)(struct inode*, char*, int);
};

extern struct devsw devsw[];

#define CONSOLE 1
