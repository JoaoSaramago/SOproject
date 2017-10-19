#!/bin/bash
PATH_HERE=$(pwd)
echo ""
echo "### Create backup ###"
echo ""
echo "Backing up the following files:"
DATE=$(date '+%Y-%m-%d-%H-%M')
zip $PATH_HERE/.ecooliul-$DATE.zip $PATH_HERE/utilizadores.txt $PATH_HERE/viaturas.txt
echo ""