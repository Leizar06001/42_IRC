#!/bin/bash
counter=0
while true; do
	valgrind -s --leak-check=full --log-file="valgrind_&{counter}.log" ./irc_server 6667 
irc42
	sleep 1
	let counter++
done
