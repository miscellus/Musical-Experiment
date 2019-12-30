#!/usr/bin/env bash

gcc -g -std=c99 -Wall -Wextra -pedantic -o music music.c -lm
./music tone.csv > tone.wav
vlc --quiet --play-and-exit tone.wav

# case $1 in
# 	build)
# 		bash -c "read line"
# 		;;
# 	run)
# 		./music tone.csv
# 		cvlc --quiet --play-and-exit Lullaby.wav
# 		;;
# 	debug)
# 		nemiver music tone.csv ;;
# esac
 