#!/bin/bash

urls=(
    "https://google.com"
    "https://github.com"
    "https://example.com"
    "https://example.com/not-found"
)

for url in "${urls[@]}"; do

    status=$(curl -s -o /dev/null -w "%{http_code}" "$url")

    case "$status" in
        200)
            echo "$url - $status OK"
            ;;
        301)
            echo "$url - $status Redirect"
            ;;
        403)
            echo "$url - $status Forbidden"
            ;;
        404)
            echo "$url - $status Not Found"
            ;;
        000)
            echo "$url - $status Connection refused"
            ;;
        *)
            echo "$url - $status Unknown status"
            ;;
    esac

done