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
#include "button.h"
#include "buzzer.h"
#include "fnd.h"
#include "lcdtext.h"

int main()
{
	int stat = 0;
	int cnt = 0;
	
	char line1[16] = "text lcd test";
	char line2[16] = "hello world!";
	
	ledLibInit();
	buttonLibInit();
	buzzerLibInit();
	fndLibInit();
	lcdtextLibInit();
	
	ledAllOn();
	sleep(1);
	ledAllOff();
	sleep(1);
	
	lcdtextwrite(line1, line2, 0);
	
	while(1)
	{
		if(stat==0)
		{
			ledOnOff(cnt,1);
			buzzerLibOnBuz(440);
			cnt += 1;
			printf("%d\n", ledStatus());
			fndDisp(ledStatus(), 0);
			sleep(1);
			if(cnt == 8)
			{
				stat = 1;
				cnt -= 1;
			}
		}
		else if(stat ==1)
		{
			ledOnOff(cnt,0);
			buzzerLibOffBuz();
			cnt -= 1;
			printf("%d\n", ledStatus());
			fndLibOff();
			sleep(1);
			if(cnt == -1)
			{
				stat = 0;
				cnt += 1;
			}			
		}
	}

	ledLibExit();
	buttonLibExit();
	buzzerLibExit();
	fndLibExit();
	lcdtextLibInit();
	
	return 0;
}
