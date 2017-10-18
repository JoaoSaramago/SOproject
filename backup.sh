#!/bin/bash
echo ""
echo "### Create backup ###"
echo ""
echo "Backing up the following files:"
DATE=`date '+%Y-%m-%d-%H-%M'`
tar -czvf .ecooliul"-"$DATE.tar.gz utilizadores.txt viaturas.txt
echo ""
