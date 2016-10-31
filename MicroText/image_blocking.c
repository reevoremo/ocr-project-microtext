# include <SDL/SDL.h>
# include <SDL/SDL_image.h>
# include <stdlib.h>
# include <err.h>

# include "pixel_operations.h"

# include "image_blocking.h"

Img_array image_blocking(SDL_Surface *image)
{
  Img_array lines = line_image_blocking(image);
  return column_image_blocking(lines);
}

Img_array line_image_blocking(SDL_Surface *image)
{
  
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

          result = SDL_CreateRGBSurface(0, image->w, i - start, 32, 0, 0, 0, 0);
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
  return lines;
}

Img_array column_image_blocking(Img_array lines)
{
  Img_array chars;

  init_array(&chars, 5);

  for (size_t line = 0; line < lines.used; line++)
  {
    SDL_Surface *img = lines.array[line];
    for (int i = 0; i < img->w; i++)
    {
      if (is_full_column(img, i) == 0)
      {
        int start = i;
        for (; i < img->w; i++)
        {
          if (is_full_column(img, i) == 1)
          {
            SDL_Surface *result;

            result = SDL_CreateRGBSurface(0, i - start, img->h, 32, 0, 0, 0, 0);
            SDL_Rect rect = {start, 0, i - start, img->h};

            SDL_UnlockSurface(result);

            SDL_BlitSurface(img, &rect, result, NULL);
            insert_array(&chars, result);
            break;
          }
        }
          //break;
      }
    }
  }
  return chars;
}

/*SDL_Surface** single_image_blocking(SDL_Surface *image, int line)
{
  SDL_Surface *result;
  SDL_Surface *character;
  static SDL_Surface *array[100];
  character = SDL_CreateRGBSurface(0, image->w, image->h, 32, 0, 0, 0, 0);
  result = SDL_CreateRGBSurface(0, image->w, image->h, 32, 0, 0, 0, 0);

  int pos = 0;
  for (; line < image->h; line++)
  {
    if (is_full_line(image, line) == 0)
    {
      int start = line;
      for (; line < image->h; line++)
      {
        if (is_full_line(image, line) == 1)
        {
          SDL_Rect rect = {0, start, image->w, line - start};
          
          SDL_BlitSurface(image, &rect, result, NULL);
          break;
        }
      }
      break;
    }
  }
  for (int i = 0; i < image -> w; i++)
  {
    if (is_full_column(result, i) == 0)
    {
      int start = i;
      for (; i < image->w; i++)
      {
        if (is_full_column(result, i) == 1)
        {
          SDL_Rect rect = {start, 0, i - start, image->h};
          SDL_BlitSurface(result, &rect, character, NULL);
          break;
        }
      }
      array[pos] = character;
      pos++;
      character = SDL_CreateRGBSurface(0, image->w, image->h, 32, 0, 0, 0, 0);
    }
  }
  return array;
}*/

int is_full_line(SDL_Surface *image, int line)
{
  Uint32 color = getpixel(image, 0, line);
  for (int i = 1; i < image->w; i++)
  {
    if (getpixel(image, i, line) != color)
      return 0;
  }
  return 1;
}

int is_full_column(SDL_Surface *image, int column)
{
  Uint32 color = getpixel(image, column, 0);
  for (int i = 1; i < image->h; i++)
  {
    if (getpixel(image, column, i) != color)
      return 0;
  }
  return 1;
}



void init_array(Img_array *a, size_t init_size)
{
  a->array = (SDL_Surface **)malloc(init_size * sizeof(SDL_Surface *));
  a->used = 0;
  a->size = init_size;
}

void insert_array(Img_array *a, SDL_Surface *element)
{
  if (a->used == a->size)
  {
    a->size *= 2;
    a->array = (SDL_Surface **)realloc(a->array, a->size * sizeof(SDL_Surface *));
  }
  a->array[a->used++] = element;
}

void free_array(Img_array *a)
{
  free(a->array);
  a->array = NULL;
  a->used = a->size = 0;
}

/*void char_init_array(Char_array *a, size_t init_size)
{
  a->array = (Img_array *)malloc(init_size * sizeof(Img_array));
  a->used = 0;
  a->size = init_size;
}

void char_insert_array(Char_array *a, Img_array element)
{
  if (a->used == a->size)
  {
    a->size *= 2;
    a->array = (Img_array *)realloc(a->array, a->size * sizeof(Img_array));
  }
  a->array[a->used++] = element;
}

void char_free_array(Char_array *a)
{
  free(a->array);
  a->array = NULL;
  a->used = a->size = 0;
}*/