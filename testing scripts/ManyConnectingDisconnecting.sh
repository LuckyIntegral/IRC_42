#!/bin/bash

# Define the IRC server details
SERVER="localhost"
PORT="1111"
PASSWORD="1234"

# Define the number of connections
CONNECTIONS=1000

# Loop to create connections
for ((i=1; i<=$CONNECTIONS; i++)); do
    # Connect to the server using netcat in the background
    # Replace 'username' and 'password' with your desired username and password
    (echo "USER testUser$i"; echo "NICK testUser$i"; echo "PASS 1234"; echo "JOIN #channel"; echo "PRIVMSG #channel : hello"; echo "QUIT") | nc $SERVER $PORT $PASSWORD &
	#nc $SERVER $PORT $PASSWORD &
    # Add a delay between connections to avoid flooding the server
done
wait
# Wait for all connections to finish
