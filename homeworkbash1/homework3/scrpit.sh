#!/bin/bash

while true; do
    echo "--- MENU ---"
    echo "1. Check network"
    echo "2. View processes"
    echo "3. Exit"
    
    read -p "Enter a number (1-3): " choice

    case $choice in
        1)
            echo "Network is active"
            ;;
        2)
            echo "Processes are normal"
            ;;
        3)
            echo "Exiting loop..."
            break
            ;;
        *)
            echo "Invalid input: Please select 1, 2, or 3:"
            ;;
    esac
    echo ""
done