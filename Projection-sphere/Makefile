# Compiler and flags
CC = gcc
CFLAGS = -Wall -g -o2

# SDL2 includes and libs
SDL2_CFLAGS = $(shell sdl2-config --cflags)
SDL2_LDFLAGS = $(shell sdl2-config --libs)

# Lib maths
LM_CFLAGS = -lm

# Executable and sources
TARGET = prog
SRCS = main.c
OBJS = $(SRCS:.c=.o)

# Build the target
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(SDL2_LDFLAGS) $(LM_CFLAGS)


# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) $(SDL2_CFLAGS) $(LM_CFLAGS) -c $< -o $@


# Clean build files
clean:
	rm -f $(OBJS) $(TARGET)
