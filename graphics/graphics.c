



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
  //  *((uint32_t*)fbp + i) = 0x00ff00ff;

  //munmap(fbp, size);
  //close(fd);
}


void fbclose()
{
  munmap(fbp, width * height);
  close(fd);
}


uint32_t get_width()
{
  return width;
}

uint32_t get_height()
{
  return height;
}

uint32_t get_xy_index(int32_t x, int32_t y)
{
  uint32_t index = y * width + x;
  if (index >= (width * height))
	return 0;
  return index;
}


void draw_point(int32_t x, int32_t y, uint32_t color)
{
  *((uint32_t*)fbp + get_xy_index(x, y)) = color;
}



void draw_line()
{


}







