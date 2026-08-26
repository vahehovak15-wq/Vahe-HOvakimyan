#!/bin/bash

read -p "Enter file name: " filename

if [ ! -f "$filename" ]; then
    echo "Error: '$filename' ."
    exit 1
fi

read -p "Enter word to search: " word

until grep -q "$word" "$filename"; do
    echo "Word '$word' not found."
    read -p "Try another word: " word
done

echo -e "\nWord found! Line details:"
grep -n "$word" "$filename"