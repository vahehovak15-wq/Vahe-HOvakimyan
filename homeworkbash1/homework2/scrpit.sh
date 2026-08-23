#!/bin/bash

important_files=(
    "/etc/passwd"
    "/etc/hosts"
    "/etc/resolv.conf"
    "/etc/shadow"
)

for file in "${important_files[@]}"; do
    if [ ! -e "$file" ] || [ ! -r "$file" ]; then
        echo "TAGNAP $file"
    else 
        echo "OK $file"
    fi
done