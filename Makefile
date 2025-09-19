.PHONY: all build linux windows clean install uninstall

CC          = gcc
CC_WINDOWS  = x86_64-w64-mingw32-gcc
CFLAGS      = -O2 -Wall -Wextra -std=c99
STRIPFLAGS  = -s
SRC         = laa.c
BUILD_DIR   = build

TARGET_LINUX    = $(BUILD_DIR)/linux/laa
TARGET_WINDOWS  = $(BUILD_DIR)/windows/laa.exe

INSTALL_DIR = /usr/local/bin

all: build

build: linux windows

linux: $(TARGET_LINUX)

windows: $(TARGET_WINDOWS)

$(TARGET_LINUX): $(SRC) | $(BUILD_DIR)/linux
    @echo "Building static Linux binary (fallback to dynamic if needed)..."
    @$(CC) $(CFLAGS) $(STRIPFLAGS) -static -o $@ $< 2>/dev/null || \
     $(CC) $(CFLAGS) $(STRIPFLAGS) -o $@ $<

$(TARGET_WINDOWS): $(SRC) | $(BUILD_DIR)/windows
    @echo "Building Windows binary..."
    @$(CC_WINDOWS) $(CFLAGS) $(STRIPFLAGS) -o $@ $<

$(BUILD_DIR)/linux $(BUILD_DIR)/windows:
    @mkdir -p $@

install: $(TARGET_LINUX)
    sudo install -m 0755 $< $(INSTALL_DIR)/laa

uninstall:
    sudo rm -f $(INSTALL_DIR)/laa

clean:
    rm -rf $(BUILD_DIR)
