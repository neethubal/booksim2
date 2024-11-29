#!/bin/bash
parent_path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )
file="$1"

$parent_path/src/booksim $file traffic=uniform
mv netdump.txt base_netdump.txt

$parent_path/src/booksim $file

pypy3 $parent_path/parse_channeldump.py
xdot netdump.dot