OPTIONS := -g -fsanitize=address -fno-common

all: global local malloc free

global: global.c
	cc -o global $(OPTIONS) global.c

local: local.c
	cc -o local $(OPTIONS) local.c

malloc: malloc.c
	cc -o malloc $(OPTIONS) malloc.c

free: free.c
	cc -o free $(OPTIONS) free.c

clean:
	rm -f global local malloc free
