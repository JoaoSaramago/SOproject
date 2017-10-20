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
while [[ $(cat viaturas.txt | grep ^$name[\;] | wc -l) -gt 0 || $(echo $name | grep '\;' | wc -w) -gt 0 || -z "$name" ]]
    do
    echo "Insert a valid vehicle name:"
    echo "Tip: You can't use ';' and the name can't be empty!"
    read name
done

echo "Insert the colour of the vehicle:"
read colour
while [[ $(echo $colour | grep '\;' | wc -w) -gt 0 || -z "$colour" ]]
    do
    echo "Insert a valid colour:"
    echo "Tip: You can't use ';' and the colour can't be empty!"
    read colour
done

echo "Insert the vehicle brand:"
read brand
while [[ $(echo $brand | grep '\;' | wc -w) -gt 0 || -z "$brand" ]]
    do
    echo "Insert a valid brand:"
    echo "Tip: You can't use ';' and the brand can't be empty!"
    read brand
done

echo "Insert the vehicle model:"
read model
while [[ $(echo $model | grep '\;' | wc -w) -gt 0 || -z "$model" ]]
    do
    echo "Insert a valid colour:"
    echo "Tip: You can't use ';' and the model can't be empty!"
    read model
done

echo "Insert the vehicle gearbox type:"
read type
while [[ $(echo $type | grep '\;' | wc -w) -gt 0 || -z "$type" ]]
    do
    echo "Insert a valid type:"
    echo "Tip: You can't use ';' and the type can't be empty!"
    read type
done

echo "Insert the number of shifts:"
read shifts
while [[ ! $shifts =~ ^[-+]?[0-9]+$ || $(echo $shifts | grep '\;' | wc -w) -gt 0 || -z "$shifts" ]]; do
    echo "You must input a number"
read shifts
done

echo "Insert the license plate number:"
read license
while [[ $(echo $license | grep '\;' | wc -w) -gt 0 || -z "$license" ]]
    do
    echo "Insert a valid license:"
    echo "Tip: You can't use ';' and the license can't be empty!"
    read license
done

echo $name';'$colour';'$brand';'$model';'$type';'$shifts';'$license >> viaturas.txt
echo "Vehicle created!"
