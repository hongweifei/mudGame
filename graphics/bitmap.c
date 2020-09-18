


#include "bitmap.h"


BITMAP *read_bitmap(const char *path)
{

  BITMAP *bitmap = (BITMAP*)calloc(1, sizeof(BITMAP));

  FILE *fp = fopen(path,"rb");
  if (fp == NULL)
	{
	  printf("the file open failed.");
	  return NULL;
	}

  
  fseek(fp,0L,SEEK_SET);
  fread(&bitmap->file_header,sizeof(BITMAP_FileHeader),1,fp);

  fseek(fp,14L,SEEK_SET);
  fread(&bitmap->information_header,sizeof(BITMAP_InformationHeader),1,fp);

  uint16_t bit_count = bitmap->information_header.bits_pixel;
  uint32_t width = bitmap->information_header.width;
  uint32_t height = bitmap->information_header.height;

  bitmap->color_table = (BITMAP_Palette)calloc(width * height, sizeof(BITMAP_RGBQUAD));
  
  #ifdef DEBUG
  print_bitmap_information(bitmap);
  #endif

  fseek(fp,54L,SEEK_SET);
  switch (bit_count)
	{
	case 1:
    bitmap = NULL;
	  break;
	case 4:
    bitmap = NULL;
	  break;
	case 8:
    bitmap = NULL;
	  break;
	case 24:

    for (size_t i = height - 1; i >= 0; i--)
    {
      for (size_t j = 0; j < width; j++)
      {
        uint32_t index = i * width + j;
        fread(&bitmap->color_table[index],3L,1,fp);
        bitmap->color_table[index].reserved = 0;
      }
      
    }
    

	  break;
	case 32:

    for (size_t i = height - 1; i >= 0; i--)
    {
      for (size_t j = 0; j < width; j++)
      {
        fread(&bitmap->color_table[i * width + j], sizeof(BITMAP_RGBQUAD), 1,fp);
      }
      
    }

	  break;

  default:
    bitmap = NULL;
    break;
	}

  
  fclose(fp);
  return bitmap;
}


uint32_t RGB2HEX(uint8_t r, uint8_t g, uint8_t b)
{
  return ((1 << 24) + (r << 16) + (g << 8) + b);
}



void print_bitmap_file_header(BITMAP *bitmap)
{
  printf("BITMAP FILE HEADER:\n");
  printf("\tTYPE:%d\n", bitmap->file_header.type); 
  printf("\tFILE SIZE:%d\n", bitmap->file_header.file_size);
  printf("\tRESERVED1:%d\n", bitmap->file_header.reserved1);
  printf("\tRESERVED2:%d\n", bitmap->file_header.reserved2);
  printf("\tOFFSET:%d\n\n", bitmap->file_header.offset);
}

void print_bitmap_information_header(BITMAP *bitmap)
{
  printf("BITMAP INFOTMATION HEADER:\n");
  printf("\tHEADER SIZE:%d\n", bitmap->information_header.header_size);
  printf("\tWIDTH:%d\n", bitmap->information_header.width);
  printf("\tHEIGHT:%d\n", bitmap->information_header.height);
  printf("\tPLANES:%d\n", bitmap->information_header.planes);
  printf("\tBIT COUNT:%d\n", bitmap->information_header.bits_pixel);
  printf("\tCOMPRESSION:%d\n", bitmap->information_header.compression);
  printf("\tDATA SIZE:%d\n", bitmap->information_header.data_size);
  printf("\tX RESOLUTION:%d\n", bitmap->information_header.h_resolution);
  printf("\tY RESOLUTION:%d\n", bitmap->information_header.v_resolution);
  printf("\tCOLORS:%d\n", bitmap->information_header.colors);
  printf("\tIMPORTANT COLORS:%d\n\n", bitmap->information_header.important_colors);
}   

void print_bitmap_information(BITMAP *bitmap)
{
  print_bitmap_file_header(bitmap);
  print_bitmap_information_header(bitmap);
}





