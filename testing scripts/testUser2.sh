#!/bin/bash

USERNUM=2
SERVER="localhost"
PORT="1111"

# Create a unique named pipe for this connection
mkfifo "my_pipe_$USERNUM"

# Start netcat in the background, with stdin connected to the pipe
nc $SERVER $PORT < "my_pipe_$USERNUM" &

# Store the PID of this job, so we can refer to it later
echo $! > "my_pipe_${USERNUM}_pid"

# Connect to the server using netcat in the background
# Replace 'username' and 'password' with your desired username and password
{(echo "USER testUser2" 
echo "NICK testUser2" 
echo "PASS 1234" 
sleep 11
echo "JOIN #channel abced" 
echo "JOIN #channel 1234" 
echo "PRIVMSG #channel : hello from testUser2"
echo "PART #channel"
sleep 1;
echo "JOIN #channel"
sleep 1;
echo "JOIN #channel" 
sleep 1.5;
echo "TOPIC #channel :This is the new topic"
sleep 1;
echo "TOPIC #channel :This is the new topic"
echo "PART #channel" 
sleep 1;
echo "JOIN #channel"
sleep 1;
echo "JOIN #channel"
echo "KICK #channel testUser1" 
sleep 1;
echo "QUIT" )}> my_pipe_2


rm -f "my_pipe_$USERNUM"
rm -f "my_pipe_${USERNUM}_pid"