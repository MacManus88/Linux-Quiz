all: zentriert.o
	gcc -std=c99 -I/usr/include/libusb-1.0/ -Iinclude -Llib quiz_hid.c hid-libusb.c -lusb-1.0 lib/zentriert.a -o quiz_hid

zentriert.o:
	gcc -c lib/zentriert.c -o lib/zentriert.o
	ar rs lib/zentriert.a lib/zentriert.o
