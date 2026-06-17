# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
LDFLAGS = -lpthread

# Executables
TARGETS = BankServer BankClient 

# Source files
SRCS = BankServer.c BankClient.c
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGETS)

# Individual targets
BankServer: BankServer.o Bank.h
	$(CC) $(CFLAGS) -o $@ BankServer.o $(LDFLAGS)

BankClient: BankClient.o Bank.h
	$(CC) $(CFLAGS) -o $@ BankClient.o $(LDFLAGS)


# Object files
%.o: %.c Bank.h
	$(CC) $(CFLAGS) -c $< -o $@

# Cleanup
clean:
	rm -f $(TARGETS) $(OBJS) *.log
	rm -f AdaBank.bankLog bank_server_fifo bank_client_fifo_* BankDatabase.db

# Phony targets
.PHONY: all clean