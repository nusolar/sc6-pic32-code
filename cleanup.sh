#!/bin/sh

# Run this to cleanup random files and make some room on Dropbox

rm -vR *~
rm -vR projects/*/nbproject/private/
rm -vR projects/*/build/
rm -vR projects/*/dist/
rm -vR docs/html/
rm -vR docs/latex/
rm -vR docs/rtf/
rm -viR "*(conflicted copy)*"
