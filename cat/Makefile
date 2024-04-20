CC = gcc
CFLAGS =  -Wall -Wextra -Werror -std=c11

s21_cat: 
	$(CC) $(CFLAGS) s21_cat.c -o s21_cat

test: s21_cat
	sh test_func_cat.sh

clean:
	rm -rf s21_cat *.o 

rebuild: clean s21_cat

