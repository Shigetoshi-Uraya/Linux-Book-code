#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int fd;
  char buf[] = "test\n";

  if (argc != 2) {
    fprintf(stdout, "Usage: %s filename\n", argv[0]);
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
  if (unlink(argv[1]) < 0) {
    perror(argv[1]);
    exit(1);
  }
  printf("unlink: fd = %d\n", fd);
  // printf("sleep(10)\n");
  //  sleep(10);
  //ファイルシステムから消えているのかlsで確認する
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
