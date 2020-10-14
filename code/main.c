#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
  int status;
  pid_t child_pid = fork();

  if (child_pid == 0) {
    printf("Child!\n");

    exit(0);
  }
  else {
    // waitpid(child_pid, &status, 0);

    printf("Parent!\n");
  }
}