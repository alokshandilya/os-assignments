#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void set_lock(int fd) {
  struct flock lock;
  lock.l_type = F_WRLCK; // Write lock
  lock.l_whence = SEEK_SET;
  lock.l_start = 0;
  lock.l_len = 0; // Lock the entire file

  if (fcntl(fd, F_SETLKW, &lock) == -1) {
    perror("fcntl");
    exit(1);
  }
}

void release_lock(int fd) {
  struct flock lock;
  lock.l_type = F_UNLCK; // Unlock
  lock.l_whence = SEEK_SET;
  lock.l_start = 0;
  lock.l_len = 0; // Unlock the entire file

  if (fcntl(fd, F_SETLK, &lock) == -1) {
    perror("fcntl");
    exit(1);
  }
}

int main() {
  int fd = open("file.txt", O_RDONLY);
  if (fd == -1) {
    perror("open");
    exit(1);
  }

  set_lock(fd);

  char buffer[100];
  ssize_t bytesRead = read(fd, buffer, sizeof(buffer) - 1);
  if (bytesRead == -1) {
    perror("read");
    exit(1);
  }
  buffer[bytesRead] = '\0';
  printf("Process %d: Read from file: %s\n", getpid(), buffer);

  release_lock(fd);

  close(fd);

  return 0;
}
