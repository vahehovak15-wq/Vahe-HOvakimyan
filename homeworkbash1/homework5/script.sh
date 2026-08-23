#!/bin/bash

regex="^[a-zA-Z0-9._]+@[a-zA-Z0-9.-]+\.[a-zA-Z]+$"

while true; do
    read -p "Enter your email address: " email

    if [[ "$email" =~ $regex ]]; then
        echo "Valid email adress $email"
        break
    else
        echo "Invalid email adsress:Please try again"
    fi
done