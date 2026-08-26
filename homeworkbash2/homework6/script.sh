#!/usr/bin/env bash
declare -A hits

while IFS= read -r ip || [[ -n "$ip" ]]; do
    ip=$(echo "$ip" | tr -d '\r\n ' )
    
    [[ -z "$ip" ]] && continue

    hits["$ip"]=$(( hits["$ip"] + 1 ))
done < access.log

echo "IP Request Counts:"
echo "------------------"

for ip in "${!hits[@]}"; do
    echo "$ip - ${hits[$ip]} requests"
done

echo
echo "IPs with more than 3 requests:"
echo "-------------------------------"

for ip in "${!hits[@]}"; do
    if [ "${hits[$ip]}" -gt 3 ]; then
        echo "$ip - ${hits[$ip]} requests"
    fi
done

