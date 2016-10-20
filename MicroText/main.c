# include <SDL/SDL.h>
# include <SDL/SDL_image.h>
# include <stdlib.h>
# include <err.h>

# include "pixel_operations.h"


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

int main()
{
  SDL_Surface *img;

  init_sdl();
  img = load_image("test_images/abs1.jpg"); //Remo// Changed location to test
  change_Image(img);
  display_image(img); 
}
