# Security Log Analyzer

A Bash-based security lab project that analyzes Linux SSH authentication logs and identifies suspicious activity.

## Features

- Counts failed SSH login attempts
- Groups failed attempts by IP address
- Detects suspicious IPs with 3 or more failed attempts
- Displays successful root logins
- Detects access to `/etc/shadow`
- Generates a security report

## Tools

- Bash
- grep
- awk
- sort
- uniq

## Usage

```bash
chmod +x analyze_logs.sh
./analyze_logs.sh
