set architecture armv6-m
target remote localhost:3333
monitor reset halt
load
break main.c:main
fs next