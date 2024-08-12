#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char *fmtname(char *path) {
  static char buf[DIRSIZ + 1];
  char *p;

  // Find first character after last slash.
  for (p = path + strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if (strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf + strlen(p), ' ', DIRSIZ - strlen(p));
  return buf;
}

void find(char *dir, char *file) {

  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;
  
  if ((fd = open(dir, O_RDONLY)) < 0) {
    fprintf(2, "find: cannot open %s\n", dir);
    return;
  }

  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot fstat %s\n", dir);
    close(fd);
    return;
  }
  if (st.type != T_DIR) {
    printf("%s is not a directory:\n", dir);
    return;
  }
   
  if (strlen(file) + 1 + DIRSIZ + 1 > sizeof buf) {
    printf("find: path too long\n");
    exit(1);
  }
  strcpy(buf, dir);
  p = buf + strlen(dir);
  *p++ = '/';
  
  // buffer is directory/
  while (read(fd, &de, sizeof(de)) == sizeof(de)) {
    if (de.inum == 0)
      continue;
    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;
    if (stat(buf, &st) < 0) {
      printf("ls: cannot fstat %s\n", buf);
      continue;
    }

    if (st.type == T_DIR) {
      if (strcmp(p, ".") == 0 || strcmp(p, "..") == 0) {
        continue;
      } else {
        find(buf, file);
      }
    }
    if (strcmp(p, file) == 0) {
      printf("%s\n", buf);
    }
  }
  close(fd);
}

int main(int argc, char *argv[]) {

  if (argc < 2) {
    exit(0);
  }
  find(argv[1], argv[2]);
  exit(0);
}
