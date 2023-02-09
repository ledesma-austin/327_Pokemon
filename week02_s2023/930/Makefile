all: stack

stack: main.o stack.o
	gcc main.o stack.o -o stack

main.o: main.c stack.h
	gcc -Wall -Werror -ggdb main.c -c

stack.o: stack.c stack.h
	gcc -Wall -Werror -ggdb stack.c -c

clean:
	rm -f main.o stack.o stack
