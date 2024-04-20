#ifndef CAT_S21_CAT_H
#define CAT_S21_CAT_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

typedef struct {
  bool number_lines;
  bool number_non_blank_lines;
  bool squeeze_blank_lines;
  bool show_ends;
  bool show_tabs;
  bool show_nonprinting;
} Options;

Options parse_flags(int argc, char* argv[]);
void print_line_number(int line_number);
void print_non_blank_line_number(int* line_number, unsigned char* line);
void print_line(unsigned char* line, bool show_ends, bool show_tabs,
                bool show_nonprinting);
void cat(FILE* file, Options options);
void process_file(char* filename, Options options);

#endif