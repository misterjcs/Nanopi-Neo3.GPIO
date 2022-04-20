#!/bin/bash

#设置GPIO
echo 70 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio70/direction

#读取当前cpu温度到temp
temp=$(cat /sys/devices/virtual/thermal/thermal_zone0/temp)
echo $temp
#判断温度(高于54度开启风扇，降到42度后关闭，避免停停转转)
if [ $temp -gt 54000 ];then
     echo 1 > /sys/class/gpio/gpio70/value
elif [ $temp -lt 42000 ];then
     echo 0 > /sys/class/gpio/gpio70/value
else
	:
fi
