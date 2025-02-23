#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

# Function to recursively find directories with a Makefile and run make clean and make
process_directories() {
  for dir in "$1"/*; do
    if [ -d "$dir" ]; then
      if [ -f "$dir/Makefile" ]; then
        echo "Processing $dir"
        (cd "$dir" && make)
      fi
      process_directories "$dir"
    fi
  done
}

# Start processing from the current directory
process_directories "$(pwd)"