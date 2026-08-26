#!/bin/bash

path="$1"

if [ -z "$path" ]; then
    echo "Usage: ./script.sh <file_or_directory>"
    exit 1
fi

if [ ! -e "$path" ]; then
    echo "Error: '$path' does not exist."
    exit 1
fi

read -p "Are you sure? (yes/no): " confirm

if [ "$confirm" = "yes" ]; then
    rm -rf "$path"
    echo "'$path' was successfully deleted."
else
    echo "Deletion cancelled."
fi