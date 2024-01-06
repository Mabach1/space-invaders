#!/bin/bash

set -xe

clear

if gcc -g -o3 -Wall -Wextra -Wpedantic -o main -fsanitize=address  src/*.c -lSDL2 -lSDL2_image -lSDL2_ttf -lm; then
	./main
else
	echo "Couldn not compile, will not execute."
fi