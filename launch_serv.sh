#!/bin/bash
counter=0
mkdir -p ./log
while true; do
	valgrind -s --leak-check=full --log-file="./log/valgrind_${counter}.log" ./irc_server 6667 irc42
	cp ./log/connections.log ./log/connections_${counter}.log
	cp ./log/last_log.txt ./log/last_log_${counter}.txt
	echo "Launching new server in 5 seconds..."
	sleep 5
	let counter++
done
