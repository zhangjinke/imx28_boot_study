
CFLAGS = -mtune=arm926ej-s -nostdlib -nostartfiles -ffreestanding -static -g3 -Wall -O0 -I"driver/include" -I"driver/source"
LDLAGS = -T link.lds -nostdlib -nostartfiles -static -Map "study.map"
#LIBPATH = -lgcc -L"C:\Program Files (x86)\GNU Tools ARM Embedded\5.4 2016q2\lib\gcc\arm-none-eabi\5.4.1"
#LIBPATH = -lgcc -L"/home/peace/workspaces/tools/gcc-linaro-7.1.1-2017.05-x86_64_arm-linux-gnueabi/lib/gcc/arm-linux-gnueabi/7.1.1"
#LIBPATH = -lgcc -L"/home/peace/workspaces/tools/gcc-arm-none-eabi-5_4-2016q3/lib/gcc/arm-none-eabi/5.4.1/"
#LIBPATH = -lgcc -L"/c/Program Files (x86)/GNU Tools ARM Embedded/5.4 2016q3/lib/gcc/arm-none-eabi/5.4.1"
#LIBPATH = -lgcc -L"/d/Program Files/gcc-linaro-7.2.1-2017.11-i686-mingw32_arm-linux-gnueabi/lib/gcc/arm-linux-gnueabi/7.2.1"
LIBPATH = -lgcc -L"/c/Program Files (x86)/GNU Tools Arm Embedded/7 2018-q2-update/lib/gcc/arm-none-eabi/7.3.1"

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

OBJS = start.o main.o
OBJS += driver/source/led.o
OBJS += driver/source/delay.o
OBJS += driver/source/duart.o
#OBJS += driver/source/ddr2.o
OBJS += driver/source/clkctrl.o
#OBJS += driver/source/relocate.o
OBJS += driver/source/power.o
#OBJS += driver/source/lcdif.o
OBJS += driver/source/pinctrl.o

all: study.elf 

study.elf : $(OBJS)
	$(LD) $(LDLAGS) $^ -o $@ $(LIBPATH)
	$(OBJCOPY) -O binary -S $@ study.bin
	$(OBJDUMP) -D $@ > study.dis
	$(SIZE) --format=berkeley "$@"

%.o : %.S
	$(CC) $(CFLAGS) -c -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $^

gdb : study.elf
	$(GDB)

sb: study.elf
	./elftosb.exe -f imx28 -c ./boot_ivt.bd -o study.sb
	./sb_loader.bat

bin: study.elf
	JLink.exe -autoconnect 1 -Device ARM9 -If JTAG -JTAGConf -1,-1 -Speed 15000 -CommanderScript CommandFile.jlink

clean :
	@rm -rfv *.o *.map *.elf *.bin *.dis *.sb driver/source/*.o

.PHONY : all clean gdb bin
