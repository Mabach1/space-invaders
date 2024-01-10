#!/bin/bash

set -xe

clear

if make; then
	./bin/main
else
	echo "Couldn not compile, will not execute."
fi