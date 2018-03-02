file study.elf

target remote localhost:2331 

monitor sleep 500
monitor reset
monitor sleep 300

# Set JTAG speed to auto
monitor speed 15000

#monitor reg pc = 1024

load

#b main

#c