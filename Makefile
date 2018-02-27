
CFLAGS = -mcpu=arm926ej-s -g3 -Wall -O0 -nostdlib

CROSS_COMPILE ?= arm-none-eabi-

AS      = $(CROSS_COMPILE)as
CC      = $(CROSS_COMPILE)gcc
LD      = $(CROSS_COMPILE)ld
CPP     = $(CROSS_COMPILE)cpp
STRIP   = $(CROSS_COMPILE)strip
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
SIZE    = $(CROSS_COMPILE)size
GDB     = $(CROSS_COMPILE)gdb

study.elf : start.o main.o driver/source/led.o driver/source/delay.o driver/source/duart.o
	$(LD) -T link.lds $^ -o $@
	$(OBJCOPY) -O binary -S $@ study.bin
	$(OBJDUMP) -D $@ > study.dis
	$(SIZE) --format=berkeley "$@"

%.o : %.S
	$(CC) $(CFLAGS) -I"driver/include" -I"driver/source" -c -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -I"driver/include" -I"driver/source" -c -o $@ $^

gdb :
	$(GDB)

clean :
	@rm -rfv *.o *.elf *.bin *.dis driver/source/*.o

.PHONY : clean gdb