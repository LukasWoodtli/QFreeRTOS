#!/bin/bash

# http://stackoverflow.com/a/3355423/1272072
cd "$(dirname "$0")"

echo "Runnning doxygen in dir: "
echo pwd

doxygen Doxyfile  > /dev/null 2>&1

echo "Doxygen finished!"

