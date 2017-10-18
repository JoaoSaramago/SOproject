#!/bin/bash
echo ""
echo "### Create backup ###"
echo ""
echo "Backing up the following files:"
DATE=`date '+%Y-%m-%d-%H-%M'`
zip /Users/JGVSaramago/Desktop/SOproject/.ecooliul-$DATE.zip /Users/JGVSaramago/Desktop/SOproject/utilizadores.txt /Users/JGVSaramago/Desktop/SOproject/viaturas.txt
echo ""
