CC = gcc
CFLAGS = -Wall -Wextra -std=c11

OBJ = main.o auth.o budget.o file_handler.o

mypocket: $(OBJ)
	$(CC) $(CFLAGS) -o mypocket $(OBJ)

main.o: main.c budget.h
	$(CC) $(CFLAGS) -c main.c

auth.o: auth.c budget.h
	$(CC) $(CFLAGS) -c auth.c

budget.o: budget.c budget.h
	$(CC) $(CFLAGS) -c budget.c

file_handler.o: file_handler.c budget.h
	$(CC) $(CFLAGS) -c file_handler.c

clean:
	rm -f $(OBJ) mypocket
	#include "budget.h"