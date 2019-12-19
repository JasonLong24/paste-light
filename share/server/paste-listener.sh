#!/bin/bash
# Listen for paste pushes.

SEND_OUTPUT_PORT="5555";

while true; do
  paste_input=$(nc -l -q 0 -p $SEND_OUTPUT_PORT)
  if ! echo $paste_input | sed 1q | grep '//\**' > /dev/null; then
    echo "File found but not a valid paste... Skipping"
    continue;
  fi
  filename=$(cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w 8 | head -n 1)
  echo "$paste_input" > posts/"$filename".txt
  paste-light --compile
  echo -e "File found at $(date). Assigned $filename."
done
