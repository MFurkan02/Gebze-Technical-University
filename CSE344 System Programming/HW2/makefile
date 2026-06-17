CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = daemon
SRCS = daemon.c
OBJS = $(SRCS:.c=.o)

# $(wildcard. *c)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
