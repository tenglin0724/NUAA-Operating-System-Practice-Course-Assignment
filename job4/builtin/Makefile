sh: main.o parse.o cmd.o
	cc -o sh main.o parse.o cmd.o
main.o: main.c cmd.h parse.h
	cc -c main.c
cmd.o: cmd.c cmd.h
	cc -c cmd.c
parse.o: parse.c parse.h
	cc -c parse.c


