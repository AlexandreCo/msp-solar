msp-solar
=========

msp430 solar powered test

compilation : 
sudo apt-get install msp430-libc mspdebug msp430mcu binutils-msp430 gcc-msp430 gdb-msp430 srecord dos2unix
make

Programation : 
mspdebug rf2500 "prog solar.elf"
