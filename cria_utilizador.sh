#!/bin/bash
if [ ! -e "etc/passwd.txt" ]; then
echo "File etc/passwd.txt not found, it is not possible to run the script without this file."
exit
fi
echo ""
echo "### Create User ###"
echo ""
echo "Insert your student number:"
read studentNo
while [[ ! $studentNo =~ ^[-+]?[0-9]+$ || $(echo $studentNo | grep '\;' | wc -w) -gt 0 || -z "$studentNo" ]]
    do
        echo "Your input can't be empty or have a ';'."
        read studentNo
done

x=$(cat etc/passwd.txt | grep [a]$studentNo[:] | wc -l)

if [ $x -gt 0 ]; then
	id=$(($studentNo + 1000000))
    if [ ! -e "utilizadores.txt" ]; then
        echo "File utilizadores.txt not found. Creating new one..."
        touch utilizadores.txt
    fi
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
	echo 'Invalid student number.'
fi
echo ""
