#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(void) {
  size_t buffer_sz = 11;
  char *lineptr = malloc(buffer_sz * sizeof(char));
  ssize_t nread;
  printf("Please enter some text: ");
  nread = getline(&lineptr, &buffer_sz, stdin);
  if (nread < 0) {
    printf("Read failed");
    free(lineptr);
    return -1;
  }
  // char *line_copy = malloc(buffer_sz);
  // strcpy(line_copy, lineptr);
  // line_copy[nread - 1] = '\0';
  // printf("You wrote: \"%s\"\n", line_copy);
  // printf("The number of char read is %zd\n", nread);
  // printf("The buffer size is %zu\n", buffer_sz);
  char *bookmark;
  char *token = strtok_r(lineptr, " ", &bookmark);
  printf("Token: \n");
  while (token != NULL) {
    printf("%s\n", token);
    token = strtok_r(NULL, " ", &bookmark);
  }
  free(lineptr);
  // free(line_copy);
  printf("Pointer(s) freed");
  return 0;
}
