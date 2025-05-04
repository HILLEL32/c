# Makefile for myShell project

# Target executable
TARGET = myShell

# Source files
SRC = myFuncShell.c

# Compiler and flags
CC = gcc
CFLAGS = -Wall -g

# Default rule
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Cleanup
clean:
	rm -f $(TARGET)
