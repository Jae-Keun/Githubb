CC = arm-linux-gnueabi-gcc
AR = arm-linux-gnueabi-ar

all :
	$(CC) -c fnd.c -o fnd.o
	$(CC) -c button.c -o button.o -lpthread
	$(AR) rc libMyPeri.a fnd.o button.o
	
	$(CC) -O3 -o libBitmap.o -c libBitmap.c
	$(AR) rc libMyLib.a libBitmap.o 	

	#$(CC) -O3 2048.c -o 2048 -L. -l MyPeri -l MyLib -lpthread --static
	$(CC) 2048.c -o 2048 libMyPeri.a libMyLib.a -lpthread
	#$(CC) 2048.c -o 2048 libMyPeri.a -lpthread
	
clean :
	rm 2048 fnd.o button.o libMyPeri.a libMyLib.a
