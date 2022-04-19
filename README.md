# Nanopi-Neo3.GPIO
使用sysfs设置rk3288的GPIO，系统为Dietpi

## 简介
Nanopi-Neo3设有散热风扇接口，散热扇运行噪声较大，故想设置CPU达到温度后自动开启风扇，降温后再关闭。  
查阅网上案例，树莓派等设备较多采用RPI.GPIO库等实现，但其未适配Nanopi-Neo3，故采取直接设置sysfs方式实现。  

## 参考资料
* https://bbs.elecfans.com/jishu_527063_1_1.html&emsp; 思路来源  
* https://bbs.huaweicloud.com/blogs/detail/297252&emsp; 详细例子  
* https://blog.csdn.net/qq_30898641/article/details/89515799&emsp; 引脚计算  
* http://wiki.friendlyelec.com/wiki/images/a/ae/NanoPi-NEO3-2005-Schematic.pdf&emsp; 官方原理图  
* https://blog.csdn.net/qq_20553613/article/details/107703442&emsp; 读取CPU温度 
* https://blog.csdn.net/lc013/article/details/103775702?msclkid=2cd81662bfeb11ec8c391204449af968 &emsp; crontab定时执行命令
* https://www.cnblogs.com/wqbin/p/11824942.html?msclkid=7dae8caebff111ec8481a4aff7793022 &emsp; crontab status查看的坑
* https://askubuntu.com/questions/222512/cron-info-no-mta-installed-discarding-output-error-in-the-syslog?msclkid=a9a7e43bbff111ec8139bdf524c2e90e &emsp; crontab执行的坑1
* https://blog.csdn.net/FuJinlong94/article/details/120428955?msclkid=e7a99656bff211ecbe77b7ca9fdbda90 &emsp; crontab执行的坑2

## 实现步骤
### 1.GPIO sysfs interface 基本原理  
* 通过操作/sys/class/gpio目录下的export、unexport、gpio{N}/direction, gpio{N}/value（用实际引脚号替代{N}）等文件实现。  

### 2.声明GPIO口  
* 计算引脚号。Nanopi-Neo3散热扇接口为GPIO2A6（设置1为打开，0为关闭。通过原理图获得），通过计算可得引脚号为70（计算公式见资料3）。  
* 定义GPIO70指令：（这个命令需要root权限）  
`echo 70 > /sys/class/gpio/export`   
此时在/sys/class/gpio，会生成一个新目录，其名字为gpio70。  
进入其目录，依次能看到：active_low、device、direction、edge、power、subsystem、uevent、value文件。  
**direction**文件定义端口为输入或输出，参数范围是：**in或out**;  
**value**文件定义GPIO的值，参数范围是：**0或1**。  
直接对其读写，达到读写GPIO的效果。  
### 3.设置GPIO口为输出
* 设置指令：（这个命令需要root权限）  
`echo out > /sys/class/gpio/gpio70/direction`  
### 4.设置GPIO输出值
* 设置指令：  
`echo 1 > /sys/class/gpio/gpio70/value`&emsp;输出1  
`echo 0 > /sys/class/gpio/gpio70/value`&emsp;输出0  
### 5.读取CPU温度指令
* cpu原厂提供的linux内核通常已经带有cpu温度检测驱动，并将温度信息映射到用户文件系统下，用户只需从该虚拟文件读取温度信息即可。  
* 经查找，cpu温度存储于 **/sys/devices/virtual/thermal/thermal_zone0/temp** 文件中，数值除1000即为当前实际温度。  
* 查看指令：  
`cat /sys/devices/virtual/thermal/thermal_zone0/temp`
### 6.编写简单的shell控制  
* 设置指令：见fan_ctrl.sh
### 7.使用crontab定时执行
* 设置指令：sudo crontab -e
添加`*/1 * * * *  sh /root/fan_ctrl.sh >/dev/null 2>&1`至文件，设置每分钟执行一次。   
* service cron status &emsp; 查看运行状态  
* service cron restart &emsp; 重启服务  
