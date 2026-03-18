CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -g

TARGET  = program
SRCS    = main.c digraf.c hrana.c
OBJS    = $(SRCS:.c=.o)
HEADERS = digraf.h hrana.h vrchol.h types.h

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)
