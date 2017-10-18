#!/bin/bash

nickname=$1
addtobalance=$2

if [ $( cat utilizadores.txt | grep ^$nickname[\;] | wc -l ) -gt 0 ]; then
if [[ $addtobalance =~ ^[-+]?[0-9]+$ ]]; then
        i=$(cat utilizadores.txt | grep ^$nickname[\;] | awk -F ";" '{print $7}' )
        cat utilizadores.txt | grep ^$nickname[\;] | sed -i 's/;'$i'$/;'$(( $i + $addtobalance ))'/g' utilizadores.txt
        echo "Value added to user "$nickname" account."
    else
        echo "The balance to add isn't a integer"
    fi
else
    echo "No user with this nickname."
fi
