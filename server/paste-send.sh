#!/bin/bash
# Push files to listening server.

SEND_OUTPUT_HOST=""
SEND_OUTPUT_PORT="5555"

if [ ! -f $1 ]; then echo "$1 is not a valid file."; return 1; fi
read -p "Please enter a title: " entryTitle && read -p "Please enter the date: " entryDate && read -p "Please enter a filetype: " entryFT
echo -e "{{*title: $entryTitle\n{{*date: $entryDate\n{{*filetype: $entryFT" | cat - $1 | nc -v -q 0 $SEND_OUTPUT_HOST $SEND_OUTPUT_PORT
echo "File Sent!"

