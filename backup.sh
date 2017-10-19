#!/bin/bash
<<<<<<< HEAD
PATH_HERE=$(pwd)
=======
crontab cron.txt #Quando correr o backup.sh este script é automaticamente adicionado ao crontab em qualquer computador
>>>>>>> 61ef7763266b3c21ce6ca4e7e629a2d58fabf54c
echo ""
echo "### Create backup ###"
echo ""
echo "Backing up the following files:"
DATE=$(date '+%Y-%m-%d-%H-%M')
zip $PATH_HERE/.ecooliul-$DATE.zip $PATH_HERE/utilizadores.txt $PATH_HERE/viaturas.txt
echo ""
