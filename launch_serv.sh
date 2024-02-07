#!/bin/bash
counter=0
while true; do
	valgrind -s --leak-check=full --log-file="valgrind_${counter}.log" ./irc_server 6667 irc42
	echo "Launching new server in 5 seconds..."
	sleep 5
	let counter++
done
