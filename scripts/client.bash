#!/bin/bash
if [[ -z $1 ]]
then
    echo "Zadajte port"
    exit
fi
if [[ -z $2 ]]
then
    echo "Zadajte root prehladavania"
    exit
fi
if [[ $3 = "-s" ]]
then
    sim=1 
fi
exec 3<>/dev/tcp/127.0.0.1/$1

find $2 -type f | 
    while read filename; do
        s=$(md5sum $filename | cut -d ' ' -f 1)
        echo -e "DEL $s" >&3
        read -u 3 DB2
    done

find $2 -type f  | 
    while read filename; do
        s=$(md5sum $filename | cut -d ' ' -f 1)
        echo -e "GET $s" >&3
        read -u 3 DB
        if [[ -z $DB ]]
        then
            echo -e "SET $s $filename" >&3
            read -u 3 DB2
        else
            echo "Duplicita $filename $DB"
            if [[ -z $sim ]]
            then
                rm $filename
                echo "Odstranujem $filename"
            fi
        fi
    done

exec 3<&-
exec 3>&-