#!/bin/bash

tmux kill-session -t 0
tmux new-session -d 'picocom -b57600 /dev/ttyACM0 --echo --omap lfcrlf --imap lfcrlf'
tmux split-window -h 'picocom /dev/ttyUSB0 -b 115200'
tmux split-window -v 'cd server && python main.py'
tmux attach-session -d 
