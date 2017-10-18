#!/bin/bash

echo ""
echo "### Create vehicle script ###"
if [ ! -e "viaturas.txt" ]; then
    echo "File viaturas.txt not found."
    touch viaturas.txt
fi
echo "### Create new Vehicle script ###"
echo "Insert the vehicle name:"
read name
while [[ $(cat viaturas.txt | grep $name[\;] | wc -l) -gt 0 || $(echo $name | grep '\;' | wc -w) -gt 0 ]]
    do
    echo "Insert a valid vehicle name:"
    echo "Tip: You can't use ';' and the name can't be empty!"
    read name
done
echo "Insert the colour of the vehicle:"
read colour
echo "Insert the vehicle brand:"
read brand
echo "Insert the vehicle model:"
read model
echo "Insert the vehicle gearbox type:"
read type
echo "Insert the number of shifts:"
read shifts
while [[ ! $shifts =~ ^[-+]?[0-9]+$ ]]; do
    echo "You must input a number"
read shifts
done
echo "Insert the license plate number:"
read license
echo $name';'$colour';'$brand';'$model';'$type';'$shifts';'$license >> viaturas.txt
echo "Vehicle created!"