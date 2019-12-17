#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

#include "buzzer.h"

#define MAX_SCALE_STEP 8
#define BUZZER_BASE_SYS_PATH "/sys/bus/platform/devices/"
#define BUZZER_FILENAME "peribuzzer"
#define BUZZER_ENABLE_NAME "enable"
#define BUZZER_FREQUENCY_NAME "frequency"

char gBuzzerBaseSysDir[128];		// /sys/bus/platform/devices/peribuzzer.XX 가 결정됨

static int fd_en = 0;
static int fd_freq = 0;

const int musicSclae[MAX_SCALE_STEP] = {
		262, /* do */ 294, 330, 349, 392, 440, 494, /* si */ 523 
	};

//버저 경로 찾기: /sys/bus/platform/devices/peribuzzer.XX 의 XX를 결정하는 것
int fineBuzzerSysPath()
{
	DIR * dir_info = opendir(BUZZER_BASE_SYS_PATH);
	
	int ifNotFound = 1;
	
	if (dir_info != NULL){
		while (1){
			struct dirent *dir_entry;
			dir_entry = readdir (dir_info);
			
			if (dir_entry == NULL) break;
			if (strncasecmp(BUZZER_FILENAME, dir_entry->d_name, strlen(BUZZER_FILENAME)) == 0){
				ifNotFound = 0;
				sprintf(gBuzzerBaseSysDir,"%s%s/",BUZZER_BASE_SYS_PATH,dir_entry->d_name);
			}
		}
	}
	printf("find %s\n",gBuzzerBaseSysDir);
	return ifNotFound;
}

int buzzerLibInit(void)
{
	char path[200];
	
	fineBuzzerSysPath();
	
	sprintf(path,"%s%s",gBuzzerBaseSysDir,BUZZER_ENABLE_NAME);
	fd_en = open(path,O_WRONLY);	
	
	sprintf(path,"%s%s",gBuzzerBaseSysDir,BUZZER_FREQUENCY_NAME);
	fd_freq = open(path,O_WRONLY);
}

int buzzerLibExit(void)
{
	write(fd_en, &"0", 1);
	
	close(fd_en);
	close(fd_freq);
}

int buzzerLibOnBuz(int freq)
{
	write(fd_en, &"1", 1);
	dprintf(fd_freq, "%d", freq);
}
int buzzerLibOffBuz(void)
{
	write(fd_en, &"0", 1);
}
