# ifndef MAIN_H
# define MAIN_H
 
# include <stdlib.h>
# include <SDL.h>

void save_characters(Img_array characters, char *file_name);

char* concat(const char *s1, const char *s2);

void char_to_matrix(SDL_Surface *chara, char *save);

# endif
