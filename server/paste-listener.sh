#!/bin/bash
# Listen for paste pushes.

SEND_OUTPUT_PORT="5555";

while true ; do
  paste_input=$(nc -l -q 0 -p $SEND_OUTPUT_PORT)
  filename=$(cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w 8 | head -n 1)
  echo "$paste_input" > posts/"$filename".txt
  paste --compile
  echo -e "File found at $(date). Assigned $filename."
done
