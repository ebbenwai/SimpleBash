#ifndef GREP_S21_GREP_H_
#define GREP_S21_GREP_H_

#define _GNU_SOURCE

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 1024

struct grep_flags {
  int e;  // Шаблон поиска
  int i;  // Игнорирует различия регистра
  int v;  // Инвертирует смысл поиска соответствий
  int c;  // Выводит только количество совпадающих строк
  int l;  // Выводит только названия совпадающих файлов
  int n;  // Предваряет каждую строку вывода номером строки из файла ввода
  int h;  // Выводит совпадающие строки, не предваряя их именами файлов
  int s;  // Подавляет сообщения об ошибках о несуществующих или нечитаемых
          // файлах
  int f;  // Получает регулярные выражения из файла
  int o;  // Печатает только совпадающие (непустые) части совпавшей строки
  int status;  // Статус корректности флагов
};

void data_process(int argc, char *argv[]);
void allocate_memory(char **list_words);
void separate_arguments(int argc, char *argv[], char **list_patterns,
                        char **list_words, char **list_files,
                        struct grep_flags *flags, size_t *status, size_t *wrds,
                        size_t *ptrns, int *feflags, size_t *fls);
void read_patterns(char **list_patterns, char **list_words, size_t *wrds,
                   size_t *ptrns);
void free_memory(char **list_words);

void file_manager(char **files, char **words, struct grep_flags flags, int fls);
int word_finder(char *line, char *words, struct grep_flags flags, int print);
void fill_flags(struct grep_flags *flag_list, const char *flags,
                size_t *status);
int f_e_words(const char *flags);
void str_manager(FILE *target, char **words, struct grep_flags flags,
                 char *filename, int num_files);

#endif
