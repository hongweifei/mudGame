






#ifndef _GRAPHICS_HEAD_
#define _GRAPHICS_HEAD_


#include <stdint.h>
#include <linux/fb.h>


void init(int32_t *fbfd, struct fb_var_screeninfo *screen_info, char **fbp);
void fbclose();
#define fbinit() init(NULL,NULL,NULL)
#define closefd fbclose

uint32_t get_width();
uint32_t get_height();

void draw_point(int32_t x, int32_t y, uint32_t color);
void draw_line();




#endif

