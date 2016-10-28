# ifndef IMAGE_BLOCKING_H
# define IMAGE_BLOCKING_H
 
# include <stdlib.h>
# include <SDL.h>
 
SDL_Surface* single_image_blocking(SDL_Surface *image, int line);

int is_full_line(SDL_Surface *image, int line);
 
# endif