echo "Insert the vehicle name:"
read name
while [ $(cat viaturas.txt | grep $name[\;] | wc -l) -gt 0 ]
    do
    echo "Name already added to the file."
    echo "Insert a new vehicle name:"
    read name
done
echo "Insert the colour of the vehicle:"
read colour
echo "Insert the vehicle brand:"
read brand
echo "Insert the vehicle model:"
read model
echo "Insert the vehicle type:"
read type
echo "Insert the number of shifts:"
read shifts
echo "Insert the license plate number:"
read license
echo $name';'$colour';'$brand';'$model';'$type';'$shifts';'$license >> viaturas.txt