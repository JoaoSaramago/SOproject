#!/bin/bash
clear
echo ""
echo "### Statistics eCoolIUL ###"
echo ""
echo "Number of users: " $(cat utilizadores.txt | wc -l)
echo ""
echo "Number of cars with each color:"
cat viaturas.txt | cut -d ';' -f2 | sort | uniq -c
echo ""
cat utilizadores.txt | awk -F ';' 'BEGIN{sum=0} {sum += $7} END {print "Accumulated users balance: ", sum}'
echo ""
echo "Top 5 users with more balance:"
cat utilizadores.txt | awk -F ';' '{print "  ", $7, "- balance from user", $4}' | sort -r
echo ""
echo "Number of drivers per degree course:"
IGE=0
ETI=0
LEI=0
while read p;
    do
        if [[ $p = *"EI"* ]];
        then
            LEI=$(( LEI+=1 ))
        else if [[ $p = *"ET"* ]];
        then
            ETI=$(( ETI+=1 ))
        else if [[ $p = *"I"* ]];
        then
            IGE=$(( IGE+=1 ))
        fi fi fi
done <utilizadores.txt
echo "   LEI: "$LEI
echo "   ETI: "$ETI
echo "   IGE: "$IGE
echo ""
