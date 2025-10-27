#!/usr/bin/env python
#!/usr/bin/env python3
import sys
import os

# Collect the parameters
previous_head = sys.argv[1]
new_head = sys.argv[2]
is_branch_checkout = sys.argv[3]

# A common problem with Python developers occurs when generated .pyc files
# stick around after switching branches. The interpreter sometimes uses these
# .pyc instead of the .py source file. To avoid any confusion, delete
# all .pyc files every time you check out a new branch using this post-checkout script.
if is_branch_checkout == "0":
    print("post-checkout: This is a file checkout. Nothing to do.")
    sys.exit(0)

print("post-checkout: Deleting all '.pyc' files in working directory")
for root, dirs, files in os.walk('.'):
    for filename in files:
        if filename.endswith('.pyc'):
            os.unlink(os.path.join(root, filename))
