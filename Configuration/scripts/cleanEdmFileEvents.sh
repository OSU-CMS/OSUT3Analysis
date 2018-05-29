#!/usr/bin/env bash

# one-liner for formatting the output from "edmFileUtil -e ..." as a list of
# events that edmPickEvents.py can understand

cat /dev/stdin | grep "^  " | grep -v "(\(Run\|Lumi\))\|TTree" | awk '{print $1,$2,$3}' | sed "s/ /:/g" | sort -h
