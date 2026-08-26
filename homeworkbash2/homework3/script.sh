#!/bin/bash

file="data.txt"

lines=0
words=0
chars=0

while IFS= read -r line
do
    ((lines++))

    read -ra word_array <<< "$line"
    ((words += ${#word_array[@]}))

    ((chars += ${#line}))
done < "$file"

echo "Total Lines:      $lines"
echo "Total Words:      $words"
echo "Total Characters: $chars"