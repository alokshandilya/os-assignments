#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define FIFO_NAME "myfifo"
#define BUFFER_SIZE 256

int main() {
  int fd;
  char buffer[BUFFER_SIZE];

  // Create the named pipe
  mkfifo(FIFO_NAME, 0666);

  printf("Program1: FIFO created\n");

  // Open the named pipe for reading
  fd = open(FIFO_NAME, O_RDONLY);
  if (fd == -1) {
    fprintf(stderr, "Program1: Failed to open FIFO for reading\n");
    return 1;
  }

  printf("Program1: Waiting for data...\n");

  // Read data from the named pipe
  read(fd, buffer, BUFFER_SIZE);
  printf("Program1: Received data: %s\n", buffer);

  // Close the named pipe
  close(fd);

  // Open the named pipe for writing
  fd = open(FIFO_NAME, O_WRONLY);
  if (fd == -1) {
    fprintf(stderr, "Program1: Failed to open FIFO for writing\n");
    return 1;
  }

  // Write acknowledgement to the named pipe
  strcpy(buffer, "ACKNOWLEDGEMENT");
  write(fd, buffer, strlen(buffer) + 1);
  printf("Program1: Acknowledgement sent\n");

  // Close the named pipe
  close(fd);

  // Remove the named pipe
  unlink(FIFO_NAME);

  printf("Program1: FIFO removed\n");

  return 0;
}
