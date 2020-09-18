


#include "graphics.h"


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#include "bitmap.h"

/*
void init()
{
  int fd = 0;
  struct fb_var_screeninfo info;
  uint64_t size = 0;
  char *fbp = 0;
  int_fast32_t x = 0, y = 0;
  int_fast32_t i = 0;


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
  size = info.xres * info.yres * info.bits_per_pixel / 8;
  printf("pixels:%d,screensize:%dB\n", info.xres * info.yres, size);


  // Map the device to memory  将屏幕缓冲区映射到用户空间。
  fbp = (char *)mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (fbp[0] == -1)
	{
	  printf("Error: failed to map framebuffer device to memory.\n");
	  exit(4);
	}
  printf("The framebuffer device was mapped to memory successfully.\n");

  for (int i = 0; i < size; i++)
	*((uint32_t*)fbp + i) = 0x00ff00ff;

  munmap(fbp, size);
  close(fd);
}
*/


int main()
{
  BITMAP *bitmap = read_bitmap("./test.bmp");
  if (bitmap == NULL)
  {
    printf("read bitmap failed.\n");
  }

  print_bitmap_information(bitmap);

  //fbinit();
  //cbreak();

  /*
  while (getch() != 'q')
	{
	  draw_line(1920,1080,0,0,0x00ff00ff,1);
	  draw_line(1000,0,1000,1079,0x00ff00ff,1);
	  draw_rect_stroke(1000,200,100,100,0x00ff00ff,2);
	  draw_circle_stroke(1000,800,200,0x00ff00ff,2);
    //draw_bitmap(0,0,bitmap);
	}
  */

  
  
  //fbclose();
  return 0;
}



