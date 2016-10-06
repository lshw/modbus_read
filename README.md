# modbus_read
read or write modbus  regs

先 git clone  https://github.com/lshw/modbus_read

然后进入目录编译

cd modbus_read

#编译安装
编译需要 libmodbus-dev 包， 运行需要 libmodbus5包 ，在debian系下， apt-get install 即可 
make install 会安装到/usr/bin/modbu_read , 并建立一个符号链接: /usr/bin/modbus_write 

#使用方法:

读取在串口/dev/ttyS3上的 1号设备的0号寄存器，开始的9个寄存器值， 并且以%d格式输出,速率9600" 
modbus_read /dev/ttyS3 9600 1 0 9  "%d " 


读取在串口/dev/ttyS3上的 1号设备的9号寄存器，开始的1个寄存器值， 并且以4位16进制格式输出,速率9600: 
modbus_read /dev/ttyS3 9600 1 9 1 %04x 


向串口/dev/ttyS3上的 1号设备的9号寄存器，写一个整数102,速率9600: 
modbus_write /dev/ttyS3 9600 1 9 102 %d 
