#!/bin/bash

#读取当前cpu温度到temp
temp=$(cat /sys/devices/virtual/thermal/thermal_zone0/temp)
echo $temp
#判断温度(高于50度开启风扇，降到45度后关闭，避免停停转转)
if [ $temp -gt 50000 ];then
     echo 1 > /sys/class/gpio/gpio70/value
elif [ $temp -lt 45000 ];then
     echo 0 > /sys/class/gpio/gpio70/value
else
	:
fi
