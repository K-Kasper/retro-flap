SRC_DIR = src
BIN_DIR = bin
LINUX_DIR = $(BIN_DIR)/linux
WIN_DIR = $(BIN_DIR)/windows

SOURCES = $(wildcard $(SRC_DIR)/*.c)

EXECUTABLE_NAME = RetroFlap
LINUX_EXECUTABLE = $(LINUX_DIR)/$(EXECUTABLE_NAME).x86_64
WIN_EXECUTABLE = $(WIN_DIR)/$(EXECUTABLE_NAME).exe

CFLAGS = -Wall -std=c99 -Iinclude
LDFLAGS = -lSDL2 -lSDL2_ttf -lSDL2_mixer
LINUX_CC = gcc
WIN_CC = x86_64-w64-mingw32-gcc
WIN_CFLAGS = -mwindows
WIN_LDFLAGS = -Llib

all: directories linux windows

directories:
	mkdir -p $(LINUX_DIR)
	mkdir -p $(WIN_DIR)

linux: $(LINUX_EXECUTABLE)

windows: $(WIN_EXECUTABLE)

$(LINUX_EXECUTABLE): $(SOURCES)
	$(LINUX_CC) $(CFLAGS) $(SOURCES) $(LDFLAGS) -o $@

$(WIN_EXECUTABLE): $(SOURCES)
	$(WIN_CC) $(WIN_CFLAGS) $(CFLAGS) $(SOURCES) $(WIN_LDFLAGS) $(LDFLAGS) -o $@

run:
	$(LINUX_EXECUTABLE)

winrun:
	$(WIN_EXECUTABLE)

clean:
	rm -f $(LINUX_EXECUTABLE) $(WIN_EXECUTABLE)
