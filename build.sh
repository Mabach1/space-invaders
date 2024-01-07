#!/bin/bash

set -xe

clear

if clang -g -o3 -Wall -Wextra -Wpedantic -Werror -o main -fsanitize=address  src/*.c -lSDL2 -lSDL2_image -lSDL2_ttf; then
	./main
else
	echo "Couldn not compile, will not execute."
fi