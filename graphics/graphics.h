






#ifndef _GRAPHICS_HEAD_
#define _GRAPHICS_HEAD_


#include <stdint.h>
#include <linux/fb.h>


#include "bitmap.h"


typedef struct point Point;
struct point
{
  int32_t x;
  int32_t y;
};


void init(int32_t *fbfd, struct fb_var_screeninfo *screen_info, char **fbp);
void fbclose();
#define fbinit() init(NULL,NULL,NULL)
#define closefd fbclose

uint32_t get_screen_width();
uint32_t get_screen_height();
struct fb_var_screeninfo get_screen_info();

void draw_point(int32_t x, int32_t y, uint32_t color, uint32_t width);
void draw_line(int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t color, uint32_t width);
void draw_rect_stroke(int32_t x, int32_t y, uint32_t width, uint32_t height, uint32_t color, uint32_t brush_width);
void draw_rect_fill(int32_t x, int32_t y, uint32_t width, uint32_t height, uint32_t color, uint32_t brush_width);
void draw_circle_stroke(int32_t x, int32_t y, uint32_t r, uint32_t color, uint32_t brush_width);
void draw_circle_fill(int32_t x, int32_t y, uint32_t r, uint32_t color, uint32_t brush_width);


void draw_bitmap(int32_t x, int32_t y, BITMAP *bitmap);


#endif // _GRAPHICS_HEAD_







