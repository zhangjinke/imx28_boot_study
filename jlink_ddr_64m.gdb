#connect to jlink jdbserver
target remote localhost:2331

#monitor long 0x800401e0 = 0x00000002
monitor sleep 500

#reset target
monitor reset
monitor sleep 300

#select endian
monitor endian little

# Set JTAG speed to auto
monitor speed 15000

#initilized memery contror
#monitor long 0x53000000 = 0x00000000

#wait for moment��
monitor sleep 20


#set cpu to svc mode(on cpu reset)
monitor reg cpsr = 0xd3

#debug in ram
monitor reg pc   = 0x40000000

monitor long 0x800400F0 = 0x80000002
monitor long 0x800401B0 = 0x92921613
monitor long 0x800401D8 = 0x00000080
monitor long 0x80044050 = 0x0000270C
monitor long 0x80018008 = 0xC0000000
monitor long 0x80019B80 = 0x00032AAA
monitor long 0x800181A8 = 0xFFFFFFFF
monitor long 0x800181B8 = 0xFFFFFFFF
monitor long 0x800181C8 = 0xFFFFFFFF
monitor long 0x800181D8 = 0xFFFFFFFF
monitor long 0x800400F0 = 0x00000002

#****************************
# EMI DDR2 Initialization
#****************************
monitor long 0x800e0000 = 0x00000000
monitor long 0x800e0004 = 0x00000000
monitor long 0x800e0008 = 0x00000000
monitor long 0x800e000c = 0x00000000
monitor long 0x800e0010 = 0x00000000
monitor long 0x800e0014 = 0x00000000
monitor long 0x800e0018 = 0x00000000
monitor long 0x800e001c = 0x00000000
monitor long 0x800e0020 = 0x00000000
monitor long 0x800e0024 = 0x00000000
monitor long 0x800e0028 = 0x00000000
monitor long 0x800e002c = 0x00000000
monitor long 0x800e0030 = 0x00000000
monitor long 0x800e0034 = 0x00000000
monitor long 0x800e0038 = 0x00000000
monitor long 0x800e003c = 0x00000000
monitor long 0x800e0040 = 0x00000000
monitor long 0x800e0044 = 0x00000100
monitor long 0x800e0048 = 0x00000000
monitor long 0x800e004c = 0x00000000
monitor long 0x800e0050 = 0x00000000
monitor long 0x800e0054 = 0x00000000
monitor long 0x800e0058 = 0x00000000
monitor long 0x800e005c = 0x00000000
monitor long 0x800e0060 = 0x00000000
monitor long 0x800e0064 = 0x00000000
monitor long 0x800e0068 = 0x00010101
monitor long 0x800e006c = 0x01010101
monitor long 0x800e0070 = 0x000f0f01
monitor long 0x800e0074 = 0x0102010a
monitor long 0x800e0078 = 0x00000000
monitor long 0x800e007c = 0x00000101
monitor long 0x800e0080 = 0x00000100
monitor long 0x800e0084 = 0x00000100
monitor long 0x800e0088 = 0x00000000
monitor long 0x800e008c = 0x00000002
monitor long 0x800e0090 = 0x01010000
monitor long 0x800e0094 = 0x07080403
monitor long 0x800e0098 = 0x0900a003
monitor long 0x800e009c = 0x140000c8
monitor long 0x800e00a0 = 0x02013880
monitor long 0x800e00a4 = 0x00020303
monitor long 0x800e00a8 = 0x006d6012
monitor long 0x800e00ac = 0x03330c2a
monitor long 0x800e00b0 = 0x03060304
monitor long 0x800e00b4 = 0x00c80037
monitor long 0x800e00b8 = 0x00000000
monitor long 0x800e00bc = 0x00000000
monitor long 0x800e00c0 = 0x00012100
monitor long 0x800e00c4 = 0xffff0303
monitor long 0x800e00c8 = 0x00012100
monitor long 0x800e00cc = 0xffff0303
monitor long 0x800e00d0 = 0x00012100
monitor long 0x800e00d4 = 0xffff0303
monitor long 0x800e00d8 = 0x00012100
monitor long 0x800e00dc = 0xffff0303
monitor long 0x800e00e0 = 0x00000003
monitor long 0x800e00e4 = 0x00000000
monitor long 0x800e00e8 = 0x00000000
monitor long 0x800e00ec = 0x00000000
monitor long 0x800e00f0 = 0x00000000
monitor long 0x800e00f4 = 0x00000000
monitor long 0x800e00f8 = 0x00000000
monitor long 0x800e00fc = 0x00000000
monitor long 0x800e0100 = 0x00000000
monitor long 0x800e0104 = 0x00000000
monitor long 0x800e0108 = 0x00000612
monitor long 0x800e010c = 0x01000f02
monitor long 0x800e0110 = 0x06120612
monitor long 0x800e0114 = 0x00000200
monitor long 0x800e0118 = 0x00020007
monitor long 0x800e011c = 0xf4004a27
monitor long 0x800e0120 = 0xf4004a27
monitor long 0x800e0124 = 0xf4004a27
monitor long 0x800e0128 = 0xf4004a27
monitor long 0x800e012c = 0x07000300
monitor long 0x800e0130 = 0x07000300
monitor long 0x800e0134 = 0x07400300
monitor long 0x800e0138 = 0x07400300
monitor long 0x800e013c = 0x00000005
monitor long 0x800e0140 = 0x00000000
monitor long 0x800e0144 = 0x00000000
monitor long 0x800e0148 = 0x01000000
monitor long 0x800e014c = 0x01020408
monitor long 0x800e0150 = 0x08040201
monitor long 0x800e0154 = 0x000f1133
monitor long 0x800e0158 = 0x00000000
monitor long 0x800e015c = 0x00001f04
monitor long 0x800e0160 = 0x00001f04
monitor long 0x800e0164 = 0x00001f04
monitor long 0x800e0168 = 0x00001f04
monitor long 0x800e016c = 0x00001f04
monitor long 0x800e0170 = 0x00001f04
monitor long 0x800e0174 = 0x00001f04
monitor long 0x800e0178 = 0x00001f04
monitor long 0x800e017c = 0x00000000
monitor long 0x800e0180 = 0x00000000
monitor long 0x800e0184 = 0x00000000
monitor long 0x800e0188 = 0x00000000
monitor long 0x800e018c = 0x00000000
monitor long 0x800e0190 = 0x00000000
monitor long 0x800e0194 = 0x00000000
monitor long 0x800e0198 = 0x00000000
monitor long 0x800e019c = 0x00000000
monitor long 0x800e01a0 = 0x00000000
monitor long 0x800e01a4 = 0x00000000
monitor long 0x800e01a8 = 0x00000000
monitor long 0x800e01ac = 0x00000000
monitor long 0x800e01b0 = 0x00000000
monitor long 0x800e01b4 = 0x00000000
monitor long 0x800e01b8 = 0x00000000
monitor long 0x800e01bc = 0x00000000
monitor long 0x800e01c0 = 0x00000000
monitor long 0x800e01c4 = 0x00000000
monitor long 0x800e01c8 = 0x00000000
monitor long 0x800e01cc = 0x00000000
monitor long 0x800e01d0 = 0x00000000
monitor long 0x800e01d4 = 0x00000000
monitor long 0x800e01d8 = 0x00000000
monitor long 0x800e01dc = 0x00000000
monitor long 0x800e01e0 = 0x00000000
monitor long 0x800e01e4 = 0x00000000
monitor long 0x800e01e8 = 0x00000000
monitor long 0x800e01ec = 0x00000000
monitor long 0x800e01f0 = 0x00000000
monitor long 0x800e01f4 = 0x00000000
monitor long 0x800e01f8 = 0x00000000
monitor long 0x800e01fc = 0x00000000
monitor long 0x800e0200 = 0x00000000
monitor long 0x800e0204 = 0x00000000
monitor long 0x800e0208 = 0x00000000
monitor long 0x800e020c = 0x00000000
monitor long 0x800e0210 = 0x00000000
monitor long 0x800e0214 = 0x00000000
monitor long 0x800e0218 = 0x00000000
monitor long 0x800e021c = 0x00000000
monitor long 0x800e0220 = 0x00000000
monitor long 0x800e0224 = 0x00000000
monitor long 0x800e0228 = 0x00000000
monitor long 0x800e022c = 0x00000000
monitor long 0x800e0230 = 0x00000000
monitor long 0x800e0234 = 0x00000000
monitor long 0x800e0238 = 0x00000000
monitor long 0x800e023c = 0x00000000
monitor long 0x800e0240 = 0x00000000
monitor long 0x800e0244 = 0x00000000
monitor long 0x800e0248 = 0x00000000
monitor long 0x800e024c = 0x00000000
monitor long 0x800e0250 = 0x00000000
monitor long 0x800e0254 = 0x00000000
monitor long 0x800e0258 = 0x00000000
monitor long 0x800e025c = 0x00000000
monitor long 0x800e0260 = 0x00000000
monitor long 0x800e0264 = 0x00000000
monitor long 0x800e0268 = 0x00000000
monitor long 0x800e026c = 0x00000000
monitor long 0x800e0270 = 0x00000000
monitor long 0x800e0274 = 0x00000000
monitor long 0x800e0278 = 0x00000000
monitor long 0x800e027c = 0x00000000
monitor long 0x800e0280 = 0x00000000
monitor long 0x800e0284 = 0x00000000
monitor long 0x800e0288 = 0x00010000
monitor long 0x800e028c = 0x00030404
monitor long 0x800e0290 = 0x00000005
monitor long 0x800e0294 = 0x00000000
monitor long 0x800e0298 = 0x00000000
monitor long 0x800e029c = 0x00000000
monitor long 0x800e02a0 = 0x00000000
monitor long 0x800e02a4 = 0x00000000
monitor long 0x800e02a8 = 0x00000000
monitor long 0x800e02ac = 0x01010000
monitor long 0x800e02b0 = 0x01000000
monitor long 0x800e02b4 = 0x03030000
monitor long 0x800e02b8 = 0x00010303
monitor long 0x800e02bc = 0x01020202
monitor long 0x800e02c0 = 0x00000000
monitor long 0x800e02c4 = 0x02060303
monitor long 0x800e02c8 = 0x21002103
monitor long 0x800e02cc = 0x00061200
monitor long 0x800e02d0 = 0x06120612
monitor long 0x800e02d4 = 0x04420442
monitor long 0x800e02d8 = 0x04420442
monitor long 0x800e02dc = 0x00040004
monitor long 0x800e02e0 = 0x00040004
monitor long 0x800e02e4 = 0x00000000
monitor long 0x800e02e8 = 0x00000000
monitor long 0x800e02ec = 0x00000000
monitor long 0x800e02f0 = 0x00000000
monitor long 0x800e02f4 = 0xffffffff

#**  start controller **#
monitor long 0x800e0040 = 0x00000001


#load the debug image
load

# Set break points
break aw_main
#break main

#debug begin
