#!/bin/bash

LOG_FILE="auth.log"

echo "=== Security Log Report ==="
echo

echo "Total failed SSH attempts:"
grep -c "Failed password" "$LOG_FILE"
echo

echo "Failed attempts by IP:"
grep "Failed password" "$LOG_FILE" |
awk '{print $(NF-3)}' |
sort |
uniq -c |
sort -nr
echo

echo "Successful root logins:"
grep "Accepted password for root" "$LOG_FILE"
echo

echo "Access to /etc/shadow:"
grep "/etc/shadow" "$LOG_FILE"

echo
echo "Suspicious IPs (3 or more failed attempts):"

grep "Failed password" "$LOG_FILE" |
awk '{print $(NF-3)}' |
sort |
uniq -c |
awk '$1 >= 3 {print "ALERT:", $2, "-", $1, "failed attempts"}'
