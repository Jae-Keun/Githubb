#ifndef _BUTTON_H_
#define _BUTTON_H_

#define BUTTON_DRIVER_NAME "/dev/input/event4"
#define MESSAGE_ID 1122

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

char button_input;

typedef struct{
	long int messageNum;
	int keyInput;
} BUTTON_MSG_T;

int buttonLibInit(void);
int buttonLibExit(void);
#endif //_BUTTON_H_
