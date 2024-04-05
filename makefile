# Compiler and linker configuration
CC=gcc
CFLAGS=-ansi -pedantic -Wall
LDFLAGS=

# Project files
SOURCES=main.c first_pass.c second_pass.c parser.c macro_processor.c \
        symbol_table.c data_instruction.c code_instruction.c \
        error_handling.c output_files.c utils.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=assembler

# Default target
all: $(SOURCES) $(EXECUTABLE)

# Linking
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

# Compilation
.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

# Phony targets
.PHONY: all clean
