#include <err.h>
#include <string.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"
#include "display.h"
#include "tools.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

char* Get_ImgPath(int argc , char ** argv)
{
    if (argc == 1)
        errx(1,"Need argument : path of image to segment.\n");
    if (argc > 2)
        errx(1,"To many argument : only need path of image to segment.\n");

    return argv[1];
}

SDL_Surface* Load_bmp(char* Path)
{
    SDL_Surface* img;

    img = SDL_LoadBMP(Path);
    if (img == NULL)
        errx(1,"ERROR : load BMP failed > %s",SDL_GetError());

    return img;
}

void Binarisation(SDL_Surface* image_surface)
{
    int width = image_surface->w;
    int height = image_surface->h;

 	// BINARISATION
    double aveR = 0, aveG = 0, aveB = 0;
    int black = 255;
    int white = 0;
    //determine average
	// METHODE OTSU
    //Otsu method that determine optimized rgb value
    int seuil = toBin(image_surface);
    if (seuil < 127)
    {
        black = 0;
        white = 255;
    }
    Uint8 r = 0, g = 0, b = 0;
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            int32 pixel = get_pixel(image_surface, i, j);
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
            if ((r + g + b) / 3 > seuil)
            {
                pixel = SDL_MapRGB(image_surface->format, black, black, black);
            }
            else
            {
                pixel = SDL_MapRGB(image_surface->format, white, white, white);
            }
            put_pixel(image_surface, i, j, pixel);
        }
    }
}


int main(int argc, char** argv)
{
    //_________________INIT VALUE________________
    SDL_Surface* img_surface;

    //________________GET IMG PATH_______________
    char* Path = Get_ImgPath(argc,argv);

    //__________________INIT SDL_________________
    Init_SDL();

    //__________________LOAD IMG_________________
    img_surface = Load_bmp(Path);

    if( img_surface == NULL)
        printf("ERROR : Failed to load img BMP");


    //______________Segmentation_________________
    //Segment_lines(img_surface);
    //Segment_char(img_surface);
    //Segment_hori_char(img_surface);


    //______________Save new image________________
    SDL_SaveBMP(img_surface,"/newImage.bmp");

    //_______________FREE SURFACE________________
    SDL_FreeSurface(img_surface);


    return 0;
}