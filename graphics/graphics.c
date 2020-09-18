



#include "graphics.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>



static int32_t fd = 0;
static struct fb_var_screeninfo info;
static char *fbp = NULL;
static uint32_t width = 0;
static uint32_t height = 0;


void init(int32_t *fbfd, struct fb_var_screeninfo *screen_info, char **f_bp)
{


  // Open the file for reading and writing
  fd = open("/dev/fb0", O_RDWR);
  if (!fd)
	{
	  printf("Error: cannot open framebuffer device.\n");
	  exit(1);
	}
  printf("The framebuffer device was opened successfully.\n");

  // Get variable screen information
  if (ioctl(fd, FBIOGET_VSCREENINFO, &info))
    {
	  printf("Error reading variable information.\n");
	  exit(1);
    }
  printf("R:%d,G:%d,B:%d \n", info.red, info.green, info.blue );
  printf("%dx%d, %dbpp\n", info.xres, info.yres, info.bits_per_pixel);
  

  // Figure out the size of the screen in bytes
  width = info.xres;
  height = info.yres;
  uint64_t size = width * height * info.bits_per_pixel / 8;
  printf("pixels:%d,screensize:%dB\n", width * height, size);
  printf("width:%d\nheight:%d\n",width,height);
  

  // Map the device to memory  将屏幕缓冲区映射到用户空间。
  fbp = (char *)mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (fbp[0] == -1)
	{
	  printf("Error: failed to map framebuffer device to memory.\n");
	  exit(4);
	}
  printf("The framebuffer device was mapped to memory successfully.\n");

  

  if (fbfd != NULL)
	*fbfd = fd;
  if (screen_info != NULL)
	*screen_info = info;
  if (f_bp != NULL)
	*f_bp = fbp;
  
  
  //for (int i = 0; i < size; i++)
  //  *((uint32_t*)fbp + i) = 0x00ff00ff;
  
  //munmap(fbp, size);
  //close(fd);
}


void fbclose()
{
  munmap(fbp, width * height);
  close(fd);
}


uint32_t get_screen_width()
{
  return width;
}

uint32_t get_screen_height()
{
  return height;
}

struct fb_var_screeninfo get_screen_info()
{
  return info;
}

uint32_t get_xy_index(int32_t x, int32_t y)
{
  uint32_t index = y * width + x;
  if (index >= (width * height))
	return 0;
  return index;
}


int32_t linear_function(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t **x, int32_t **y)
{
  //y = kx + b;
  int32_t b = y1;
  if (x2 < x1)
	b = y2;

  //-kx = b - y;
  //kx = y - b;
  //k = (y - b) / x;
  double k = 0;
  if (x1 != 0)
	{
	  k = y1 - b;
	  k /= x1;
	}
  if (x2 != 0 && k == 0)
	{
	  k = y2 - b;
	  k /= x2;
	}
  
  
  int32_t n = x2 - x1;
  if (x2 < x1)
	n = x1 - x2;
  
  //uint32_t point_x[n];
  //uint32_t point_y[n];
  (*x) = (int32_t*)calloc(n,sizeof(int32_t));
  (*y) = (int32_t*)calloc(n,sizeof(int32_t));
  
  
  if (x1 < x2)
	{
	  for (uint32_t i = 0; i < n; i++)
		{
		  (*x)[i] = x1 + i;
		  (*y)[i] = k * (*x)[i] + b;
		}
	}
  else
	{
	  for (uint32_t i = 0; i < n; i++)
	  {
	    (*x)[i] = x2 + i;
	    (*y)[i] = k * (*x)[i] + b;
	  }
	}

  #ifdef DEBUG
  printf("k:%f\nb:%d\n",k,b);
  #endif
  
  return n;
}



void quadratic_function(int32_t way,int32_t h, int32_t k, uint32_t width, uint32_t height, Point **point)
{
   /**
   * Quadratic function
   *
   * y = ax * x + bx + c;    a != 0
   * y = a(x-x1)(x-x2);      a != 0
   * y = a(x-h)(x-h) + k;    a != 0   <'_'> use this
   *
   */

  const int32_t x1 = h;
  const int32_t y1 = k;

  if (way == 0)
	return;

  struct point left;
  struct point right;

  left.x = x1 - (width / 2);
  right.x = x1 + (width / 2);
  
  if (way > 0)
	  left.y = y1 + height;
  else if (way < 0)
	  left.y = y1 - height;

  right.y = left.y;
  

  // left.y = a * left.x * left.x + b * left.x + y1(c);
  // right.y = a * right.x * right.x + b * right.x + y1(c);
  // 1 = a * -1 * -1 + b * -1 + 0;
  // 1 = a * 1 * 1 + b * 1 + 0;
  // a + (-b) = a + b
  // 5 = a * -5 * -5 + b * -5 + 0
  // 5 = a * 5 * 5 + b * 5 + 0
  // 25a + (-5b) = 25a + 5b
  // b = 0
  // const int32_t b = 0;

  // left.y = a * left.x * left.x
  // left.y / a = left.x * left.x
  // a = left.y / left.x / left.x
  // double a = left.y / left.x / left.x;


  // x1 = -b / 2a
  // x1 * 2a = -b
  // -b = x1 * 2a
  // b = -(x1 * 2a)
  // const double b = x1 * 2 * a * -1;
  

  // y1 = (4ac - b * b)(4ac - b * b) / 4a
  // y1 = [(16a * a * c * c) - (4ac * b * b) - (4ac * b * b) + (b * b * b * b)] / 4a
  // y1 * 4a = (16a * a * c * c) - (4ac * b * b) - (4ac * b * b) + (b * b * b * b)
  // y1 * 4a + (4ac * b * b) + (4ac * b * b) - (b * b * b * b) = 16a * a * c * c
  // [y1 * 4a + (4ac * b * b) + (4ac * b * b) - (b * b * b * b)] / 16a / a / c = c
  // c = [y1 * 4a + (4ac * b *b) + (4ac * b * b) - (b * b * b * b)] / 16a / a / c
  // c * 16a * a * c * c = y1 * 4a + (4ac * b *b) + (4ac * b * b) - (b * b * b * b)
  // c * 16a * a * c * c - (4ac * b * b) - (4ac * b * b) = y1 * 4a - (b * b * b * b)
  // c * c * c - (4ac * b * b) - (4ac * b * b) = [(y1 * 4a) - (b * b * b * b)] / 16a / a
  // const double c = ;


  // h(x1) = -b / 2a
  // y = a(x - x1)(x - x1) + k(y1)
  // y = a(x - (-b / 2a))(x - (-b / 2a)) + k(y1)
  // y = ax - a(-b / 2a)(x - (-b / 2a)) + k
  // y = (ax - ah)(x - h) + k
  // y = ax * x - ahx - ahx + ah * h + k
  // y = a * x * x - a * h * x - a * h * x + a * h * h + k
  
  // y = a(x - x1)(x - x1) + y1
  // y - y1 = a(x - x1)(x - x1)
  // (y - y1) / (x - x1) / (x - x1) = a

  
  double a = left.y - k;
  a /= (left.x - h);
  a /= (left.x - h);
  //double a = (left.y - k) / (left.x - h) / (left.x - h);
  

  if (way < 0 && a > 0)
	a = -(a);
  else if (way > 0 && a < 0)
	a = -(a);
  

  uint32_t n = width;
  (*point) = (struct point*)calloc(n, sizeof(struct point));

  for (uint32_t i = 0; i < n; i++)
	{
	  (*point)[i].x = left.x + i;
	  (*point)[i].y = (*point)[i].x - h;
	  (*point)[i].y *= (*point)[i].x - h;
	  (*point)[i].y *= a;
	  (*point)[i].y += k;
	  //(*point)[i].y = a * ((*point)[i].x - h) * ((*point)[i].x - h) + k;
	}

  
  #ifdef DEBUG
  printf("y=%f(x - %d)(x - %d)+%d\n",a,x1,x1,y1);
  #endif
  
  
  return;
}




uint32_t half_circle(int32_t way, int32_t x0, int32_t y0, uint32_t r, Point **point)
{
  /**
   * y = y0 + sqrt((r * r) - (x - x0) * (x - x0))
   * y = y0 - sqrt((r * r) - (x - x0) * (x - x0))
   */

  if (way == 0)
	return 0;

  const uint32_t n = 2 * r;
  struct point left = {x0 - r, y0};
  (*point) = (struct point*)calloc(n, sizeof(struct point));

  
  if (way > 0)
	{ 
	  for (uint32_t i = 0; i < n; i++)
		{
		  (*point)[i].x = left.x + i;
		  (*point)[i].y = y0 - sqrt((r * r) - ((*point)[i].x - x0) * ((*point)[i].x - x0));
		}

	  #ifdef DEBUG
	  printf("y = %d + sqrt(%d * %d - (x - %d)(x - %d))",y0,r,r,x0,x0);
	  #endif
	}
  else if (way < 0)
	{ 
	  for (uint32_t i = 0; i < n; i++)
		{
		  (*point)[i].x = left.x + i;
		  (*point)[i].y = y0 + sqrt((r * r) - ((*point)[i].x - x0) * ((*point)[i].x - x0));
		}

	  
	  #ifdef DEBUG
	  printf("y = %d - sqrt(%d * %d - (x - %d)(x - %d))",y0,r,r,x0,x0);
	  #endif
	}

  
  return n;
}



void draw_point(int32_t x, int32_t y, uint32_t color, uint32_t width)
{
  if (x < 0 || y < 0)
	return;
  
  //uint32_t w = width - 1;
  //int32_t end_x = x + w;
  //int32_t end_y = y + w;
  
  for (uint32_t i = 0; i < width; i++)
	for (uint32_t j = 0; j < width; j++)
	  {
		*((uint32_t*)fbp + get_xy_index(x + j, y + i)) = color;
	  }
}



void draw_line(int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t color, uint32_t width)
{
  if (x1 == x2)
	{
	  if (y1 > y2)
		{
		  for (uint32_t i = 0; i < y1 - y2; i++)
			draw_point(x1,y2 + i,color,width);
		  return;
		}
	  
	  //y1 < y2
	  for (uint32_t i = 0; i < y2 - y1; i++)
		draw_point(x1,y1 + i,color,width);
	  return;
	}
  
  
  int32_t *point_x;
  int32_t *point_y;
  
  int32_t n = linear_function(x1,y1,x2,y2,&point_x,&point_y);
  
  for (uint32_t i = 0; i < n; i++)
	{
	  draw_point(point_x[i],point_y[i],color,width);
	  //printf("x:%d y:%d\n",point_x[i],point_y[i]);
	}

  #ifdef DEBUG
  printf("n:%d\n",n);
  #endif

  free(point_x);
  free(point_y);
}


void draw_rect_stroke(int32_t x, int32_t y, uint32_t width, uint32_t height, uint32_t color, uint32_t brush_width)
{
  int32_t right = x + width;
  int32_t bottom = y + height;
  draw_line(x, y, right, y, color, brush_width);
  draw_line(right, y, right, bottom, color, brush_width);
  draw_line(right, bottom, x, bottom, color, brush_width);
  draw_line(x, bottom, x, y, color, brush_width);
}




void draw_rect_fill(int32_t x, int32_t y, uint32_t width, uint32_t height, uint32_t color, uint32_t brush_width)
{
  int32_t right = x + width;
  int32_t bottom = y + height;
  
  for (uint32_t i = y; i <= bottom; i++)
	{
	  draw_line(x,i,right,i,color,brush_width);
	}
}









void draw_circle_stroke(int32_t x, int32_t y, uint32_t r, uint32_t color, uint32_t brush_width)
{
  
  Point *point1;
  Point *point2;

  uint32_t n = half_circle(1,x,y,r,&point1);
  half_circle(-1,x,y,r,&point2);

  for (uint32_t i = 0; i < n; i++)
	{
	  draw_point(point1[i].x, point1[i].y, color, brush_width);
	  draw_point(point2[i].x, point2[i].y, color, brush_width);
	}

  free(point1);
  free(point2);
}


void draw_circle_fill(int32_t x, int32_t y, uint32_t r, uint32_t color, uint32_t brush_width)
{
  
}



void draw_bitmap(int32_t x, int32_t y, BITMAP *bitmap)
{
  for (size_t i = 0; i < bitmap->information_header.height; i++)
  {
    for (size_t j = 0; j < bitmap->information_header.width; j++)
    {
      BITMAP_RGBQUAD *rgb = &bitmap->color_table[i * width + j];
      draw_point(x + j, y + i, RGB2HEX(rgb->r,rgb->g,rgb->b), 1);
    }
    
  }
  
}







