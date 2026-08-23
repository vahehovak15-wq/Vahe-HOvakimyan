#!/bin/bash

stop_process() {
    pid=$(pgrep "$1")

    if [ -n "$pid" ]; then
        printf "Stopping process: %s\n" "$pid"
    else
        printf "Process not found\n"
    fi
}

stop_process "zsh"