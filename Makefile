# CXXFLAGS = -std=c++17
# CC=g++
#
# # Include directories
# INCLUDE=-I /home/yanis/packages/boost -I ./src
#
# # Library directories and libraries
# # LDFLAGS=-L /home/yanis/packages/tqdm.cpp/build -l tqdmlib
#
# main: main.cpp
# 	$(CC) $^ -o $@ $(CXXFLAGS) $(INCLUDE) $(LDFLAGS)
#


# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -ggdb -Iinclude
LDFLAGS =

# Directories
SRCDIR = ./src
BUILDDIR = ./build
TARGET = main lorenz

# Source and object files
SRC = $(wildcard $(SRCDIR)/*.c) main.c
OBJ = $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SRC))

# Main target
all: $(BUILDDIR) $(TARGET)

# Ensure build directory exists
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# Link the final executable
$(TARGET): $(OBJ) $(SRCDIR)/tensor.h
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile source files into object files
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean up build artifacts
clean:
	rm -f $(BUILDDIR)/*.o $(TARGET)
	rmdir $(BUILDDIR)

.PHONY: all clean
