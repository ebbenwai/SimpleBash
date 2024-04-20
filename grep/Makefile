CC = gcc
CFLAGS =  -Wall -Wextra -Werror -std=c11

s21_grep: 
	$(CC) $(CFLAGS) s21_grep.c -o s21_grep

tests: s21_grep
	sh test_func_grep.sh

clean:
	rm -rf s21_grep *.o 

rebuild: clean s21_grep 

