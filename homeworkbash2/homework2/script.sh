#!/bin/bash

# Recursive function to calculate factorial
factorial() {
    local n=$1

    # Base case: if n <= 1, factorial is 1
    if [ "$n" -le 1 ]; then
        echo 1
    else
        # Recursive call: (n - 1)
        local prev=$(factorial $(( n - 1 )))
        # Return n * (n - 1)!
        echo $(( n * prev ))
    fi
}

number=$1

result=$(factorial "$number")
echo "Factorial of $number ($number!) is: $result"