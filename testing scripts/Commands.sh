#!/bin/bash

CONNECTIONS=2

# Loop to start each connection script in a separate terminal
for ((i=1; i<=$CONNECTIONS; i++))
do
    gnome-terminal -- bash -c "./testUser${i}.sh $i; exec bash" &
done

echo "Join the channel #channel , set your nick to admin "