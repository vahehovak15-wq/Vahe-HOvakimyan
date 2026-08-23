#!/bin/bash

shopt -s nullglob

files=(*.bak)

if [ ${#files[@]} -ne 0 ]; then
    echo "Kan .bak "
else
    echo "CKan .bak "
fi

for file in *.bak; do
    mv "$file" "${file%.bak}.old"
    echo "Poxvel e: $file -> ${file%.bak}.old"
done