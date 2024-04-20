#include "s21_cat.h"

int main(int argc, char* argv[]) {
  if (argc < 2) {
    printf("Использование: %s [опции] файл1 файл2 ...\n", argv[0]);
    return 1;
  }

  Options options = parse_flags(argc, argv);

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] != '-' || argv[i][1] == '-') {
      process_file(argv[i], options);
    }
  }

  return 0;
}

Options parse_flags(int argc, char* argv[]) {
  Options options = {false, false, false, false, false, false};

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-' && argv[i][1] != '-') {
      for (int j = 1; argv[i][j] != '\0'; j++) {
        switch (argv[i][j]) {
          case 'b':
            options.number_non_blank_lines = true;
            break;
          case 'e':
            options.show_ends = true;
            options.show_nonprinting = true;
            break;
          case 'E':
            options.show_ends = true;
            break;
          case 'n':
            options.number_lines = true;
            break;
          case 's':
            options.squeeze_blank_lines = true;
            break;
          case 't':
            options.show_tabs = true;
            options.show_nonprinting = true;
            break;
          case 'T':
            options.show_tabs = true;
            break;
          case 'v':
            options.show_nonprinting = true;
            break;
          default:
            printf("Неизвестная опция: %c\n", argv[i][j]);
            exit(1);
        }
      }
    } else if (argv[i][0] == '-' && argv[i][1] == '-') {
      if (strcmp(argv[i], "--number") == 0) {
        options.number_lines = true;
      } else if (strcmp(argv[i], "--number-nonblank") == 0) {
        options.number_non_blank_lines = true;
      } else if (strcmp(argv[i], "--squeeze-blank") == 0) {
        options.squeeze_blank_lines = true;
      }
    }
  }

  return options;
}

void print_line_number(int line_number) { printf("%6d\t", line_number); }

void print_non_blank_line_number(int* line_number, unsigned char* line) {
  if (strcmp((char*)line, "\n") != 0) {
    printf("%6d\t", *line_number);
    *line_number = *line_number + 1;
  }
}

void print_line(unsigned char* line, bool show_ends, bool show_tabs,
                bool show_nonprinting) {
  for (int i = 0; line[i] != '\0'; i++) {
    switch (line[i]) {
      case '\n':
        if (show_ends) {
          printf("$\n");
        } else {
          putchar(line[i]);
        }
        break;
      case '\t':
        if (show_tabs) {
          printf("^I");
        } else {
          putchar(line[i]);
        }
        break;
      default:
        if (show_nonprinting) {
          if (line[i] < 32 && line[i] != '\t' && line[i] != '\n') {
            printf("^%c", line[i] + 64);
          } else if (line[i] == 127) {
            printf("^?");
          } else if (line[i] > 127 && line[i] < 160) {
            printf("^%c", line[i] - 128 + 64);
          } else if (line[i] > 159) {
            printf("M-%c", line[i] - 64);
          } else {
            putchar(line[i]);
          }
        } else {
          putchar(line[i]);
        }
    }
  }
}

void cat(FILE* file, Options options) {
  unsigned char line[MAX_LINE_LENGTH];
  int line_number = 1;
  bool previous_line_blank = false;

  while (fgets((char*)line, sizeof(line), file)) {
    if (options.squeeze_blank_lines && previous_line_blank && line[0] == '\n') {
      continue;
    }

    if (options.number_non_blank_lines) {
      print_non_blank_line_number(&line_number, line);
    } else if (options.number_lines) {
      print_line_number(line_number);
    }

    print_line(line, options.show_ends, options.show_tabs,
               options.show_nonprinting);
    previous_line_blank = (line[0] == '\n');

    if (!options.number_non_blank_lines) {
      line_number++;
    }
  }
}

void process_file(char* filename, Options options) {
  FILE* file = fopen(filename, "rb");

  if (file == NULL) {
    printf("Ошибка при открытии файла: %s\n", filename);
    exit(1);
  }

  cat(file, options);
  fclose(file);
}