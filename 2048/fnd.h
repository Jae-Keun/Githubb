#ifndef _FND_H_
#define _FND_H_

#define MAX_FND_NUM 6
#define FND_DATA_BUFF_LEN 8
#define FND_DRIVER_NAME "/dev/perifnd"

int fndLibInit(void);
int fndLibExit(void);

int fndDisp(int num, int dotflag);
int fndLibOff(void);

typedef struct FNDWriteDataForm_tag
{
	char DataNumeric[FND_DATA_BUFF_LEN];		//숫자0-9
	char DataDot[FND_DATA_BUFF_LEN];				//숫자0or1
	char DataValid[FND_DATA_BUFF_LEN];			//숫자0or1
}stFndWriteForm, *pStFndWriteForm;

#endif
