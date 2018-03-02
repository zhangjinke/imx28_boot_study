
CFLAGS = -mcpu=arm926ej-s -nostdlib -nostartfiles -ffreestanding -static -g3 -Wall -O0 -I"driver/include" -I"driver/source"
LDLAGS = -T link.lds -nostdlib -nostartfiles -static -Map "study.map"
LIBPATH = -lgcc -L"C:\Program Files (x86)\GNU Tools ARM Embedded\5.4 2016q2\lib\gcc\arm-none-eabi\5.4.1"

CROSS_COMPILE ?= arm-none-eabi-
#CROSS_COMPILE ?= arm-linux-gnueabi-

AS      = $(CROSS_COMPILE)as
CC      = $(CROSS_COMPILE)gcc
LD      = $(CROSS_COMPILE)ld
CPP     = $(CROSS_COMPILE)cpp
STRIP   = $(CROSS_COMPILE)strip
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
SIZE    = $(CROSS_COMPILE)size
GDB     = $(CROSS_COMPILE)gdb

study.elf : start.o main.o driver/source/led.o driver/source/delay.o driver/source/duart.o driver/source/ddr2.o driver/source/clkctrl.o
	$(LD) $(LDLAGS) $^ -o $@ $(LIBPATH)
	$(OBJCOPY) -O binary -S $@ study.bin
	$(OBJDUMP) -D $@ > study.dis
	elftosb -f imx28 -c ./boot_ivt.bd -o study.sb
	$(SIZE) --format=berkeley "$@"

%.o : %.S
	$(CC) $(CFLAGS) -c -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $^

gdb :
	$(GDB)

sb:
	sb_loader.bat

clean :
	@rm -rfv *.o *.map *.elf *.bin *.dis *.sb driver/source/*.o

.PHONY : clean gdb