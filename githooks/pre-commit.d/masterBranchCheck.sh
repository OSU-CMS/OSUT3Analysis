#!/bin/sh
echo "pre-commit master branch check started"

branch=$(git rev-parse --abbrev-ref HEAD | grep -e 'master')
if [ $? -eq 0 ]; then
  echo "!!!WARNING!! You should not be working on the master branch."
  echo "Are you sure you want to continue? [y/N]"
  read confirmation < /dev/tty
  if [ "$confirmation" == "Y" ] || [ "$confirmation" == "y" ]; then
    exit 0
  else
    exit 1
  fi
fi
echo "pre-commit master branch check done"
