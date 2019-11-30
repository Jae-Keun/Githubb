#ifndef _BUZZER_H_
#define _BUZZER_H_

int buzzerLibInit(void);
int buzzerLibExit(void);

int buzzerLibOnBuz(int freq);
int buzzerLibOffBuz(void);

#endif
