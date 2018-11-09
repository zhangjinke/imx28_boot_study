
INCLUDE_PATH = -I"driver/include"
INCLUDE_PATH += -I"malloc"
INCLUDE_PATH += -I"3rdparty/lwip-2.1.0/adapter/include"
INCLUDE_PATH += -I"3rdparty/lwip-2.1.0/adapter/include/lwip"
INCLUDE_PATH += -I"3rdparty/lwip-2.1.0/src/include"

CFLAGS = -mtune=arm926ej-s -nostdlib -nostartfiles -ffreestanding -static -g3 -Wall -O0 $(INCLUDE_PATH)
LDLAGS = -T link.lds -nostdlib -nostartfiles -static -Map "study.map"
#LIBPATH = -lgcc -L"C:\Program Files (x86)\GNU Tools ARM Embedded\5.4 2016q2\lib\gcc\arm-none-eabi\5.4.1"
#LIBPATH = -lgcc -L"/home/peace/workspaces/tools/gcc-linaro-7.1.1-2017.05-x86_64_arm-linux-gnueabi/lib/gcc/arm-linux-gnueabi/7.1.1"
#LIBPATH = -lgcc -L"/home/peace/workspaces/tools/gcc-arm-none-eabi-5_4-2016q3/lib/gcc/arm-none-eabi/5.4.1/"
#LIBPATH = -lgcc -L"/c/Program Files (x86)/GNU Tools ARM Embedded/5.4 2016q3/lib/gcc/arm-none-eabi/5.4.1"
#LIBPATH = -lgcc -L"/d/Program Files/gcc-linaro-7.2.1-2017.11-i686-mingw32_arm-linux-gnueabi/lib/gcc/arm-linux-gnueabi/7.2.1"
LIBPATH = -lc -L"/c/Program Files (x86)/GNU Tools Arm Embedded/7 2018-q2-update/arm-none-eabi/lib"
LIBPATH += -lgcc -L"/c/Program Files (x86)/GNU Tools Arm Embedded/7 2018-q2-update/lib/gcc/arm-none-eabi/7.3.1"

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
OBJS += newlib_adapter.o
OBJS += driver/source/arm_int_gcc.o
OBJS += driver/source/led.o
OBJS += driver/source/delay.o
OBJS += driver/source/duart.o
#OBJS += driver/source/ddr2.o
OBJS += driver/source/clkctrl.o
OBJS += driver/source/relocate.o
OBJS += driver/source/power.o
#OBJS += driver/source/lcdif.o
OBJS += driver/source/pinctrl.o
OBJS += driver/source/icoll.o

OBJS += malloc/my_malloc.o

#lwip 相关文件
OBJS += 3rdparty/lwip-2.1.0/src/api/api_lib.o
OBJS += 3rdparty/lwip-2.1.0/src/api/api_msg.o
OBJS += 3rdparty/lwip-2.1.0/src/api/err.o
OBJS += 3rdparty/lwip-2.1.0/src/api/if_api.o
OBJS += 3rdparty/lwip-2.1.0/src/api/netbuf.o
OBJS += 3rdparty/lwip-2.1.0/src/api/netdb.o
OBJS += 3rdparty/lwip-2.1.0/src/api/netifapi.o
OBJS += 3rdparty/lwip-2.1.0/src/api/sockets.o
OBJS += 3rdparty/lwip-2.1.0/src/api/tcpip.o

OBJS += 3rdparty/lwip-2.1.0/src/core/ipv4/autoip.o
OBJS += 3rdparty/lwip-2.1.0/src/core/ipv4/dhcp.o
OBJS += 3rdparty/lwip-2.1.0/src/core/ipv4/etharp.o
OBJS += 3rdparty/lwip-2.1.0/src/core/ipv4/icmp.o
OBJS += 3rdparty/lwip-2.1.0/src/core/ipv4/igmp.o
OBJS += 3rdparty/lwip-2.1.0/src/core/ipv4/ip4.o
OBJS += 3rdparty/lwip-2.1.0/src/core/ipv4/ip4_addr.o
OBJS += 3rdparty/lwip-2.1.0/src/core/ipv4/ip4_frag.o

#OBJS += 3rdparty/lwip-2.1.0/src/core/ipv6/dhcp6.o
#OBJS += 3rdparty/lwip-2.1.0/src/core/ipv6/ethip6.o
#OBJS += 3rdparty/lwip-2.1.0/src/core/ipv6/icmp6.o
#OBJS += 3rdparty/lwip-2.1.0/src/core/ipv6/inet6.o
#OBJS += 3rdparty/lwip-2.1.0/src/core/ipv6/ip6.o
#OBJS += 3rdparty/lwip-2.1.0/src/core/ipv6/ip6_addr.o
#OBJS += 3rdparty/lwip-2.1.0/src/core/ipv6/ip6_frag.o
#OBJS += 3rdparty/lwip-2.1.0/src/core/ipv6/mld6.o
#OBJS += 3rdparty/lwip-2.1.0/src/core/ipv6/nd6.o

OBJS += 3rdparty/lwip-2.1.0/src/core/altcp.o
OBJS += 3rdparty/lwip-2.1.0/src/core/altcp_alloc.o
OBJS += 3rdparty/lwip-2.1.0/src/core/altcp_tcp.o
OBJS += 3rdparty/lwip-2.1.0/src/core/def.o
OBJS += 3rdparty/lwip-2.1.0/src/core/dns.o
OBJS += 3rdparty/lwip-2.1.0/src/core/inet_chksum.o
OBJS += 3rdparty/lwip-2.1.0/src/core/init.o
OBJS += 3rdparty/lwip-2.1.0/src/core/ip.o
OBJS += 3rdparty/lwip-2.1.0/src/core/mem.o
OBJS += 3rdparty/lwip-2.1.0/src/core/memp.o
OBJS += 3rdparty/lwip-2.1.0/src/core/netif.o
OBJS += 3rdparty/lwip-2.1.0/src/core/pbuf.o
OBJS += 3rdparty/lwip-2.1.0/src/core/raw.o
OBJS += 3rdparty/lwip-2.1.0/src/core/stats.o
OBJS += 3rdparty/lwip-2.1.0/src/core/sys.o
OBJS += 3rdparty/lwip-2.1.0/src/core/tcp.o
OBJS += 3rdparty/lwip-2.1.0/src/core/tcp_in.o
OBJS += 3rdparty/lwip-2.1.0/src/core/tcp_out.o
OBJS += 3rdparty/lwip-2.1.0/src/core/timeouts.o
OBJS += 3rdparty/lwip-2.1.0/src/core/udp.o

OBJS += 3rdparty/lwip-2.1.0/src/netif/ethernet.o

#lwip 适配文件
OBJS += 3rdparty/lwip-2.1.0/adapter/src/sys_arch.o

all: study.elf

study.elf : $(OBJS)
	$(LD) $(LDLAGS) $^ -o $@ $(LIBPATH)
#	$(OBJCOPY) -O binary -S $@ study.bin
	$(OBJDUMP) -D $@ > study.dis
	$(SIZE) --format=berkeley "$@"

%.o : %.s
	$(CC) -c -o $@ $(CFLAGS) $^

%.o : %.c
	$(CC) -c -o $@ $(CFLAGS) $^

gdb : study.elf
	$(GDB)

sb: study.elf
	./elftosb.exe -f imx28 -c ./boot_ivt.bd -o study.sb
	./sb_loader.bat

bin: study.elf
	JLink.exe -autoconnect 1 -Device ARM9 -If JTAG -JTAGConf -1,-1 -Speed 15000 -CommanderScript CommandFile.jlink

clean :
	rm -rf *.o *.map *.elf *.bin *.dis *.sb
	find . -name "*.o" | xargs rm -f

.PHONY : all clean gdb bin
