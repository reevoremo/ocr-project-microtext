# ifndef IMAGE_BLOCKING_H
# define IMAGE_BLOCKING_H
 
# include <stdlib.h>
# include <SDL.h>

typedef struct {
  SDL_Surface **array;
  size_t used;
  size_t size;
} Img_array;

/*typedef struct {
  Img_array *array;
  size_t used;
  size_t size;
} Char_array;*/

Img_array image_blocking(SDL_Surface *image);

Img_array line_image_blocking(SDL_Surface *image);

Img_array column_image_blocking(Img_array lines);

int is_full_line(SDL_Surface *image, int line);

int is_full_column(SDL_Surface *image, int column);

void init_array(Img_array *a, size_t init_size);

void insert_array(Img_array *a, SDL_Surface *element);

void free_array(Img_array *a);

SDL_Surface *height_fix(SDL_Surface *img);

/*void char_init_array(Char_array *a, size_t init_size);

void char_insert_array(Char_array *a, Img_array element);

void char_free_array(Char_array *a);*/
 
# endif
