






#ifndef __READ_BITMAP_HEAD__
#define __READ_BITMAP_HEAD__


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


typedef struct bitmap_file_header
{

  uint16_t type;
  uint32_t file_size;
  uint16_t reserved1;
  uint16_t reserved2;
  uint32_t offset;
  
} BITMAP_FileHeader;


typedef struct bitmap_information_header
{

  uint32_t header_size;
  uint32_t width;
  uint32_t height;
  uint16_t planes;
  uint16_t bits_pixel;       //bit_count
  uint32_t compression;
  uint32_t data_size;
  uint32_t h_resolution;
  uint32_t v_resolution;
  uint32_t colors;
  uint32_t important_colors;
  
} BITMAP_InformationHeader;


typedef struct bitmap_rgbquad
{

  uint8_t b;
  uint8_t g;
  uint8_t r;
  uint8_t reserved;
  
} BITMAP_RGBQUAD;


#define BITMAP_Palette BITMAP_RGBQUAD*



typedef struct _bitmap_
{

  /*
  struct bitmap_file_header file_header;
  struct bitmap_information_header information_header;
  struct bitmap_rgbaquad *color_table;
  */

  BITMAP_FileHeader file_header;
  BITMAP_InformationHeader information_header;
  BITMAP_Palette color_table;
  
} BITMAP;


BITMAP *read_bitmap(const char *path);
void print_bitmap_file_header(BITMAP *bitmap);
void print_bitmap_information_header(BITMAP *bitmap);
void print_bitmap_information(BITMAP *bitmap);

uint32_t RGB2HEX(uint8_t r, uint8_t g, uint8_t b);



#endif






