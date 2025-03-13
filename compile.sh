CACHE_FILE="volatile/where-is-emacs-module-dot-h.txt"
ITERATION_FILE="volatile/iteration-number.txt"

# Make sure some folders exist
mkdir -p volatile lib

# Check for a valid cached path to emacs-module.h
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

EMACS_INCLUDE_DIR=$(dirname "$EMACS_MODULE_PATH")
echo "Include directory: $EMACS_INCLUDE_DIR"

# Get source file from command line or use default
SOURCE_FILE=${1:-"example.cpp"}
OUTPUT_FILE=${SOURCE_FILE%.*}.so

# Get or initialize the iteration number
if [ -f "$ITERATION_FILE" ] && [[ $(cat "$ITERATION_FILE") =~ ^[0-9]+$ ]]; then
    ITERATION=$(cat "$ITERATION_FILE")
    ITERATION=$((ITERATION + 1))
else
    ITERATION=1
fi

# Save the new iteration number
echo $ITERATION > "$ITERATION_FILE"
echo "Using iteration number: $ITERATION"

OUTPUT_LINK=${SOURCE_FILE%.*}.ln-$ITERATION.so

# Compile the C++ file into a shared library
echo "Compiling $SOURCE_FILE to lib/$OUTPUT_FILE"
g++ -std=c++17 -Wall -I"$EMACS_INCLUDE_DIR" -shared -fpic "$SOURCE_FILE" -o "lib/$OUTPUT_FILE"

if [ $? -eq 0 ]; then
    # Create symlink with iteration number
    rm -f lib/${SOURCE_FILE%.*}.ln-*.so  # Remove any previous symlinks
    ln -s $OUTPUT_FILE lib/$OUTPUT_LINK
    echo "Compilation successful with link: lib/$OUTPUT_LINK"

    # Generate a new test.el file with updated link
    cat > volatile/load.el << EOF
(module-load "$(pwd)/lib/$OUTPUT_LINK")
EOF

    echo "Updated test.el with new module path"
else
    echo "Compilation failed."
    exit 1
fi
