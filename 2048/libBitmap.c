#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <unistd.h>     // for open/close
#include <fcntl.h>      // for O_RDWR
#include <sys/ioctl.h>  // for ioctl
#include <sys/mman.h>
#include <linux/fb.h>   // for fb_var_screeninfo, FBIOGET_VSCREENINFO

#include "libBitmap.h"
#include "bitmapFileHeader.h"

static char *pDib;

static int fbfd;
static int fbHeight=0;	//현재 하드웨어의 사이즈
static int fbWidth=0;	//현재 하드웨어의 사이즈
static unsigned long   *pfbmap;	//프레임 버퍼
static struct fb_var_screeninfo fbInfo;	//To use to do double buffering.
static struct fb_fix_screeninfo fbFixInfo;	//To use to do double buffering.

unsigned long brick_color[12][130 *130];

//Read BMP from filename, to data, pDib, with cols, rows.
int read_bmp()
{
	int coor_y=0;
	int coor_x=0;
    int n =3;
    int k = 0;
    unsigned long temp[3];
    FILE *fp;
	unsigned char data, trash;

	for(int i = 0; i < 130 * 130; i++)
		brick_color[0][i] = 0xf0f0f0;

	//----------------------- 2 -------------------------//
    fp  =  fopen("2048-2.bmp", "rb");
    if(fp == NULL) {
        printf("ERROR\n");
        return -1;
    }

	for(int i = 0; i< 54;i++){
		fscanf(fp, "%c", &data);
	}
	
	while(!feof(fp)) {
		for(int i = 0; i< 390; i++){
			fscanf(fp, "%c", &data);
			if(n > 0){
				n--;
				temp[n] = (unsigned long) data;
			}
			if(n == 0) {
				brick_color[1][k] = (temp[2] << 16) + (temp[1] << 8) + temp[0];
				n = 3;
				k++;
			}
		}
		fscanf(fp, "%c", &trash);
		fscanf(fp, "%c", &trash);
	}
    fclose(fp);

	//----------------------- 4 -------------------------//
    fp  =  fopen("2048-4.bmp", "rb");
    k=0;
    if(fp == NULL) {
        printf("ERROR\n");
        return -1;
    }

	for(int i = 0; i< 54;i++){
		fscanf(fp, "%c", &data);
	}
	
	while(!feof(fp)) {
		for(int i = 0; i< 390; i++){
			fscanf(fp, "%c", &data);
			if(n > 0){
				n--;
				temp[n] = (unsigned long) data;
			}
			if(n == 0) {
				brick_color[2][k] = (temp[2] << 16) + (temp[1] << 8) + temp[0];
				n = 3;
				k++;
			}
		}
		fscanf(fp, "%c", &trash);
		fscanf(fp, "%c", &trash);
	}
    fclose(fp);
    
    //----------------------- 8 -------------------------//
    fp  =  fopen("2048-8.bmp", "rb");
    k=0;
    if(fp == NULL) {
        printf("ERROR\n");
        return -1;
    }

	for(int i = 0; i< 54;i++){
		fscanf(fp, "%c", &data);
	}
	
	while(!feof(fp)) {
		for(int i = 0; i< 390; i++){
			fscanf(fp, "%c", &data);
			if(n > 0){
				n--;
				temp[n] = (unsigned long) data;
			}
			if(n == 0) {
				brick_color[3][k] = (temp[2] << 16) + (temp[1] << 8) + temp[0];
				n = 3;
				k++;
			}
		}
		fscanf(fp, "%c", &trash);
		fscanf(fp, "%c", &trash);
	}
    fclose(fp);
    
    //----------------------- 16 -------------------------//
    fp  =  fopen("2048-16.bmp", "rb");
    k=0;
    if(fp == NULL) {
        printf("ERROR\n");
        return -1;
    }

	for(int i = 0; i< 54;i++){
		fscanf(fp, "%c", &data);
	}
	
	while(!feof(fp)) {
		for(int i = 0; i< 390; i++){
			fscanf(fp, "%c", &data);
			if(n > 0){
				n--;
				temp[n] = (unsigned long) data;
			}
			if(n == 0) {
				brick_color[4][k] = (temp[2] << 16) + (temp[1] << 8) + temp[0];
				n = 3;
				k++;
			}
		}
		fscanf(fp, "%c", &trash);
		fscanf(fp, "%c", &trash);
	}
    fclose(fp);
    
    //----------------------- 32 -------------------------//
    fp  =  fopen("2048-32.bmp", "rb");
    k=0;
    if(fp == NULL) {
        printf("ERROR\n");
        return -1;
    }

	for(int i = 0; i< 54;i++){
		fscanf(fp, "%c", &data);
	}
	
	while(!feof(fp)) {
		for(int i = 0; i< 390; i++){
			fscanf(fp, "%c", &data);
			if(n > 0){
				n--;
				temp[n] = (unsigned long) data;
			}
			if(n == 0) {
				brick_color[5][k] = (temp[2] << 16) + (temp[1] << 8) + temp[0];
				n = 3;
				k++;
			}
		}
		fscanf(fp, "%c", &trash);
		fscanf(fp, "%c", &trash);
	}
    fclose(fp);
    
    //----------------------- 64 -------------------------//
    fp  =  fopen("2048-64.bmp", "rb");
    k=0;
    if(fp == NULL) {
        printf("ERROR\n");
        return -1;
    }

	for(int i = 0; i< 54;i++){
		fscanf(fp, "%c", &data);
	}
	
	while(!feof(fp)) {
		for(int i = 0; i< 390; i++){
			fscanf(fp, "%c", &data);
			if(n > 0){
				n--;
				temp[n] = (unsigned long) data;
			}
			if(n == 0) {
				brick_color[6][k] = (temp[2] << 16) + (temp[1] << 8) + temp[0];
				n = 3;
				k++;
			}
		}
		fscanf(fp, "%c", &trash);
		fscanf(fp, "%c", &trash);
	}
    fclose(fp);
    
    //----------------------- 128 -------------------------//
    fp  =  fopen("2048-128.bmp", "rb");
    k=0;
    if(fp == NULL) {
        printf("ERROR\n");
        return -1;
    }

	for(int i = 0; i< 54;i++){
		fscanf(fp, "%c", &data);
	}
	
	while(!feof(fp)) {
		for(int i = 0; i< 390; i++){
			fscanf(fp, "%c", &data);
			if(n > 0){
				n--;
				temp[n] = (unsigned long) data;
			}
			if(n == 0) {
				brick_color[7][k] = (temp[2] << 16) + (temp[1] << 8) + temp[0];
				n = 3;
				k++;
			}
		}
		fscanf(fp, "%c", &trash);
		fscanf(fp, "%c", &trash);
	}
    fclose(fp);
    
    //----------------------- 256 -------------------------//
    fp  =  fopen("2048-256.bmp", "rb");
    k=0;
    if(fp == NULL) {
        printf("ERROR\n");
        return -1;
    }

	for(int i = 0; i< 54;i++){
		fscanf(fp, "%c", &data);
	}
	
	while(!feof(fp)) {
		for(int i = 0; i< 390; i++){
			fscanf(fp, "%c", &data);
			if(n > 0){
				n--;
				temp[n] = (unsigned long) data;
			}
			if(n == 0) {
				brick_color[8][k] = (temp[2] << 16) + (temp[1] << 8) + temp[0];
				n = 3;
				k++;
			}
		}
		fscanf(fp, "%c", &trash);
		fscanf(fp, "%c", &trash);
	}
    fclose(fp);
    
    //----------------------- 512 -------------------------//
    fp  =  fopen("2048-512.bmp", "rb");
    k=0;
    if(fp == NULL) {
        printf("ERROR\n");
        return -1;
    }

	for(int i = 0; i< 54;i++){
		fscanf(fp, "%c", &data);
	}
	
	while(!feof(fp)) {
		for(int i = 0; i< 390; i++){
			fscanf(fp, "%c", &data);
			if(n > 0){
				n--;
				temp[n] = (unsigned long) data;
			}
			if(n == 0) {
				brick_color[9][k] = (temp[2] << 16) + (temp[1] << 8) + temp[0];
				n = 3;
				k++;
			}
		}
		fscanf(fp, "%c", &trash);
		fscanf(fp, "%c", &trash);
	}
    fclose(fp);
    
    //----------------------- 1024 -------------------------//
    fp  =  fopen("2048-1024.bmp", "rb");
    k=0;
    if(fp == NULL) {
        printf("ERROR\n");
        return -1;
    }

	for(int i = 0; i< 54;i++){
		fscanf(fp, "%c", &data);
	}
	
	while(!feof(fp)) {
		for(int i = 0; i< 390; i++){
			fscanf(fp, "%c", &data);
			if(n > 0){
				n--;
				temp[n] = (unsigned long) data;
			}
			if(n == 0) {
				brick_color[10][k] = (temp[2] << 16) + (temp[1] << 8) + temp[0];
				n = 3;
				k++;
			}
		}
		fscanf(fp, "%c", &trash);
		fscanf(fp, "%c", &trash);
	}
    fclose(fp);
    
    //----------------------- 2048 -------------------------//
    fp  =  fopen("2048-2048.bmp", "rb");
    k=0;
    if(fp == NULL) {
        printf("ERROR\n");
        return -1;
    }

	for(int i = 0; i< 54;i++){
		fscanf(fp, "%c", &data);
	}
	
	while(!feof(fp)) {
		for(int i = 0; i< 390; i++){
			fscanf(fp, "%c", &data);
			if(n > 0){
				n--;
				temp[n] = (unsigned long) data;
			}
			if(n == 0) {
				brick_color[11][k] = (temp[2] << 16) + (temp[1] << 8) + temp[0];
				n = 3;
				k++;
			}
		}
		fscanf(fp, "%c", &trash);
		fscanf(fp, "%c", &trash);
	}
    fclose(fp);

	return 1;
}

int close_bmp(void)     // DIB(Device Independent Bitmap)
{
    free(pDib);
	return 1;
}

#define PFBSIZE 			(fbHeight*fbWidth*sizeof(unsigned long)*2)	//Double Buffering
#define DOUBLE_BUFF_START	(fbHeight*fbWidth)	///Double Swaping
static int currentEmptyBufferPos = 0;
//1 Pixel 4Byte Framebuffer.
int fb_init(int * screen_width, int * screen_height, int * bits_per_pixel, int * line_length)
{
    struct  fb_fix_screeninfo fbfix;

	if( (fbfd = open(FBDEV_FILE, O_RDWR)) < 0)
    {
        printf("%s: open error\n", FBDEV_FILE);
        return -1;
    }

    if( ioctl(fbfd, FBIOGET_VSCREENINFO, &fbInfo) )
    {
        printf("%s: ioctl error - FBIOGET_VSCREENINFO \n", FBDEV_FILE);
		close(fbfd);
        return -1;
    }
    /*
   	if( ioctl(fbfd, FBIOGET_FSCREENINFO, &fbFixInfo) )
    {
        printf("%s: ioctl error - FBIOGET_FSCREENINFO \n", FBDEV_FILE);
        close(fbfd);
        return -1;
    }
    * */
	printf ("FBInfo.YOffset:%d\r\n",fbInfo.yoffset);
	fbInfo.yoffset = 0;
	ioctl(fbfd, FBIOPUT_VSCREENINFO, &fbInfo);	//슉!
/*
    if (fbInfo.bits_per_pixel != 32)
    {
        printf("bpp is not 32\n");
		close(fbfd);
        return -1;
    }	
*/
    fbWidth = *screen_width    =   fbInfo.xres;
    fbHeight = *screen_height   =   fbInfo.yres;
    *bits_per_pixel  =   fbInfo.bits_per_pixel;
    // *line_length     =   fbFixInfo.line_length;

	pfbmap  =   (unsigned long *)
        mmap(0, PFBSIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fbfd, 0);
	
	if ((unsigned)pfbmap == (unsigned)-1)
    {
        printf("fbdev mmap failed\n");
        close(fbfd);
		return -1;
    }
	#ifdef ENABLED_DOUBLE_BUFFERING
		currentEmptyBufferPos = DOUBLE_BUFF_START;	//더블버퍼링 임시 주소로 할당
	#else
		currentEmptyBufferPos = 0;
	#endif
	//printf ("CurrentEmptyBuffPos:%d\r\n",currentEmptyBufferPos);
	return 1;
}

void fb_clear(void)
{
	int coor_y = 0;
	int coor_x = 0;
	// fb clear - black
    printf("%d \t %d \n", fbHeight, fbWidth);
    for(coor_y = 0; coor_y < fbHeight; coor_y++) 
	{
        unsigned long *ptr =   pfbmap + currentEmptyBufferPos + (fbWidth * coor_y);
        for(coor_x = 0; coor_x < fbWidth; coor_x++)
        {
			if(coor_x < 212 || coor_x > 812)
			*ptr++ = 0x808080;
			else
			*ptr++ = 0xffffff;
        }
        
		for(int i =0;i<4;i++)
		{
			for(int j = 0; j< 4; j++)
			{
				fillbox(j,i, 0);
			}
		}
        
    }
    
	#ifdef ENABLED_DOUBLE_BUFFERING
		fb_doubleBufSwap();
	#endif
}
void fb_doubleBufSwap(void)
{
	if (currentEmptyBufferPos == 0)
	{
		fbInfo.yoffset = 0;
		currentEmptyBufferPos = DOUBLE_BUFF_START;
	}
	else
	{
		fbInfo.yoffset = fbHeight;
		currentEmptyBufferPos = 0;		
	}
	ioctl(fbfd, FBIOPUT_VSCREENINFO, &fbInfo);	//슉!
}
void fb_close(void)
{
	printf ("Memory UnMapped!\r\n");
    munmap( pfbmap, PFBSIZE);
	printf ("CloseFB\r\n");
    close( fbfd);
}
void fb_write_reverse(char* picData, int picWidth, int picHeight)
{
	int coor_y=0;
	int coor_x=0;
	int targetHeight = (fbHeight<picHeight)?fbHeight:picHeight;	//if Screen과 파일 사이즈가 안맞으면
	int targetWidth = (fbWidth<picWidth)?fbWidth:picWidth;		//if Screen과 파일 사이즈가 안맞으면
	
	for(coor_y = 0; coor_y < targetHeight; coor_y++) 
	{
		int bmpYOffset = coor_y*picWidth*3; ///Every 1Pixel requires 3Bytes.
		int bmpXOffset = 0;
		for (coor_x=0; coor_x < targetWidth; coor_x++)
		{
			//BMP: B-G-R로 인코딩 됨, FB: 0-R-G-B로 인코딩 됨.
			pfbmap[coor_y*fbWidth+ (coor_x) + currentEmptyBufferPos] = 
				((unsigned long)(picData[bmpYOffset+bmpXOffset+0])<<16) 	+
				((unsigned long)(picData[bmpYOffset+bmpXOffset+1])<<8) 		+
				((unsigned long)(picData[bmpYOffset+bmpXOffset+2]));
			bmpXOffset+=3;	//Three Byte.
		}
    }	
	#ifdef ENABLED_DOUBLE_BUFFERING
		fb_doubleBufSwap();
	#endif	
}
#define SQAR_SIZE 140

void fillbox(int i, int j, uint8_t num)
{
	int coor_y=0;
	int coor_x=0;
	int z = 0;
	int y = 1;
	/*
	for(coor_y = 25+SQAR_SIZE*i;coor_y < 15 + SQAR_SIZE*(i+1); coor_y ++)
	{
		for(coor_x = 237+SQAR_SIZE*j; coor_x < 227 + SQAR_SIZE*(j+1); coor_x++)
		{
			//pfbmap[coor_y*fbWidth+ (fbWidth-coor_x) + currentEmptyBufferPos] = 0xf0f0f0;
			//pfbmap[coor_y*fbWidth+ (fbWidth-coor_x) + currentEmptyBufferPos] = color;
			pfbmap[coor_y*fbWidth+ (fbWidth-coor_x) + currentEmptyBufferPos] = brick_color[num][z];
			z++;
		}
	}*/
	for(coor_x = 237+SQAR_SIZE*j; coor_x < 227 + SQAR_SIZE*(j+1); coor_x++)
	{
		for(coor_y = 25+SQAR_SIZE*i;coor_y < 15 + SQAR_SIZE*(i+1); coor_y ++)
		{
			//pfbmap[coor_y*fbWidth+ (fbWidth-coor_x) + currentEmptyBufferPos] = 0xf0f0f0;
			//pfbmap[coor_y*fbWidth+ (fbWidth-coor_x) + currentEmptyBufferPos] = color;
			pfbmap[coor_y*fbWidth+ (fbWidth-coor_x) + currentEmptyBufferPos] = brick_color[num][130 * y - z];
			z++;
		}
		y++;
		z=0;
	}
}


void fb_write(uint8_t board[SIZE][SIZE])
{
	int i, j;
	int coor_y=0;
	int coor_x=0;
	int targetHeight = 600;
    
    for(int i =0;i<4;i++)
    {
		for(int j = 0; j< 4; j++)
		{
			fillbox(j,i, board[3-j][3-i]);
		}
	}
    
	#ifdef ENABLED_DOUBLE_BUFFERING
		fb_doubleBufSwap();
	#endif
}
