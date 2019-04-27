#!/usr/bin/env bash

astyle --recursive 'src/*' \
    --style=allman \
    --indent=spaces=4 \
    --indent-switches \
    --indent-col1-comments \
    --break-blocks \
    --pad-oper \
    --pad-comma \

find . -name "*.orig" -delete
