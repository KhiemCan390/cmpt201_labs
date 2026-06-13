
#define _DEFAULT_SOURCE
#define _ISOC99_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
typedef struct header {
  uint64_t size;
  struct header *next;
} header;

void handle_error(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

void print_out(char *format, void *data, size_t data_size) {
  // we can put BUF_SZ to be anything greater than the length of what we print using print_out
  // i.e. first block size: 16
  int BUF_SZ = 100;
  char buf[BUF_SZ];
  // Since in the header struct, we have the uint64_t data type and a pointer,
  // so both of them should have the same size. However, it is still incorrect
  // to store a pointer in an uint63_t and print it out with format %p, since %p
  // expect a pointer (*void), not an int.  Despite this error, the code still works here.
  uint64_t val;
  memcpy(&val, data, data_size);
  int num_char_written = snprintf(buf, BUF_SZ, format, val);
  if (num_char_written < 0) {
    handle_error("snprintf");
  }
  write(STDOUT_FILENO, buf, num_char_written);
}

void initialize_block(header *block, uint64_t size, header *next, int fill_value) {
  block->size = size;
  block->next = next;
  memset(block + 1, fill_value, size - sizeof(header));
}

void print_block(char *block) {
  char *start = block + sizeof(header);
  for (int i = 0; i < 128 - sizeof(header); i++) {
    char *curr = start + i;
    // notes that since curr is a char pointer, so dereferencing it only reads 1 byte
    int val = (int)*curr;
    print_out("%d\n", &val, sizeof(val));
  }
}
int main(void) {
  void *old_break = (char *)sbrk(0);
  if (sbrk(256) == (void *)-1) {
    handle_error("sbrk");
  }
  // void* arithmetics is similar to char8 arithmetic
  header *first_block = old_break;
  header *second_block = old_break + 128;
  // you are passing the address of the pointer first_block here and the size of the pointer, i.e.
  // 8 bytes here. It is because in print_out() we use memcpy(), which we need to pass the address
  // of the source and the number of bytes needed to be copied. Here we are trying to print out an
  // address, so we have to pass a pointer points to that address and the size of that address
  // variable
  print_out("first block: %p\n", &first_block, sizeof(first_block));
  print_out("second block: %p\n", &second_block, sizeof(second_block));
  initialize_block(first_block, 128, NULL, 0);
  initialize_block(second_block, 128, first_block, 1);
  print_out("first block size: %lu\n", &first_block->size, sizeof(first_block->size));
  print_out("first block next: %p\n", &first_block->next, sizeof(first_block->next));
  print_out("second block size: %lu\n", &second_block->size, sizeof(second_block->size));
  print_out("second block next: %p\n", &second_block->next, sizeof(second_block->next));
  print_block((char *)first_block);
  print_block((char *)second_block);
}
