#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LEN 5

typedef struct HistoryNode {
  char *input;
  struct HistoryNode *next;
} HistoryNode;

HistoryNode *head = NULL;
HistoryNode *tail = NULL;
int history_count = 0;

char *get_input(void);
void add_to_history(char *input);
void remove_oldest_record(void);
void print_history(void);
void free_history(void);

int main(void) {
  while (1) {
    char *input = get_input();
    if (input == NULL) {
      break;
    }
    add_to_history(input);
    if (strcmp(input, "print") == 0) {
      print_history();
    }
  }
  free_history();
  return 0;
}

char *get_input(void) {
  char *line = NULL;
  size_t sz = 0;
  printf("Enter input: ");
  ssize_t len_read = getline(&line, &sz, stdin);
  if (len_read == -1) {
    return NULL;
  }
  line[len_read - 1] = '\0';
  return line;
}

void add_to_history(char *input) {
  HistoryNode *new_node = malloc(sizeof(HistoryNode));
  if (new_node == NULL) {
    perror("malloc failed!");
    free(input);
    exit(-1);
  }
  new_node->input = input;
  new_node->next = NULL;
  if (head == NULL) {
    head = new_node;
    tail = new_node;
  } else {
    tail->next = new_node;
    tail = new_node;
  }
  history_count++;
  if (history_count > MAX_LEN) {
    remove_oldest_record();
  }
}

void remove_oldest_record(void) {
  if (head == NULL) {
    return;
  }
  HistoryNode *old_head = head;
  head = head->next;
  free(old_head->input);
  free(old_head);
  history_count--;
}

void print_history(void) {
  HistoryNode *curr = head;
  while (curr != NULL) {
    printf("%s\n", curr->input);
    curr = curr->next;
  }
}
void free_history() {
  while (head != NULL) {
    remove_oldest_record();
  }
}
