all: shell

shell: shell.o csapp.o
	gcc shell.o csapp.o -o shell -g -Wall

shell.o: shell.c
	gcc shell.c -c

csapp.o: csapp.c
	gcc csapp.c -c
