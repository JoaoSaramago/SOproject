#!/bin/bash
crontab /home/a77981/SOproject/cron.txt #Sempre que alguém abre o menu, o ficheiro cron.txt é adicionado ao crontab

echo ""
echo "### iCoolIUL Menu ###"
echo ""
echo "1. Create user"
echo "2. Change balance"
echo "3. Create vehicle"
echo "4. Show users"
echo "5. Show stats"
echo "6. Backup"
echo "0. Exit"
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
