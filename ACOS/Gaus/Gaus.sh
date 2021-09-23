#!/usr/bin/env bash

columns=0
rows=0
index=0

array=()

while read string
do
	IFS=,

	for elem in $string
	do
		array[index]=$elem

		index=$(( $index+1 ))
	done

	rows=$(( $rows+1 ))

done < "$1"

columns=$(( $rows+1 ))

x=()

for (( i=0; i<$rows; i++ ))
do
	index=$(( $i*$columns+$i ))
	delim=${array[$index]}

	for((j=0; j<$columns; j++))
	do
		index=$(( $i*$columns+$j ))
		array[index]=$(echo "(${array[$index]})/($delim)" | bc -l)
	done

	for (( k=i+1; k<$rows; k++ ))
	do
		index=$(( $k*$columns+$i ))
		term=${array[$index]}

		for (( b=0; b<$columns; b++ ))
		do
			array[$(( $k*$columns+$b ))]=$(echo "(${array[$(( $k*$columns+$b ))]})-((${array[$(( $i*$columns+$b ))]})*($term))" | bc -l)
		done
	done
done

for(( i=$(( $rows-1 )); i>=0; i-- ))
do
	for(( j=$(( $i-1 )); j>=0; j-- ))
	do
		index=$(( $j*$columns+$i ))
		term=${array[$index]}

		for(( k=$(( $columns-1 )); k>=0; k-- ))
		do
			array[$(( $j*$columns+$k ))]=$(echo "(${array[$(( $j*$columns+$k ))]})-((${array[$(( $i*$columns+$k ))]})*($term))" | bc -l)
		done
	done
done

for(( i=0; i<$rows; i++ ))
do
	index=$(( $columns-1+$i*columns))
	echo ${array[index]}
done