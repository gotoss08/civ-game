PLATFORM ?= LINUX

ifeq ($(PLATFORM),WSL2)
	RAYLIB_PATH = /mnt/c/code/_libs/raylib-5.5_win64_mingw-w64
	CC = x86_64-w64-mingw32-gcc
	CFLAGS = -Wall -Wextra -pedantic -I$(RAYLIB_PATH)/include
	LINKER_FLAGS = -L$(RAYLIB_PATH)/lib -lraylib -lgdi32 -lwinmm
else
	CC = gcc
	CFLAGS = -Wall -Wextra -pedantic
	LINKER_FLAGS = -lraylib -lm -lGL
endif

# List of source files
SOURCES = src/main.c src/engine.c src/game.c

# Header files
HEADERS = src/engine.h src/game.h src/astar.h

# Object files (generated in the build directory)
OBJECTS = $(patsubst src/%.c, build/%.o, $(SOURCES))

# Target: executable name
TARGET = bin/game.exe

# Create directories if needed
.PHONY: all build dirs
all: dirs $(TARGET)

dirs:
	@mkdir -p build bin

# Rule to build the executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LINKER_FLAGS)

# Rule to create object files from source files
build/%.o: src/%.c $(HEADERS)
	$(CC) -c $(CFLAGS) $< -o $@

# Run target
run: all
	$(TARGET)

# Clean target to remove object files and executable
clean:
	rm -f $(OBJECTS) $(TARGET)
