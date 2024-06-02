CC   := gcc
LIBS := lib/stdio.c lib/coro.c lib/cpu.S

all:
	cc -o pc pc.c $(LIBS)
	cc -o pfc pfc.c $(LIBS)

clean:
	rm -f *.o pc
