#!/bin/bash

clang -g helloworld.c -o helloworld.bin || exit 1

./helloworld.bin || exit 1
