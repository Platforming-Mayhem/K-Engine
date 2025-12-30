#!/bin/bash
set -e

cmake -B bin -DCMAKE_WARN_DEPRECATED=OFF -DEDITOR=ON
read -p "Press any key to resume ..."
cmake --build bin --target Editor
read -p "Press any key to resume ..."
