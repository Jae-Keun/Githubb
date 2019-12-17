#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>

#include "fnd.h"

static int fd = 0;

int fndLibInit(void)
{
	fd = open(FND_DRIVER_NAME, O_WRONLY);
	
	if(fd ==0) {
			printf("ERROR!");
			return 1;
	}
}
int fndLibExit(void)
{
	fndLibOff();
	close(fd);
}

#define ONE_SEG_DISPLAY_TIME_USEC	1000
// return 1 => success  , 0 => error
int fndDisp(int num, int dotflag)
{
	int temp, i;
	stFndWriteForm stWriteData;
	
	for(i=0;i<MAX_FND_NUM;i++)
	{
		stWriteData.DataDot[i] = (dotflag&(0x1<<i))?1:0;
		stWriteData.DataValid[i] = 1;
	}
	
	// if 6 fnd
	temp = num % 1000000;
	stWriteData.DataNumeric[0]= temp /100000;

	temp = num % 100000;
	stWriteData.DataNumeric[1]= temp /10000;

	temp = num % 10000;
	stWriteData.DataNumeric[2] = temp /1000;

	temp = num %1000;
	stWriteData.DataNumeric[3] = temp /100;

	temp = num %100;
	stWriteData.DataNumeric[4] = temp /10;

	stWriteData.DataNumeric[5] = num %10;
	
	write(fd, &stWriteData, sizeof(stFndWriteForm));
}

int fndLibOff(void)
{
	int i;
	stFndWriteForm stWriteData;
	
	for (i = 0; i < MAX_FND_NUM ; i++ )
	{
		stWriteData.DataDot[i] =  0;  
		stWriteData.DataNumeric[i] = 0;
		stWriteData.DataValid[i] = 0;
	}
	
	write(fd,&stWriteData,sizeof(stFndWriteForm));	
	return 1;
}
