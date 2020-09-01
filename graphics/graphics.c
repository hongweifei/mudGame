



#include "graphics.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
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
	//	*((uint32_t*)fbp + i) = 0x00ff00ff;

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


int32_t get_line_point(int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t **x, uint32_t **y)
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
  (*x) = (uint32_t*)calloc(n,sizeof(uint32_t));
  (*y) = (uint32_t*)calloc(n,sizeof(uint32_t));


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

  printf("K:%f\nb:%d\n",k,b);

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
  uint32_t *point_x;
  uint32_t *point_y;

  int32_t n = get_line_point(x1,y1,x2,y2,&point_x,&point_y);
  
  for (uint32_t i = 0; i < n; i++)
	{
		draw_point(point_x[i],point_y[i],color,width);
		//printf("x:%d y:%d\n",point_x[i],point_y[i]);
	}
  printf("N:%d\n",n);
}







