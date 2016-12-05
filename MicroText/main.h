# ifndef MAIN_H
# define MAIN_H
 
# include <stdlib.h>
# include <SDL.h>
# include <gtk/gtk.h>

void save_characters(Img_array characters, char *file_name);

char* concat(const char *s1, const char *s2);

void char_to_matrix(SDL_Surface *chara, char *save);

int make_interface(int argc, char **argv);

GdkPixbuf *create_pixbuf(const gchar *filename);

void file_get(GtkWidget *w, gpointer user_data);

# endif
