#!/bin/sh
if [ -n "$1" ] ; then
    if [ -b /dev/$1 ]; then
        if [ ! -d /media ]; then
            mkdir -p /media
        fi  
        if [ ! -d /media/$1 ]; then
            mkdir -p /media/$1
        fi  
   
        mount /dev/$1 /media/$1
    
        if [ $? -ne 0 ]; then
            rm -rf /media/$1
        fi  
    fi  
fi

