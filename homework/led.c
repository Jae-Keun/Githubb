#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "led.h"

static unsigned int ledValue = 0;
static int fd = 0;

int ledOnOff(int ledNum, int ledOnOff)
{
	int i = 1;
	i = i << ledNum;
	ledValue = ledValue & (~i);
	if(ledOnOff != 0)	ledValue |= i;
	write(fd, &ledValue, 4);
}

int ledLibInit(void)
{
	fd = open(LED_DRIVER_NAME, O_WRONLY);
	
	if(fd ==0) {
			printf("ERROR!");
			return 1;
	}
	
	ledValue = 0;
}

int ledStatus(void)
{
	return ledValue;
}

int ledLibExit(void)
{
	ledValue = 0;
	ledOnOff(0, 0);
	close(fd);
}

void ledAllOn(void)
{
	ledValue = 255;
	write(fd, &ledValue, 4);
}

void ledAllOff(void)
{
	ledValue = 0;
	write(fd, &ledValue, 4);
}
