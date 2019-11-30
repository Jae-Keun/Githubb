CC = arm-linux-gnueabi-gcc
AR = arm-linux-gnueabi-ar

all :
	$(CC) -c led.c -o led.o
	$(CC) -c button.c -o button.o -lpthread
	$(CC) -c buzzer.c -o buzzer.o
	$(AR) rc libMyPeri.a led.o button.o buzzer.o
	
	$(CC) --static -o peritest ledtest.c libMyPeri.a -lpthread

clean:
	rm led.o button.o buzzer.o libMyPeri.a peritest
