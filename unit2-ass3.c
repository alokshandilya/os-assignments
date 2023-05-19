// The signal() system call in C is used to set up signal handlers for specific
// signals. Signals are asynchronous notifications that can be sent to a process
// to indicate certain events or conditions. For example, the INTERRUPT FROM THE
// KEYBOARD signal (SIGINT) is sent to a process when the user presses Ctrl+C.
// The signal() function allows you to define a custom signal handler function
// that will be executed when a specific signal is received by the process.



#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_TERMS 100

void signal_handler(int signum);

int main() {
  pid_t pid;
  int pipefd[2];
  int n, i;
  int fib[MAX_TERMS];

  // Creating pipe
  if (pipe(pipefd) == -1) {
    perror("Pipe creation failed");
    exit(EXIT_FAILURE);
  }

  // Registering signal handler
  signal(SIGINT, signal_handler);

  // Forking a child process
  pid = fork();

  if (pid < 0) {
    perror("Fork failed");
    exit(EXIT_FAILURE);
  }

  if (pid == 0) {
    // Child process
    close(pipefd[0]); // Close the read end of the pipe

    printf("Enter the number of terms to generate Fibonacci series: ");
    scanf("%d", &n);

    fib[0] = 0;
    fib[1] = 1;

    // Generate Fibonacci series terms up to n
    for (i = 2; i < n; i++) {
      fib[i] = fib[i - 1] + fib[i - 2];
    }

    // Send child process ID to the parent process
    write(pipefd[1], &pid, sizeof(pid_t));

    // Send Fibonacci series terms one by one
    for (i = 0; i < n; i++) {
      write(pipefd[1], &fib[i], sizeof(int));
    }

    // Send the signal ID for the handler function
    int signal_id = SIGINT;
    write(pipefd[1], &signal_id, sizeof(int));

    close(pipefd[1]); // Close the write end of the pipe
    exit(EXIT_SUCCESS);
  } else {
    // Parent process
    close(pipefd[1]); // Close the write end of the pipe

    // Read child process ID from the pipe
    pid_t child_pid;
    read(pipefd[0], &child_pid, sizeof(pid_t));
    printf("Child process ID: %d\n", child_pid);

    // Read and display Fibonacci series terms one by one
    int term;
    printf("Fibonacci series: ");
    while (read(pipefd[0], &term, sizeof(int)) > 0) {
      printf("%d ", term);
    }
    printf("\n");

    // Read and display the signal ID for the handler function
    int signal_id;
    read(pipefd[0], &signal_id, sizeof(int));
    printf("Signal ID: %d\n", signal_id);

    close(pipefd[0]); // Close the read end of the pipe
    exit(EXIT_SUCCESS);
  }

  return 0;
}

// Signal handler function
void signal_handler(int signum) {
  printf("\nInterrupt signal received. Exiting...\n");
  exit(signum);
}
