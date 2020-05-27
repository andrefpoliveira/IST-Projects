#!/bin/bash

if [ "$#" -ne 4 ]; then
    echo O numero de argumentos nao e valido
    exit 1

elif [ ! -d "$1" ]; then
    echo A diretoria dos inputs nao existe
    exit 1

elif [ ! -d "$2" ]; then
    echo A diretoria dos outputs nao existe
    exit 1

elif [ $3 -lt 1 ]; then
    echo Numero de threads invalido
    exit 1

elif [ $4 -lt 1 ]; then
    echo Numero de buckets invalido
    exit 1
fi

for f in $1/*
do
    file=${f##*/}

    echo InputFile=$file NumThreads=1
    fileToCreate=${file%.*}-1.txt
    touch $2/$fileToCreate

    ./tecnicofs-nosync $f $2/$fileToCreate 1 1 | tail -1

    for i in $(seq 2 $3)
    do
        echo InputFile=$file NumThreads=$i
        fileToCreate=${file%.txt}-$i.txt
        touch $2/$fileToCreate
        
        ./tecnicofs-mutex $f $2/$fileToCreate $i $4 | tail -1


    done
done