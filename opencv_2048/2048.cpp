/*
 ============================================================================
 Name        : 2048.c
 Author      : Maurits van der Schee
 Description : Console version of the game "2048" for GNU/Linux
 ============================================================================
 */
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
using namespace cv;
using namespace std;

#include "project.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <stdint.h>
#include <time.h>
#include <signal.h>

#include "fnd.h"			// for use fnd
#include "button.h"
#include "lcdtext.h"
#include "led.h"
#include "buzzer.h"
#include "bitmapFileHeader.h"
#include "libBitmap.h"

#include "project.h"



#define SIZE 4

#define true 1
#define false 0

#define bool int

uint32_t score=0;
uint8_t scheme=0;

void getColor(uint8_t value, char *color, size_t length) {
	uint8_t original[] = {8,255,1,255,2,255,3,255,4,255,5,255,6,255,7,255,9,0,10,0,11,0,12,0,13,0,14,0,255,0,255,0};
	uint8_t blackwhite[] = {232,255,234,255,236,255,238,255,240,255,242,255,244,255,246,0,248,0,249,0,250,0,251,0,252,0,253,0,254,0,255,0};
	uint8_t bluered[] = {235,255,63,255,57,255,93,255,129,255,165,255,201,255,200,255,199,255,198,255,197,255,196,255,196,255,196,255,196,255,196,255};
	uint8_t *schemes[] = {original,blackwhite,bluered};
	uint8_t *background = schemes[scheme]+0;
	uint8_t *foreground = schemes[scheme]+1;
	if (value > 0) while (value--) {
		if (background+2<schemes[scheme]+sizeof(original)) {
			background+=2;
			foreground+=2;
		}
	}
	snprintf(color,length,"\033[38;5;%d;48;5;%dm",*foreground,*background);
}

void drawBoard(uint8_t board[SIZE][SIZE]) {
	uint8_t x,y;
	char color[40], reset[] = "\033[m";
	printf("\033[H");

	printf("2048.c %17d pts\n\n",score);

	for (y=0;y<SIZE;y++) {
		for (x=0;x<SIZE;x++) {
			getColor(board[x][y],color,40);
			printf("%s",color);
			printf("       ");
			printf("%s",reset);
		}
		printf("\n");
		for (x=0;x<SIZE;x++) {
			getColor(board[x][y],color,40);
			printf("%s",color);
			if (board[x][y]!=0) {
				char s[8];
				snprintf(s,8,"%u",(uint32_t)1<<board[x][y]);
				uint8_t t = 7-strlen(s);
				printf("%*s%s%*s",t-t/2,"",s,t/2,"");
			} else {
				printf("   ·   ");
			}
			printf("%s",reset);
		}
		printf("\n");
		for (x=0;x<SIZE;x++) {
			getColor(board[x][y],color,40);
			printf("%s",color);
			printf("       ");
			printf("%s",reset);
		}
		printf("\n");
	}
	printf("\n");
	printf("        ←,↑,→,↓ or q        \n");
	printf("\033[A"); // one line up
}

uint8_t findTarget(uint8_t array[SIZE],uint8_t x,uint8_t stop) {
	uint8_t t;
	// if the position is already on the first, don't evaluate
	if (x==0) {
		return x;
	}
	for(t=x-1;;t--) {
		if (array[t]!=0) {
			if (array[t]!=array[x]) {
				// merge is not possible, take next position
				return t+1;
			}
			return t;
		} else {
			// we should not slide further, return this one
			if (t==stop) {
				return t;
			}
		}
	}
	// we did not find a
	return x;
}

int slideArray(uint8_t array[SIZE]) {
	int success = false;
	uint8_t x,t,stop=0;

	for (x=0;x<SIZE;x++) {
		if (array[x]!=0) {
			t = findTarget(array,x,stop);
			// if target is not original position, then move or merge
			if (t!=x) {
				// if target is zero, this is a move
				if (array[t]==0) {
					array[t]=array[x];
				} else if (array[t]==array[x]) {
					// merge (increase power of two)
					array[t]++;
					// increase score
					score+=(uint32_t)1<<array[t];
					// set stop to avoid double merge
					stop = t+1;
				}
				array[x]=0;
				success = true;
			}
		}
	}
	return success;
}

void rotateBoard(uint8_t board[SIZE][SIZE]) {
	uint8_t i,j,n=SIZE;
	uint8_t tmp;
	for (i=0; i<n/2; i++) {
		for (j=i; j<n-i-1; j++) {
			tmp = board[i][j];
			board[i][j] = board[j][n-i-1];
			board[j][n-i-1] = board[n-i-1][n-j-1];
			board[n-i-1][n-j-1] = board[n-j-1][i];
			board[n-j-1][i] = tmp;
		}
	}
}

bool moveUp(uint8_t board[SIZE][SIZE]) {
	bool success = false;
	uint8_t x;
	for (x=0;x<SIZE;x++) {
		success |= slideArray(board[x]);
	}
	return success;
}

bool moveLeft(uint8_t board[SIZE][SIZE]) {
	printf("7\r\n");
	bool success;
	rotateBoard(board);
	success = moveUp(board);
	rotateBoard(board);
	rotateBoard(board);
	rotateBoard(board);
	return success;
}

bool moveDown(uint8_t board[SIZE][SIZE]) {
	bool success;
	rotateBoard(board);
	rotateBoard(board);
	success = moveUp(board);
	rotateBoard(board);
	rotateBoard(board);
	return success;
}

bool moveRight(uint8_t board[SIZE][SIZE]) {
	bool success;
	rotateBoard(board);
	rotateBoard(board);
	rotateBoard(board);
	success = moveUp(board);
	rotateBoard(board);
	return success;
}

bool findPairDown(uint8_t board[SIZE][SIZE]) {
	bool success = false;
	uint8_t x,y;
	for (x=0;x<SIZE;x++) {
		for (y=0;y<SIZE-1;y++) {
			if (board[x][y]==board[x][y+1]) return true;
		}
	}
	return success;
}

uint8_t countEmpty(uint8_t board[SIZE][SIZE]) {
	uint8_t x,y;
	uint8_t count=0;
	for (x=0;x<SIZE;x++) {
		for (y=0;y<SIZE;y++) {
			if (board[x][y]==0) {
				count++;
			}
		}
	}
	return count;
}

bool gameEnded(uint8_t board[SIZE][SIZE]) {
	bool ended = true;
	if (countEmpty(board)>0) return false;
	if (findPairDown(board)) return false;
	rotateBoard(board);
	if (findPairDown(board)) ended = false;
	rotateBoard(board);
	rotateBoard(board);
	rotateBoard(board);
	return ended;
}

void addRandom(uint8_t board[SIZE][SIZE]) {
	static bool initialized = false;
	uint8_t x,y;
	uint8_t r,len=0;
	uint8_t n,list[SIZE*SIZE][2];

	if (!initialized) {
		srand(time(NULL));
		initialized = true;
	}

	for (x=0;x<SIZE;x++) {
		for (y=0;y<SIZE;y++) {
			if (board[x][y]==0) {
				list[len][0]=x;
				list[len][1]=y;
				len++;
			}
		}
	}

	if (len>0) {
		r = rand()%len;
		x = list[r][0];
		y = list[r][1];
		n = (rand()%10)/9+1;
		board[x][y]=n;
	}
}

void initBoard(uint8_t board[SIZE][SIZE]) {
	uint8_t x,y;
	for (x=0;x<SIZE;x++) {
		for (y=0;y<SIZE;y++) {
			board[x][y]=0;
		}
	}
	addRandom(board);
	addRandom(board);
	drawBoard(board);
	score = 0;
}

void setBufferedInput(bool enable) {
	static bool enabled = true;
	static struct termios old;
	struct termios news;

	if (enable && !enabled) {
		// restore the former settings
		tcsetattr(STDIN_FILENO,TCSANOW,&old);
		// set the new state
		enabled = true;
	} else if (!enable && enabled) {
		// get the terminal settings for standard input
		tcgetattr(STDIN_FILENO,&news);
		// we want to keep the old setting to restore them at the end
		old = news;
		// disable canonical mode (buffered i/o) and local echo
		news.c_lflag &=(~ICANON & ~ECHO);
		// set the new settings immediately
		tcsetattr(STDIN_FILENO,TCSANOW,&news);
		// set the new state
		enabled = false;
	}
}

void signal_callback_handler(int signum) {
	printf("         TERMINATED         \n");
	setBufferedInput(true);
	printf("\033[?25h\033[m");
	exit(signum);
}
int width,height;
int main() {
	fndLibInit();
	buttonLibInit();
	lcdtextLibInit();
	ledLibInit();
	buzzerLibInit();
	
	CvCapture* capture = cvCreateCameraCapture(2);  // 웹캠인 경우
	IplImage *frame=cvQueryFrame(capture); // 원본영상 
	IplImage *hsv=cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U,3);
	IplImage *grayhsv=cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U,1);
	width = frame->width; 
	height = frame->height; 
	
	int H_Low=0,H_High=32,S_Low=215,S_High=255,V_Low=0,V_High=163;
	int b = 0;
	
	uint8_t board[SIZE][SIZE];
	char c;
	bool success;
	
	int buzzer_flag[8] = {1,1,1,1,1,1,1,1};

	char GameName[16] = "2048 GAME";
	char GameStart[16] = "GMAE START!";
	char GameEnd[16] = "GAME END!";
	char GameRestart[16] = "GAME RESTART!";
	
	char CameraLeft[16] = "Camera_LEFT!";
	char CameraRight[16] = "Camera_RIGHT!";
	char CameraUp[16] = "Camera_UP!";
	char CameraDown[16] = "Camera_Down!";
	
	char ButtonLeft[16] = "Button_LEFT!";
	char ButtonRight[16] = "Button_RIGHT!";
	char ButtonUp[16] = "Button_UP!";
	char ButtonDown[16] = "Button_Down!";
	
	
	ledAllOn();
	buzzerLibOnBuz(262);
	// 브릭들을 읽어옴
	read_bmp();
	buzzerLibOffBuz();
	sleep(1);
	ledAllOff();

	printf("\033[?25l\033[2J");

	// register signal handler for when ctrl-c is pressed
	signal(SIGINT, signal_callback_handler);

	// print lcd
	int screen_width;
    int screen_height;
    int bits_per_pixel;
    int line_length;
	
	
	//FrameBuffer init
    if ( fb_init(&screen_width, &screen_height, &bits_per_pixel, &line_length) < 0 )
	{
		printf ("FrameBuffer Init Failed\r\n");
		return 0;
	}
	
	fb_clear();

	initBoard(board);
	fb_write(board);
	setBufferedInput(false);
	lcdtextwrite(GameName, GameStart, 0);
	

	while(true){
		
		frame = cvQueryFrame(capture); 
		cvCvtColor(frame,hsv,CV_BGR2HSV);
		cvInRangeS(hsv,cvScalar(H_Low,V_Low,S_Low),cvScalar(H_High,V_High,S_High),grayhsv);
		cvFlip(grayhsv,grayhsv,1);
		//cvShowImage("grayhsv",grayhsv);
		for(int i = 0; i < (grayhsv->height); i++) 
		{ 
			for(int j = 0; j < (grayhsv->widthStep); j++) // frame안에 저장되어있는 width 640 * 480영상의 3채널을 사용할 경우 width 640 * 3 =1920이다. 
			{ 				
				int index = i + j*grayhsv->widthStep;
				unsigned int value = grayhsv -> imageData[index];
				if( (b == 0) &&  (j == 300) && ( i== 300)&& (value >= 1))  //가운데 네모에 들어왔을 떄
				{
					printf("가운데\r\n");		
					b = 1;
					success = false;	
					sleep(0.2);
				}
										
				if( (b == 1) &&    (j == 240) && ( i== 479)  && (value >= 10) ) // 가운데 > 왼쪽 이동 
					{
						b = 0;
						printf("왼쪽\r\n");						
						success =  moveRight(board);
						lcdtextwrite(GameName, CameraLeft, 0);
						sleep(0.5);
						
						if (success) {
							drawBoard(board);
							fb_write(board);
							
							fndDisp(score, 0);		// fnd
							usleep(150000);
							addRandom(board);
							
							drawBoard(board);
							fb_write(board);
							
							button_input = 0;
							
							if (gameEnded(board)) {
								printf("         GAME OVER          \n");
								lcdtextwrite(GameName, GameEnd, 0);
								//break;
								usleep(150000);
								button_input = RESTART;
							}
						}				
						sleep(2);			
						success = false;	
					}					
				else if( (b == 1) &&(j == 233) && (i== 10)  && (value >= 10)) // 가운데 > 오른쪽 이동  
				{
					b = 0;
					printf("오른쪽\r\n");					
					success =  moveLeft(board);
					lcdtextwrite(GameName, CameraRight, 0);
					sleep(0.5);
					
						if (success) {
							drawBoard(board);
							fb_write(board);
							
							fndDisp(score, 0);		// fnd
							usleep(150000);
							addRandom(board);
							
							drawBoard(board);
							fb_write(board);
							
							button_input = 0;
							
							if (gameEnded(board)) {
								printf("         GAME OVER          \n");
								lcdtextwrite(GameName, GameEnd, 0);
								//break;
								usleep(150000);
								button_input = RESTART;
							}
						}	
						sleep(2);
						success = false;				
				
				}
				else if( (b == 1) && (j == 10) && (i== 300) && (value >= 10)) // 가운데 > 위쪽 이동 
				{
					b = 0;
					printf("위쪽\r\n");
					success = moveUp(board);
					lcdtextwrite(GameName, CameraUp, 0);
					sleep(0.5);
					
						if (success) {
							drawBoard(board);
							fb_write(board);
							
							fndDisp(score, 0);		// fnd
							usleep(150000);
							addRandom(board);
							
							drawBoard(board);
							fb_write(board);
							
							button_input = 0;
							
							if (gameEnded(board)) {
								printf("         GAME OVER          \n");
								lcdtextwrite(GameName, GameEnd, 0);
								//break;
								usleep(150000);
								button_input = RESTART;
							}
						}					
						sleep(2);
						success = false;	
				
				}
				else if( (b == 1) && (j == 410) && ( i== 270)  &&  (value >= 10)) // 가운데 > 아래쪽 이동 
				{
					b = 0;
					printf("아래쪽\r\n");
					success = moveDown(board);
					lcdtextwrite(GameName, CameraDown, 0);
					sleep(0.5);
					
						if (success) {
							drawBoard(board);
							fb_write(board);
							
							fndDisp(score, 0);		// fnd
							usleep(150000);
							addRandom(board);
							
							drawBoard(board);
							fb_write(board);
							
							button_input = 0;
							
							if (gameEnded(board)) {
								printf("         GAME OVER          \n");
								lcdtextwrite(GameName, GameEnd, 0);
								//break;
								usleep(150000);
								button_input = RESTART;
							}
						}				
						sleep(2);
						success = false;		
					
				}
				else 
				{
					success = false;
				}
			}
		} 

/*		
		if (success) {
			drawBoard(board);
			fb_write(board);
			
			fndDisp(score, 0);		// fnd
			usleep(150000);
			addRandom(board);
			
			drawBoard(board);
			fb_write(board);
			
			button_input = 0;
			
			if (gameEnded(board)) {
				printf("         GAME OVER          \n");
				lcdtextwrite(GameName, GameEnd, 0);
				//break;
				usleep(150000);
				button_input = RESTART;
			}
		}*/
		if(button_input == RESTART) {
			button_input = 0;
			lcdtextwrite(GameName, GameRestart, 0);
			buzzerLibOnBuz(262);
			usleep(150000);
			buzzerLibOffBuz();
			ledAllOff();
			for(int k = 0; k < 8; k++) buzzer_flag[k] = 1;
			initBoard(board);
			drawBoard(board);
			fb_write(board);
		}
		if(button_input == QUIT) {
			// 화면 초기화 필요
			buzzerLibOnBuz(523);
			usleep(300000);
			buzzerLibOffBuz();
			break;
		}
		
		for(int k = 0; k < 4; k++){
			for(int l = 0; l < 4; l++){
				if(board[k][l] == 4 && buzzer_flag[0] == 1){	
					ledOnOff(0,1);
					buzzerLibOnBuz(262);
					usleep(150000);
					buzzerLibOffBuz();
					buzzer_flag[0] = 0;
				}
				else if(board[k][l] == 5 && buzzer_flag[1] == 1){
					ledOnOff(1,1);
					buzzerLibOnBuz(294);
					usleep(150000);
					buzzerLibOffBuz();
					buzzer_flag[1] = 0;
				  }
				else if(board[k][l] == 6 && buzzer_flag[2] == 1) { 
					ledOnOff(2,1);
					buzzerLibOnBuz(330);
					usleep(150000);
					buzzerLibOffBuz();
					buzzer_flag[2] = 0;
				}
				else if(board[k][l] == 7 && buzzer_flag[3] == 1){
					ledOnOff(3,1);
					buzzerLibOnBuz(349);
					usleep(150000);
					buzzerLibOffBuz();
					buzzer_flag[3] = 0;					
				}
				else if(board[k][l] == 8 && buzzer_flag[4] == 1){
					ledOnOff(4,1);
					buzzerLibOnBuz(392);
					usleep(150000);
					buzzerLibOffBuz();
					buzzer_flag[4] = 0;	
				}
				else if(board[k][l] == 9 && buzzer_flag[5] == 1){
					ledOnOff(5,1);
					buzzerLibOnBuz(440);
					usleep(150000);
					buzzerLibOffBuz();
					buzzer_flag[5] = 0;	
				}
				else if(board[k][l] == 10 && buzzer_flag[6] == 1){
					ledOnOff(6,1);
					buzzerLibOnBuz(494);
					usleep(150000);
					buzzerLibOffBuz();
					buzzer_flag[6] = 0;	
				}
				else if(board[k][l] == 11 && buzzer_flag[7] == 1){
					ledOnOff(7,1);
					buzzerLibOnBuz(523);
					usleep(150000);
					buzzerLibOffBuz();
					buzzer_flag[7] = 0;	
				}
			}
		}
	}
	setBufferedInput(true);

	printf("\033[?25h\033[m");

	//fndLibOff();

	fndLibExit();
	buttonLibExit();
	lcdtextLibInit();
	ledLibExit();
	buzzerLibExit();
	
	close_bmp();
	//fb_close();
	
	return EXIT_SUCCESS;
}
