ALL SCRIPTS ARE MEANT TO BE TESTED BY STARTING THE SERVER WITH MAKE TEST

Script BigMessages:
##########################################

connects to the server with the number of <CONNECTIONS> 
each connection then joins a channel #channel and sends a message of 10,000 characters and quits


Script ManyConnectingDisconnecting : 
##########################################

creates 1000 connections one after another with each :
1. joins the server
2. writes a message inside of #channel
3. quits the servers

Script MultipleConnections :
##########################################
- this script is meant to verify server clients limit and 
	possible server freeze by using all fds
creates 1200 connections one after another with each :
1. joins the server
2. never leaves

terminal has to be closed to stop the connections


Script Commands :
##########################################
- is meant to use every command as operator and regular user
- testUser1.sh and testUser2.sh are used by this script

usage :
1. start server with make test
2. start irssi
3. /connect localhost 1111 1234 admin
4. start the script : bash IRC_Test.sh
6. /join channel
----------------See the magic happen----------------------------
