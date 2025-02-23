#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

# Check if an argument is provided
if [ -z "$1" ]; then
  echo "Usage: $0 <all|clean>"
  exit 1
fi

# Function to recursively find directories with a Makefile and run make or make clean
process_directories() {
  for dir in "$1"/*; do
    if [ -d "$dir" ]; then
      if [ -f "$dir/Makefile" ]; then
        echo "Processing $dir"
        (cd "$dir" && make $2)
      fi
      process_directories "$dir" "$2"
    fi
  done
}

# Start processing from the current directory
process_directories "$(pwd)" "$1"