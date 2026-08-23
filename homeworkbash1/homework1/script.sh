#!/bin/bash

LOG_FILE="system.log"
OUTPUT_FILE="issues.txt"

if [[ ! -f "$LOG_FILE" ]]; then
    echo "$LOG_FILE goyutyun chuni" 
    exit 1
fi


while IFS= read -r line || [[ -n "$line" ]]; do
    if [[ "$line" == *"error"* ]] || [[ "$line" == *"critical"* ]]; then
        echo "$line" >> "$OUTPUT_FILE"
    fi
done < "$LOG_FILE"

echo "$OUTPUT_FILE"