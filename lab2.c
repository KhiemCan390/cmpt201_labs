#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
int main(void) {
  while (1) {
    printf("Enter programs to run.\n");
    printf(">");
    size_t buffer_sz;
    char *str_ptr = NULL;
    ssize_t nread = getline(&str_ptr, &buffer_sz, stdin);
    char *str_copy = malloc(buffer_sz * sizeof(char));
    strcpy(str_copy, str_ptr);
    str_copy[nread - 1] = '\0';
    pid_t pid = fork();
    switch (pid) {
    case -1:
      perror("fork() failed");
      exit(EXIT_FAILURE);
    case 0:
      // child process
      int c_status = execl(str_copy, str_copy, NULL);
      if (c_status == -1) {
        // printf("Exec failure");
        exit(EXIT_FAILURE);
      }
      return 0;
    default:
      int status;
      waitpid(pid, &status, 0);
      if (WIFEXITED(status)) {
        if (WEXITSTATUS(status) != 0) {
          printf("Exec failure\n");
        }
      }
    }
    free(str_ptr);
    free(str_copy);
  }
  return 0;
}
