#!/bin/bash
crontab /home/a77981/SOproject/cron.txt #Sempre que alguém abre o menu, o ficheiro cron.txt é adicionado ao crontab
BOLD='tput bold'
RED='tput setaf 1'
RESET='tput sgr0'
echo ""
echo "$($BOLD)$($RED)### iCoolIUL Menu ###$($RESET)"
echo ""
echo "$($BOLD)1.$($RESET) Create user"
echo "$($BOLD)2.$($RESET) Change balance"
echo "$($BOLD)3.$($RESET) Create vehicle"
echo "$($BOLD)4.$($RESET) Show users"
echo "$($BOLD)5.$($RESET) Show stats"
echo "$($BOLD)6.$($RESET) Backup"
echo "$($BOLD)0.$($RESET) Exit"
echo ""
echo "Choose one option:"
echo ""
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
    echo "Nickname:"
    read name
    echo "Value to add:"
    read value
    ./adiciona_saldo.sh $name $value
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
echo ""
