# include <SDL/SDL.h>
# include <SDL/SDL_image.h>
# include <stdlib.h>
# include <err.h>

# include "pixel_operations.h"

# include "image_blocking.h"

SDL_Surface* single_image_blocking(SDL_Surface *image, int line)
{
  SDL_Surface *result;
  result = SDL_CreateRGBSurface(0, image->w, image->h, 32, 0, 0, 0, 0);
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
  return result;
}

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