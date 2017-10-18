#!/bin/bash
PATH="
clear
echo "### MENU ###"
echo "1. Create user"
echo "2. Change balance"
echo "3. Create vehicle"
echo "4. Show users"
echo "5. Show stats"
echo "6. Backup"
echo "0. Exit"
read input
while [[ ! $input =~ [0-6] ]]; do
    echo "Invalid option"
    read input
done
case $input in
1)
    ./cria_utilizador.sh
    ;;
2)
    ./adiciona_saldo.sh
    ;;
3)
    ./cria_viatura.sh
    ;;
4)
    echo "Utilizadores:"
    cat utilizadores.txt | cut -d ';' -f4
    ;;
5)
    ./stats.sh
    ;;
6)
    ./backup.sh
    ;;
0)
    ;;
esac