# Compiler
CC = gcc

# Flags
CFLAGS = -Wall -Wextra -g

# Target binary
TARGET = uart_app

# Source files
SRCS = main.c uart.c

# Object files
OBJS = $(SRCS:.c=.o)

# Default rule
all: $(TARGET)

# Link step
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile step
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(TARGET) $(OBJS)

# Rebuild everything
rebuild: clean all

.PHONY: all clean rebuild
