#include "s21_grep.h"

int main(int argc, char *argv[]) { data_process(argc, argv); }

void data_process(int argc, char *argv[]) {
  size_t status = 0, wrds = 0, fls = 0, ptrns = 0;
  int feflags = 0;
  struct grep_flags flags = {0};
  char *list_patterns[argc];
  char **list_words = (char **)calloc(100, sizeof(char *));
  if (list_words == NULL) exit(0);
  allocate_memory(list_words);
  char *list_files[argc];
  separate_arguments(argc, argv, list_patterns, list_words, list_files, &flags,
                     &status, &wrds, &ptrns, &feflags, &fls);
  list_files[fls] = 0;
  list_patterns[ptrns] = 0;
  if (ptrns > 0) {
    read_patterns(list_patterns, list_words, &wrds, &ptrns);
  }
  if (flags.status == 0) {
    file_manager(list_files, list_words, flags, fls);
  }
  free_memory(list_words);
}

void allocate_memory(char **list_words) {
  for (int i = 0; i < 100; i++) {
    list_words[i] = (char *)calloc(100, sizeof(char));
    if (list_words[i] == NULL) {
      for (int y = i - 1; y >= 0; y++) {
        free(list_words[y]);
      }
      free(list_words);
      exit(0);
    }
  }
}

void separate_arguments(int argc, char *argv[], char **list_patterns,
                        char **list_words, char **list_files,
                        struct grep_flags *flags, size_t *status, size_t *wrds,
                        size_t *ptrns, int *feflags, size_t *fls) {
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-' && flags->e == 0 && flags->f == 0) {
      *feflags = f_e_words(argv[i]);
      if (*feflags > 0) {
        list_patterns[*ptrns] = argv[i] + *feflags;
        (*ptrns)++;
        *feflags = 0;
      } else if (*feflags < 0) {
        *feflags = -*feflags;
        strcpy(list_words[*wrds], argv[i] + *feflags);
        (*wrds)++;
        *feflags = 0;
      }
      fill_flags(flags, argv[i], status);
    } else if ((*flags).e == 1 || ((*status) == 0 && (*flags).f == 0)) {
      strcpy(list_words[*wrds], argv[i]);
      (*wrds)++;
      *status = 1;
      (*flags).e = 0;
    } else if ((*flags).f == 1) {
      list_patterns[*ptrns] = argv[i];
      (*ptrns)++;
      *status = 1;
      (*flags).f = 0;
    } else {
      list_files[*fls] = argv[i];
      (*fls)++;
    }
  }
}

void read_patterns(char **list_patterns, char **list_words, size_t *wrds,
                   size_t *ptrns) {
  FILE *ptrn = NULL;
  for (size_t i = 0; i < *ptrns; i++) {
    ptrn = fopen(list_patterns[i], "r");
    if (ptrn) {
      char str[MAX_SIZE];
      while (fgets(str, MAX_SIZE, ptrn) != NULL) {
        for (size_t y = 0; str[y] != 0; y++) {
          if (str[y] == '\n') {
            str[y] = 0;
            break;
          }
          list_words[*wrds][y] = str[y];
        }
        (*wrds)++;
      }
    } else {
      fprintf(stderr, "error: %s: unknown file", list_patterns[i]);
      exit(1);
    }

    fclose(ptrn);
  }
}

void free_memory(char **list_words) {
  for (int i = 0; i < 100; i++) {
    free(list_words[i]);
  }
  free(list_words);
}
int f_e_words(const char *flags) {
  int pos = 0;
  for (int i = 1; flags[i] != 0; i++) {
    if (flags[i] == 'e' && flags[i + 1] != 0) {
      pos = -i - 1;
      break;
    } else if (flags[i] == 'f' && flags[i + 1] != 0) {
      pos = i + 1;
      break;
    }
  }
  return pos;
}

void fill_flags(struct grep_flags *flag_list, const char *flags,
                size_t *status) {
  for (int i = 1; flags[i] != 0; i++) {
    switch (flags[i]) {
      case 'e':
        if (flags[i + 1] == 0) {
          flag_list->e = 1;
          *status = 1;
        } else {
          break;
        }
        break;
      case 'f':
        if (flags[i + 1] == 0) {
          flag_list->f = 1;
          *status = 1;
        } else {
          break;
        }
        break;
      case 'i':
        flag_list->i = 1;
        break;
      case 'v':
        flag_list->v = 1;
        break;
      case 'c':
        flag_list->c = 1;
        break;
      case 'l':
        flag_list->l = 1;
        break;
      case 'n':
        flag_list->n = 1;
        break;
      case 'h':
        flag_list->h = 1;
        break;
      case 's':
        flag_list->s = 1;
        break;
      case 'o':
        flag_list->o = 1;
        break;
      default:
        flag_list->status = 1;
        break;
    }
  }
  if (flag_list->v == 1 && flag_list->o == 1) flag_list->o = 0;
  if (flag_list->l == 1 && flag_list->o == 1) flag_list->o = 0;
}

void file_manager(char **files, char **words, struct grep_flags flags,
                  int fls) {
  FILE *target = NULL;
  for (int i = 0; files[i] != 0; i++) {
    target = fopen(files[i], "r");
    if (target != NULL) {
      str_manager(target, words, flags, files[i], fls);
      fclose(target);
    } else if (flags.s == 1) {
      printf("file %s not found\n", files[i]);
    }
  }
}

void str_manager(FILE *target, char **words, struct grep_flags flags,
                 char *filename, int count_files) {
  char *line = NULL;
  int end_file;
  size_t size = 0;
  int match = 0, num_matches = 0, str = 0, flag = 0;
  while ((end_file = getline(&line, &size, target)) != -1) {
    str++;
    for (int i = 0; words[i][0] != 0; i++) {
      match += word_finder(line, words[i], flags, 0);
      if (match == 0) flag = 1;
      if (flags.v == 0 && match == 0) {
        if (flags.c != 1 && flags.l != 1) {
          if (count_files > 1 && flags.h != 1) printf("%s:", filename);
          if (flags.n == 1) printf("%d:", str);
          if (flags.o != 1) {
            printf("%s", line);
            if (line[strlen(line) - 1] != '\n') printf("\n");
          } else {
            word_finder(line, words[i], flags, 1);
          }
        } else {
          num_matches += 1;
        }
        break;
      }
      match = 0;
    }
    if (flag == 0 && flags.v == 1) {
      if (flags.c != 1 && flags.l != 1) {
        if (count_files > 1 && flags.h != 1) printf("%s:", filename);
        if (flags.n == 1) printf("%d:", str);
        if (flags.o != 1) {
          printf("%s", line);
          if (line[strlen(line) - 1] != '\n') printf("\n");
        }
      } else {
        num_matches += 1;
      }
    }
    flag = 0;
  }
  if (flags.l == 1 && num_matches > 0) num_matches = 1;
  if (flags.c == 1) {
    if (count_files > 1 && flags.h != 1) printf("%s:", filename);
    printf("%d\n", num_matches);
  }
  if (flags.l == 1 && num_matches > 0) printf("%s\n", filename);
  free(line);
}

int word_finder(char *line, char *words, struct grep_flags flags, int print) {
  int result = 0;
  char *str = line;
  regex_t preg;
  regmatch_t mptr[1] = {0};
  int check = regcomp(&preg, words,
                      (flags.i == 1) ? REG_ICASE | REG_EXTENDED : REG_EXTENDED);
  if (check) exit(0);
  result = regexec(&preg, str, 1, mptr, 0);
  if (result != 1 && result != 0) exit(0);
  if (print == 1) {
    while (result == 0) {
      if (mptr[0].rm_so == mptr[0].rm_eo) break;
      printf("%.*s\n", (int)(mptr[0].rm_eo - mptr[0].rm_so),
             str + mptr[0].rm_so);
      str += mptr[0].rm_eo;
      result = regexec(&preg, str, 1, mptr, 0);
    }
  }
  regfree(&preg);
  return result;
}
