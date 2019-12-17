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

#include "lcdtext.h"

static int fd = 0;

int lcdtextLibInit(void)
{
	fd = open(TEXTLCD_DRIVER_NAME, O_WRONLY);
	
	if(fd ==0) {
		printf("ERROR!");
		return 1;
	}
}
int lcdtextLibExit(void)
{
	close(fd);
}

int lcdtextwrite(const char *str1, const char *str2, int lineFlag)
{	
	stTextLCD  stlcd; 
	int len1, len2;

	memset(&stlcd,0,sizeof(stTextLCD));

	len1 = strlen(str1);
	len2 = strlen(str2);

	if ( lineFlag == 1) // firsst line
	{
		//stlcd.cmdData = CMD_DATA_WRITE_LINE_1;
		if ( len1 > COLUMN_NUM) memcpy(stlcd.TextData[0],str1,COLUMN_NUM);
		else memcpy(stlcd.TextData[0],str1,len1);
		
	}
	else if ( lineFlag == 2) // second line
	{
		//stlcd.cmdData = CMD_DATA_WRITE_LINE_2;
		if ( len2 > COLUMN_NUM) memcpy(stlcd.TextData[1],str2,COLUMN_NUM);
		else memcpy(stlcd.TextData[1],str2,len2);
	}
	else if (lineFlag == 0) // both line
	{
		//stlcd.cmdData = CMD_DATA_WRITE_BOTH_LINE;
		if ( len1 > COLUMN_NUM) memcpy(stlcd.TextData[0],str1,COLUMN_NUM);
		else memcpy(stlcd.TextData[0],str1,len1);
		
		if ( len2 > COLUMN_NUM) memcpy(stlcd.TextData[1],str2,COLUMN_NUM);
		else memcpy(stlcd.TextData[1],str2,len2);
	}
	
	stlcd.cmd = CMD_WRITE_STRING;
	
	write(fd,&stlcd,sizeof(stTextLCD));
}
/*
int lcdtextoff(void)
{
	stTextLCD  stlcd; 
	memcpy(stlcd.TextData[stlcd.cmdData - 1], ,COLUMN_NUM);
	stlcd.cmd = CMD_WRITE_STRING;
	write(fd,&stlcd,sizeof(stTextLCD));
}
*/
