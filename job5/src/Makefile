CC           := gcc
LD           := gcc
CFLAGS       :=
LFLAGS       := -lssl -lcrypto
SRCS         := $(wildcard *.c utils/*.c)
OBJS         := $(subst .c,.o,$(SRCS))
TARGET       := dg 

all:
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS) $(LFLAGS)

clean:
	rm -f $(TARGET) $(OBJS)
