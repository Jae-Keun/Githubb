// button.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/input.h>
#include <unistd.h> // for open/close
#include <fcntl.h> // for O_RDWR
#include <sys/ioctl.h> // for ioctl
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <pthread.h>

#include "button.h"

static pthread_t buttonTh_id;
static int fd = 0;
static int msgID = 0;

static void* buttonThFunc(void *arg);

int buttonLibInit(void)
{
	fd=open (BUTTON_DRIVER_NAME, O_RDONLY);
	msgID = msgget (MESSAGE_ID, IPC_CREAT|0666);
	pthread_create(&buttonTh_id, NULL, &buttonThFunc, NULL);
}

int buttonLibExit(void)
{
	close(fd);
	pthread_cancel(buttonTh_id);
}

static void* buttonThFunc(void *arg)
{
	BUTTON_MSG_T msgTx;
	msgTx.messageNum = 1;
	struct input_event stEvent;
	
	while (1)
	{
		read(fd, &stEvent, sizeof (stEvent));
		
		if ( stEvent.type == EV_KEY) {
			printf("EV_KEY(");
			
			switch(stEvent.code) {
				case KEY_VOLUMEUP: printf("Volume up key):"); break;
				case KEY_HOME: printf("Home key):"); break;
				case KEY_SEARCH: printf("Search key):"); break;
				case KEY_BACK: printf("Back key):"); break;
				case KEY_MENU: printf("Menu key):"); break;
				case KEY_VOLUMEDOWN:printf("Volume down key):");break;
			}
			
			if ( stEvent.value ) printf("pressed\n");
			else printf("released\n");
		}
		
		if ( ( stEvent.type == EV_KEY) && ( stEvent.value == 0) ) {
		msgTx.keyInput = stEvent.code;
		msgsnd(MESSAGE_ID, &msgTx, sizeof(int), 0);
		}
	}
}
