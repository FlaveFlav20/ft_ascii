# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Iinclude

# Directories
SRC_DIR = src
OBJ_DIR = obj
INCLUDE_DIR = include

# Source files
SRC_FILES = draw.c init.c keyhooks.c pixel.c utils.c
SRCS = $(patsubst %, $(SRC_DIR)/%, $(SRC_FILES))

# Object files
OBJS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

# Target library
LIB = libftascii.a
DYN_LIB = libftascii.so

EXAMPLE = example
EXAMPLE_SRC = main.c

STICKMAN_EXAMPLE = stickman
STICKMAN_EXAMPLE_SRC = Examples/stickman.c

EXAMPLE_OBJ= $(OBJ_DIR)/main.o
STICKMAN_EXAMPLE_OBJ= $(OBJ_DIR)/stickman.o

# Create object directory if it doesn't exist
$(shell mkdir -p $(OBJ_DIR))

# Default target
all: $(LIB) $(EXAMPLE) $(DYN_LIB) $(STICKMAN_EXAMPLE)

# Compile each source file into an object file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile the example program object file
$(OBJ_DIR)/main.o: $(EXAMPLE_SRC)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/stickman.o: $(STICKMAN_EXAMPLE_SRC)
	$(CC) $(CFLAGS) -c $< -o $@

# Archive the object files into a static library
$(LIB): $(OBJS)
	ar rcs $(LIB) $(OBJS)


CFLAGS += -fPIC
$(DYN_LIB): $(OBJS)
	gcc -fPIC $(OBJS) -shared -o $(DYN_LIB)

# Link the example program
$(EXAMPLE): $(EXAMPLE_OBJ) $(LIB)
	$(CC) $(CFLAGS) $(EXAMPLE_OBJ) -L. -lftascii -lm -o $(EXAMPLE)

$(STICKMAN_EXAMPLE): $(STICKMAN_EXAMPLE_OBJ) $(DYN_LIB)
	$(CC) $(CFLAGS) $(STICKMAN_EXAMPLE_OBJ) -L. -lm -Wl,-rpath=.  -lm -lftascii -o $(STICKMAN_EXAMPLE)

# Clean up the build files
clean:
	$(RM) $(OBJ_DIR)/*.o $(LIB) $(EXAMPLE) $(STICKMAN_EXAMPLE) $(DYN_LIB)

re: clean all

run: all
	./$(EXAMPLE)
