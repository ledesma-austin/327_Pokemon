queue: main.o queue.o
	gcc main.o queue.o -o queue

main.o: main.c queue.h
	gcc -Wall -Werror -ggdb main.c -c

queue.o: queue.c queue.h
	gcc -Wall -ggdb queue.c -c

clean:
	rm -f queue.o main.o queue
