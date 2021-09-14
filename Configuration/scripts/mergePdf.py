#!/usr/bin/python3

# Merge several pdf's into one
# Usage:
# mergePdf.py merged.pdf source1.pdf source2.pdf source3.pdf

import os
import sys

if len(sys.argv) < 3:
    print("Error:  Must specify as arguments the output merged file and source files.")
    exit(0)

command = "gs -q -dNOPAUSE -dBATCH -sDEVICE=pdfwrite -sOutputFile="
merged = sys.argv[1]
command += merged

#sources = []
for i in range(2,len(sys.argv)):
#    sources.append(sys.argv[i])
    command += " " + sys.argv[i]

print("command = ", command)

os.system(command)
print("Finished merging files to ", merged)
