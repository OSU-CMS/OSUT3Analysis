#!/usr/bin/env python3
import sys, os, re
from subprocess import check_output

# Collect the parameters
previous_head = sys.argv[1]
new_head = sys.argv[2]
is_branch_checkout = sys.argv[3]

#A common problem with Python developers occurs when generated .pyc files
#stick around after switching branches. The interpreter sometimes uses these
#.pyc instead of the .py source file. To avoid any confusion, you can delete
#all .pyc files every time you check out a new branch using this post-checkout script:
if is_branch_checkout == "0":
    print("post-checkout: This is a file checkout. Nothing to do.")
    sys.exit(0)

print("post-checkout: Deleting all '.pyc' files in working directory")
for root, dirs, files in os.walk('.'):
    for filename in files:
        ext = os.path.splitext(filename)[1]
        if ext == '.pyc':
            os.unlink(os.path.join(root, filename))
