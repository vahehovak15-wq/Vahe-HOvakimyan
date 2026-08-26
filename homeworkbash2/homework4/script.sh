#!/bin/bash

host="$1"

if [ -z "$host" ]; then
    echo "Usage: $0 <host>"
    exit 1
fi

ports=(22 80 443)

echo "Scanning host: $host"

for port in "${ports[@]}"; do
    if (echo > /dev/tcp/"$host"/"$port") 2>/dev/null; then
        echo "Port $port: OPEN"
    else
        echo "Port $port: CLOSED"
    fi
done