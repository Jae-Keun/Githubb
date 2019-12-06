#ifndef _LIB_BITMAP_H_
#define _LIB_BITMAP_H_

#include <stdint.h>

//#define ENABLED_DOUBLE_BUFFERING	1
#define SIZE 4

int read_bmp();
int close_bmp(void);

int fb_init(int * screen_width, int * screen_height, int * bits_per_pixel, int * line_length);
void fb_close(void);
void fb_clear(void);
void fb_write(uint8_t board[SIZE][SIZE]);
void fb_write_reverse(char *picData, int picWidth, int picHeight);
void fb_doubleBufSwap(void);
void fillbox(int i, int j, uint8_t num);

#endif
