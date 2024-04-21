TARGET=app
CC=gcc
FC=-c



$(TARGET): main.o min.o max.o
	$(CC) -o $(TARGET) main.o min.o max.o
min.o: min.c min.h
	$(CC) $(FC) min.c
max.o: max.c max.h
	$(CC) $(FC) max.c
main.o: min.h max.h main.c
	$(CC) $(FC) main.c
