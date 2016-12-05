# include <SDL/SDL.h>
# include <SDL/SDL_image.h>
# include <stdlib.h>
# include <err.h>

# include "pixel_operations.h"

# include "image_blocking.h"
#ifndef DEBUG
#define DEBUG 1 
#endif
//This function puts every possible character in an array of type Img_array
Img_array image_blocking(SDL_Surface *image)
{
  Img_array lines = line_image_blocking(image);
  return column_image_blocking(lines);
}

//This function will split the image into lines where some characters might be
//We check this by seeing if the line contains only pixels from the same color
Img_array line_image_blocking(SDL_Surface *image)
{
  if(DEBUG){printf("Started: Line Block\n");}  
  Img_array lines;

  //result = SDL_CreateRGBSurface(0, image->w, image->h, 32, 0, 0, 0, 0);
  init_array(&lines, 2);
  for (int i = 0; i < image->h; i++)
  {
    if (is_full_line(image, i) == 0)
    {
      int start = i;
      for (; i < image->h; i++)
      {
        if (is_full_line(image, i) == 1)
        {
          SDL_Surface *result;

          result = SDL_CreateRGBSurface(0, image->w, i - start, 32, 0, 0, 0,0);
          SDL_Rect rect = {0, start, image->w, i - start};

          SDL_UnlockSurface(result);

          SDL_BlitSurface(image, &rect, result, NULL);
          insert_array(&lines, result);
          break;
        }
      }
      //break;
    }
  }
  if(DEBUG){printf("End: Line Block\n");}

  return lines;
}

//Same as line_image_blocking but with columns
Img_array column_image_blocking(Img_array lines)
{

  if(DEBUG){printf("Started: Column block\n");}

  Img_array chars;
  init_array(&chars, 5);

  int spaceStart = 0;
  int spaceEnd = 0;
  int interCharSpace = 0;

  for (size_t line = 0; line < lines.used; line++)
  {
    if (interCharSpace != 0)
    {
      SDL_Surface *space;
      space = SDL_CreateRGBSurface(0, 20, 20, 32, 0, 0, 0, 0);
      SDL_FillRect(space, NULL, 0xFFFFFFFF);
      insert_array(&chars, space);
    }
    SDL_Surface *img = lines.array[line];
    for (int i = 0; i < img->w; i++)
    {
      if (is_full_column(img, i) == 0)
      {
        int start = i;
        spaceEnd = i;

        if (spaceStart != 0)
        {
          if (interCharSpace != 0)
          {
            if (spaceEnd - spaceStart > interCharSpace * 2)
            {
              SDL_Surface *space;
              space = SDL_CreateRGBSurface(0, 20, 20, 32, 0, 0, 0, 0);
              SDL_FillRect(space, NULL, 0xFFFFFFFF);
              insert_array(&chars, space);
            }
          }
          else
          {
            interCharSpace = spaceEnd - spaceStart;
          }
        }

        for (; i < img->w; i++)
        {
          if (is_full_column(img, i) == 1)
          {
            spaceStart = i;

            SDL_Surface *result;

            result = SDL_CreateRGBSurface(0, i - start, img->h, 32, 0, 0, 0,0);
            SDL_Rect rect = {start, 0, i - start, img->h};

            SDL_UnlockSurface(result);

            SDL_BlitSurface(img, &rect, result, NULL);

            result = height_fix(result);

            SDL_Surface *scaleResult;
            scaleResult = SDL_CreateRGBSurface(0, 20, 20, 32, 0, 0, 0, 0);

            if (result->h < result->w)
            {
              float scale = (float)20 / result->w;
              scaleResult = SDL_CreateRGBSurface(0, result->w * scale, result->h * scale, 32, 0, 0, 0, 0);
              SDL_SoftStretch(result, NULL, scaleResult, NULL);
            }
            else
            {
              float scale = (float)20 / result->h;
              scaleResult = SDL_CreateRGBSurface(0, result->w * scale, result->h * scale, 32, 0, 0, 0, 0);
              SDL_SoftStretch(result, NULL, scaleResult, NULL);
            }

            SDL_Surface *whiteSpace;
            whiteSpace = SDL_CreateRGBSurface(0, 24, 24, 32, 0, 0, 0, 0);
            SDL_FillRect(whiteSpace, NULL, 0xFFFFFFFF);
            SDL_Rect rekt = {(24 - scaleResult->w) / 2, (24 - scaleResult->h) / 2, 20, 20};

            SDL_BlitSurface(scaleResult, NULL, whiteSpace, &rekt);

            insert_array(&chars, whiteSpace);
            break;
          }
        }
        
      }
    }
  }
	if(DEBUG){printf("End: ChangePixel\n");}

  return chars;
}

SDL_Surface *height_fix(SDL_Surface *img)
{
  int i = 0;
  while (is_full_line(img, i) == 1)
  {
    i++;
  }
  int j = img->h - 1;
  while (is_full_line(img, j) == 1)
  {
    j--;
  }
  SDL_Surface *result;
  result = SDL_CreateRGBSurface(0, img->w, j - i + 1, 32, 0, 0, 0, 0);
  SDL_Rect rect = {0, i, img->w, j - i + 1};

  SDL_BlitSurface(img, &rect, result, NULL);

  return result;
}

//This function checks if a line contains only pixels of the same color
int is_full_line(SDL_Surface *image, int line)
{
  Uint32 color =16777215;// getpixel(image, 0, line);
  for (int i = 1; i < image->w; i++)
  {
	if(DEBUG){printf("End: ChangePixel:%d\n",color);}

    if (getpixel(image, i, line) != color)
      return 0;
  }
  return 1;
}

//Same as is_full_line but with columns
int is_full_column(SDL_Surface *image, int column)
{
  Uint32 color =16777215; // getpixel(image, column, 0);
	 if(DEBUG){printf("End: ChangePixel:%d\n",color);}

  for (int i = 1; i < image->h; i++)
  {
    if (getpixel(image, column, i) != color)
      return 0;
  }
  return 1;
}


//This function initializes an Img_array (array of SDL_Surface *)
void init_array(Img_array *a, size_t init_size)
{
  a->array = (SDL_Surface **)malloc(init_size * sizeof(SDL_Surface *));
  a->used = 0;
  a->size = init_size;
}

//This function insert a SDL_Surface * into an Img_array
void insert_array(Img_array *a, SDL_Surface *element)
{
  if (a->used == a->size)
  {
    a->size *= 2;
    a->array = (SDL_Surface **)realloc(a->array,a->size*sizeof(SDL_Surface *));
  }
  a->array[a->used++] = element;
}

//This function removes an array
void free_array(Img_array *a)
{
  free(a->array);
  a->array = NULL;
  a->used = a->size = 0;
}
