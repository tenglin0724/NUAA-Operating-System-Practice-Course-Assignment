CC 	:= gcc
CFLAGS  := -O -Wall -Wno-unused -Wno-unused-result -Wno-parentheses -Wundef

all:
	$(CC) $(CFLAGS) -o httpd test.c tcp.c -lpthread
