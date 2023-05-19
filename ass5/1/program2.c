#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define FIFO_NAME "myfifo"
#define BUFFER_SIZE 256

int main(int argc, char *argv[]) {
  int fd;
  char buffer[BUFFER_SIZE];

  // Open the named pipe for writing
  fd = open(FIFO_NAME, O_WRONLY);
  if (fd == -1) {
    fprintf(stderr, "Program2: Failed to open FIFO for writing\n");
    return 1;
  }

  // Check if input string is provided as command-line argument
  if (argc > 1) {
    // Write the input string to the named pipe
    write(fd, argv[1], strlen(argv[1]) + 1);
    printf("Program2: Sent data: %s\n", argv[1]);
  } else {
    printf("Program2: Enter data to send: ");
    fgets(buffer, BUFFER_SIZE, stdin);

    // Write the input string to the named pipe
    write(fd, buffer, strlen(buffer) + 1);
    printf("Program2: Sent data: %s\n", buffer);
  }

  // Close the named pipe
  close(fd);

  // Open the named pipe for reading
  fd = open(FIFO_NAME, O_RDONLY);
  if (fd == -1) {
    fprintf(stderr, "Program2: Failed to open FIFO for reading\n");
    return 1;
  }

  // Read acknowledgement from the named pipe
  read(fd, buffer, BUFFER_SIZE);
  printf("Program2: Received acknowledgement: %s\n", buffer);

  // Close the named pipe
  close(fd);

  return 0;
}
