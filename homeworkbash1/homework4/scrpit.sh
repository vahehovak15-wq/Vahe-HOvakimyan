#!/usr/bin/env zsh

ips=("1.1.1.1" "8.8.8.8" "1.1.1.1" "10.0.0.1" "8.8.8.8")

typeset -A counts

for ip in "${ips[@]}"; do
    counts[$ip]=$(( ${counts[$ip]:-0} + 1 ))
done

for ip in "${(@k)counts}"; do
    echo "IP: $ip -> ${counts[$ip]} անգամ"
done