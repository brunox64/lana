#!/bin/zsh

file=$1

clang++ -std=c++17 -stdlib=libc++ -g "$file.cpp" -o "$file" 
