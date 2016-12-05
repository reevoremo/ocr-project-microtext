# include <SDL/SDL.h>
# include <SDL/SDL_image.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <err.h>
# include <unistd.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <gtk/gtk.h>

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
  if (nC < 45)
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

  make_interface(argc, argv);

  init_sdl();
  img = load_image(concat(argv[1], concat("/", argv[2])));
  change_Image(img);
  cha = image_blocking(img);
  save_characters(cha, argv[2]);
  display_image(img);
  
}

void file_get(GtkWidget *w, gpointer user_data)
{
  GtkWidget *file_selector = GTK_WIDGET(user_data);
  const gchar *selected_filename;

  w = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(w), "Bordel !");

  selected_filename = gtk_file_selection_get_filename(GTK_FILE_SELECTION(file_selector));
  g_print("Selected filename: %s\n", selected_filename);
}

int make_interface(int argc, char **argv)
{
  GtkWidget *window;
  GdkPixbuf *icon;
  GtkWidget *vbox;

  /*GtkWidget *menubar;
  GtkWidget *fileMenu;
  GtkWidget *fileMi;
  GtkWidget *quitMi;*/

  GtkWidget *filew;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Simple Menu");
  gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

  vbox = gtk_vbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(window), vbox);

  /*menubar = gtk_menu_bar_new();
  fileMenu = gtk_menu_new();

  fileMi = gtk_menu_item_new_with_label("File");
  quitMi = gtk_menu_item_new_with_label("Quit");*/

  filew = gtk_file_selection_new("File selection");

  g_signal_connect(filew, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  g_signal_connect(GTK_FILE_SELECTION(filew)->ok_button,
                    "clicked", G_CALLBACK(file_get), filew);

  g_signal_connect_swapped(GTK_FILE_SELECTION(filew)->ok_button,
                          "clicked", G_CALLBACK(gtk_widget_destroy), 
                          filew);

  g_signal_connect_swapped(GTK_FILE_SELECTION(filew)->cancel_button,
                            "clicked", G_CALLBACK(gtk_widget_destroy),
                            filew);

  gtk_file_selection_set_filename(GTK_FILE_SELECTION(filew),
                                  "test.png");

  /*gtk_menu_item_set_submenu(GTK_MENU_ITEM(fileMi), fileMenu);
  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), quitMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), fileMi);
  gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);*/

  icon = create_pixbuf("/home/pierre/Downloads/icon_test.jpeg");
  gtk_window_set_icon(GTK_WINDOW(window), icon);

  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

  //g_signal_connect(G_OBJECT(quitMi), "activate", G_CALLBACK(gtk_main_quit), NULL);

  gtk_widget_show(filew);

  gtk_widget_show_all(window);

  g_object_unref(icon);

  gtk_main();

  return 0;
}

GdkPixbuf *create_pixbuf(const gchar *filename)
{
  GdkPixbuf *pixbuf;
  GError *error = NULL;
  pixbuf = gdk_pixbuf_new_from_file(filename, &error);

  if (!pixbuf)
  {
    fprintf(stderr, "%s\n", error->message);
    g_error_free(error);
  }

  return pixbuf;
}

//This function save every single possible character of the image in a folder
void save_characters(Img_array characters, char *file_name)
{
  char dir[1048];

  if (getcwd(dir, sizeof(dir)) == NULL)
  {
  	return;
  }

  struct stat st;

  char *save_dir = concat(concat(dir, "/"), file_name);

  if (stat(save_dir, &st) == -1)
  {
    mkdir(save_dir, 0700);
  }

  for (size_t i = 0; i < characters.used; i++)
  {
    int index = i;
    char num[10];
    sprintf(num, "%d", index);
    char *saveFile = concat(concat(save_dir, "/"), num);
    SDL_SaveBMP(characters.array[i], saveFile);
    char_to_matrix(characters.array[i], saveFile);
    //save_matrix(matrix, saveFile);
    free(saveFile);
  }
  free(save_dir);
}

void char_to_matrix(SDL_Surface *chara, char *save)
{
  char *saveFile = concat(save, ".txt");

  FILE *f = fopen(saveFile, "w");
  
  for (int i = 0; i < chara->h; i++)
  {
    for (int j = 0; j < chara->w; j++)
    {
      Uint32 pixel = getpixel(chara, j, i);
      if (pixel == 0)
      {
        fprintf(f, "1");
      }
      else
      {
        fprintf(f, "0");
      }
    }
    fprintf(f, "\n");
  }
  fclose(f);
}

//This function concatenates two strings
char* concat(const char *s1, const char *s2)
{
  char *result = malloc(strlen(s1) + strlen(s2) + 1);
  strcpy(result, s1);
  strcat(result, s2);
  return result;
}
