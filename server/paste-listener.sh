#!/bin/bash
# Listen for paste pushes.

SEND_OUTPUT_PORT="5555";

while true ; do
  x=$(nc -l -q 0 -p $SEND_OUTPUT_PORT)
  local filename=$(cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w 8 | head -n 1)
  echo "$x" > posts/"$filename".txt
  paste --compile
  echo -e "File found at $(date). Assigned $filename."
done
