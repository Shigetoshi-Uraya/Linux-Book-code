#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int fd;
  char buf[] = "testdayo\n";

  if (argc != 3) {
    fprintf(stdout, "Usage: %s src dest\n", argv[0]);
    exit(1);
  }
  fd = open(argv[1], O_RDWR);
  if (fd < 0) {
    perror(argv[1]);
    exit(1);
  }
  printf("open: fd = %d\n", fd);

  if (write(fd, buf, sizeof(buf)) < 0) {
    perror(argv[1]);
    exit(1);
  }
  printf("write: fd = %d\n", fd);

  if (rename(argv[1], argv[2]) < 0) {
    perror(argv[1]);
    exit(1);
  }
  printf("rename: fd = %d\n", fd);
  if (write(fd, buf, sizeof(buf)) < 0) {
    perror(argv[1]);
    exit(1);
  }
  printf("write: fd = %d\n", fd);
  if (close(fd) < 0) {
    perror(argv[1]);
    exit(1);
  }
  printf("close: fd = %d\n", fd);
  exit(0);
}
