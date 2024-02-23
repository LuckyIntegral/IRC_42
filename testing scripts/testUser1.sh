#!/bin/bash

USERNUM=1
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
{(echo "USER testUser1" 
echo "NICK testUser1" 
echo "PASS 1234" 
echo "JOIN #channel" 
sleep 10;
echo "PRIVMSG #channel : hello from testUser1"
echo "MODE #channel +k 1234"
sleep 1.5;
echo "MODE #channel -k"
echo "MODE #channel +i"
sleep 1;
echo "INVITE testUser2 #channel" 
sleep 1.5;
echo "MODE #channel -i" 
echo "MODE #channel +o testUser2" 
echo "MODE #channel -o testUser2" 
sleep 1;
echo "MODE #channel +t" 
sleep 1;
echo "MODE #channel -t" 
sleep 1;
echo "MODE #channel +l 1" 
sleep 1;
echo "MODE #channel -l" 
sleep 1;
echo "KICK #channel testUser2 :You are not welcome here" 
sleep 1;
echo "PRIVMSG admin :Hello admin" 
sleep 1;
echo "PRIVMSG admin :Do you like my script ? " 
sleep 1;
echo "NICK BYE"
echo "QUIT" )}> my_pipe_1

rm -f "my_pipe_$USERNUM"
rm -f "my_pipe_${USERNUM}_pid"