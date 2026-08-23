#!/bin/bash

encoded="THiS_iS_A_TExT"

spaced="${encoded//_/ }"

decoded=$(echo "$spaced" | tr '[:upper:]' '[:lower:]')

printf "%s\n" "$decoded"