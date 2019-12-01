CC = arm-linux-gnueabi-gcc
AR = arm-linux-gnueabi-ar

all :
	$(CC) -c fnd.c -o fnd.o
	$(CC) -c button.c -o button.o -lpthread
	$(AR) rc libMyPeri.a fnd.o button.o

	$(CC) 2048.c -o 2048 libMyPeri.a -lpthread
	
clean :
	rm 2048 fnd.o button.o libMyPeri.a
