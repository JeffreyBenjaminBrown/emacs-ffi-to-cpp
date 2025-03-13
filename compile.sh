#!/usr/bin/env bash

CACHE_FILE="where-is-emacs-module-dot-h.txt"

# Check if the cache file exists and contains a valid path
if [ -f "$CACHE_FILE" ] && [ -f "$(cat "$CACHE_FILE")" ]; then
    EMACS_MODULE_PATH=$(cat "$CACHE_FILE")
    echo "Using cached emacs-module.h location: $EMACS_MODULE_PATH"
else
    echo "Searching for emacs-module.h (this may take a while)..."
    EMACS_MODULE_PATH=$(find /nix/store -name emacs-module.h | head -1)

    if [ -z "$EMACS_MODULE_PATH" ]; then
        echo "Error: Could not find emacs-module.h"
        exit 1
    fi

    # Save the path to the cache file
    echo "$EMACS_MODULE_PATH" > "$CACHE_FILE"
    echo "Found and cached emacs-module.h location: $EMACS_MODULE_PATH"
fi

# Make sure this line is correct - the variable needs to be set properly
EMACS_INCLUDE_DIR=$(dirname "$EMACS_MODULE_PATH")
echo "Include directory: $EMACS_INCLUDE_DIR"

# Get source file from command line or use default
SOURCE_FILE=${1:-"example.cpp"}
OUTPUT_FILE=${SOURCE_FILE%.*}.so

# Compile the C++ file into a shared library
echo "Compiling $SOURCE_FILE to $OUTPUT_FILE"
g++ -std=c++17 -Wall -I"$EMACS_INCLUDE_DIR" -shared -fpic "$SOURCE_FILE" -o "$OUTPUT_FILE"

if [ $? -eq 0 ]; then
    echo "Compilation successful: $OUTPUT_FILE created"
else
    echo "Compilation failed"
    exit 1
fi
