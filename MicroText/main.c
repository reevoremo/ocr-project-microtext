# include <SDL/SDL.h>
# include <SDL/SDL_image.h>
# include <stdlib.h>
# include <err.h>
# include <unistd.h>
# include <sys/stat.h>
# include <sys/types.h>

# include "pixel_operations.h"

# include "image_blocking.h"

# include "main.h"

void wait_for_keypressed(void) {
  SDL_Event             event;
  // Infinite loop, waiting for event
  for (;;) {
    // Take an event
    SDL_PollEvent( &event );
    // Switch on event type
    switch (event.type) {
    // Someone pressed a key -> leave the function
    case SDL_KEYDOWN: return;
    default: break;
    }
  // Loop until we got the expected event
  }
}

void init_sdl(void) {
  // Init only the video part
  if( SDL_Init(SDL_INIT_VIDEO)==-1 ) {
    // If it fails, die with an error message
    errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
  }
  // We don't really need a function for that ...
}

SDL_Surface* load_image(char *path) {
  SDL_Surface          *img;
  // Load an image using SDL_image with format detection
  img = IMG_Load(path);
  if (!img)
    // If it fails, die with an error message
    errx(3, "can't load %s: %s", path, IMG_GetError());
  return img;
}

SDL_Surface* display_image(SDL_Surface *img) {
  SDL_Surface          *screen;
  // Set the window to the same size as the image
  screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
  if ( screen == NULL ) {
    // error management
    errx(1, "Couldn't set %dx%d video mode: %s\n",
         img->w, img->h, SDL_GetError());
  }
 
  SDL_UnlockSurface(img);
  /* Blit onto the screen surface */
  if(SDL_BlitSurface(img, NULL, screen, NULL) < 0)
    warnx("BlitSurface error: %s\n", SDL_GetError());
 
  // Update the screen
  SDL_UpdateRect(screen, 0, 0, img->w, img->h);
 
  // wait for a key
  wait_for_keypressed();
 
  // return the screen for further uses
  return screen;
}

void change_pixel(SDL_Surface *img, int x, int y){
  Uint8 r, g, b = 0;
  double nC;
  Uint32 pixel = getpixel(img, x, y);
  SDL_GetRGB(pixel, img->format, &r, &g, &b);
  nC = (0.3*r + 0.59*g + 0.11*b)/3;
  if (nC < 40)
    nC = 0;
  else
    nC = 255;
  r = nC;
  g = nC;
  b = nC;
  pixel = SDL_MapRGB(img->format, r, g, b);
  putpixel(img, x, y, pixel);
}

void change_Image(SDL_Surface *img){
  for (int i = 0; i < img->w; i++)
  {
    for (int j = 0; j < img->h; j++)
    {
      change_pixel(img, i, j);
    }
  }
}

int main(int argc, char **argv)
{
  argc = 3;
  SDL_Surface *img;
  //Img_array line;
  Img_array cha;

  init_sdl();
  img = load_image(concat(argv[1], concat("/", argv[2])));
  change_Image(img);
  cha = image_blocking(img);
  save_characters(cha, argv[2]);
  display_image(img);
  
}

//This function save every single possible character of the image in a folder
void save_characters(Img_array characters, char *file_name)
{
  char dir[1024];
  getcwd(dir, sizeof(dir));

  struct stat st;
  if (stat(concat(concat(dir, "/"), file_name), &st) == -1)
  {
    mkdir(concat(concat(dir, "/"), file_name), 0700);
  }

  for (size_t i = 0; i < characters.used; i++)
  {
    int index = i;
    char num[3];
    sprintf(num, "%d", index);
    SDL_SaveBMP(characters.array[i], concat(concat(concat(concat(dir, "/"), file_name), "/"), num));
  }
}

//This function concatenates two strings
char* concat(const char *s1, const char *s2)
{
  char *result = malloc(strlen(s1) + strlen(s2) + 1);
  strcpy(result, s1);
  strcat(result, s2);
  return result;
}