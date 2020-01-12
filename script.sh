#!/bin/bash

if [ "$#" -lt 1 ] ; then
    #to fiew arguments
    exit 1
fi

file=$1

#Check if file exists
if ! [ -f "$file" ] ; then
    #there is no such file
    exit 1
fi

#Number regex
num_re='^[0-9]+$'

for ((i=2; i<="$#"; i++))
    do

    if ! [[ "${!i}" =~ $num_re ]] ; then
        #the argument is not a number(or is negative)
        exit 1
    fi

    if (( "${!i}" == 0 || "${!i}" > 999)) ; then
        #the argument is outside acceptable range
        exit 1
    fi

    #finding the line
    line=$( grep -E '^'"${!i}"'[;]{1}' "$file")

    #spliting the line
    IFS=';'
    read -ra ADDR <<< "$line"

    len=${#ADDR[@]}
    out=0
    any=0
    
    #calculating length of a route
    for((j=2; j < len; j+=3)) do
        ((out+="${ADDR[j]}"))
        any=1
    done

    if [[ any -eq 1 ]] ; then
        #route was found
        echo "${!i};$out"
    fi
done

exit 0
