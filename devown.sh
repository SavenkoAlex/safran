#!/bin/bash

name='Morpho'
search=`lsusb | grep $name`
if [[ $(/usr/bin/id -u) -ne 0 ]]; then
    echo "Ты не РУТ"
    exit
else
    if 
        [ -z "$search" ]
    then
        echo "lsusb не нашло устройство с именем $name"
        exit
    else
        uname=`cat /etc/passwd | grep 1000 | awk -F":" '{print $1}'`
        echo "устройство $name обнаруженно"
        bus=`lsusb | grep $name | awk '{print $2}'`
        device=`lsusb | grep $name | awk '{print $4}'| cut -c 1-3`
        echo "chown: $uname  /dev/bus/usb/$bus/$device"
        chown $uname: /dev/bus/usb/$bus/$device
        ls -l /dev/bus/usb/$bus/$device
    fi
fi