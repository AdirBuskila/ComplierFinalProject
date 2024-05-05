CC = gcc
CFLAGS = -Wall -g
OBJ = assembler.o
EXEC = assembler

# Default target
all: $(EXEC)

# Link the object file to create the executable
$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@.exe $^

# Compile the assembler.c to an object file
assembler.o: assembler.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean target for cleaning up the compiled files
clean:
	rm -f $(OBJ) $(EXEC).exe