#!/bin/sh

valgrind -v --leak-check=full --show-leak-kinds=all --read-var-info=yes --track-origins=yes "$@" 2>&1 | tee valgrind.log
