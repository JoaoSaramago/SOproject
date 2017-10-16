#!/bin/bash

clear
echo "Insert your student number:"
read studentNo

while [ -z $studentNo ]
    do
        echo "Your input can't be empty."
        read studentNo
done

x=$(cat etc/passwd.txt | grep [a]$studentNo[:] | wc -l)

if [ $x -gt 0 ]; then
	id=$(($studentNo + 1000000))
	x=$(cat utilizadores.txt | grep [\;]$id[\;] | wc -l)
	if [ $x -gt 0 ]; then
		echo “User already exists.”
	else
		name=$(cat etc/passwd.txt | grep [a]$studentNo[:] | cut -d ':' -f5 | cut -d ',' -f1 )
        clear
        echo ''
        echo '### Hello '$name' ###'
        echo ''
		echo "Insert your nickname:"
        read nickname
        while [ $( cat utilizadores.txt | grep ^$nickname[\;] | wc -l ) -gt 0 ]
            do
            echo "This nickname is already in use, please use other: "
            read nickname
        done
        echo "Insert your password:"
        read -s password
        echo '- Password registered'
        echo "Insert your class:"
        read class
        echo "Insert your balance:"
        read balance
        email='a'$studentNo'@iscte-iul.pt'
        echo $nickname';'$password';'$id';'$name';'$email';'$class';'$balance >> utilizadores.txt
	fi
else
	echo “Invalid student number.”
fi
