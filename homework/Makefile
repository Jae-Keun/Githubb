CC = arm-linux-gnueabi-gcc
AR = arm-linux-gnueabi-ar

all :
	$(CC) -c led.c -o led.o
	$(CC) -c button.c -o button.o -lpthread
	$(CC) -c buzzer.c -o buzzer.o
	$(CC) -c fnd.c -o fnd.o
	$(CC) -c lcdtext.c -o lcdtext.o
	$(AR) rc libMyPeri.a led.o button.o buzzer.o fnd.o lcdtext.o
	
	$(CC) --static -o peritest mylib.c libMyPeri.a -lpthread

clean:
	rm led.o button.o buzzer.o fnd.o lcdtext.o libMyPeri.a peritest
