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

void Binarisation(int ac, char** nom)
{
	if (ac != 3)
        errx(1,"wrong arg number, expected : 2\n format : <image_jpg> <bin_methode>");
    char *str = nom[ac - 2];
    
    //load image
    SDL_Surface* image_surface = load_image(str);

    int width = image_surface->w;
    int height = image_surface->h;

 	// BINARISATION
    double aveR = 0, aveG = 0, aveB = 0;
    int black = 255;
    int white = 0;
    //determine average
    if (strcmp(nom[ac - 1],"0"))
    {
        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                Uint32 pixel = get_pixel(image_surface, i, j);
                Uint8 r, g, b;
                SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
                aveR += r;
                aveG += g;
                aveB += b;
            }
        }
        aveR = aveR / (width * height);
        aveG = aveG / (width * height);
        aveB = aveB / (width * height);
	// METHODE SIMPLE
    //use 127 as average rgb value
        if ((aveR+aveG+aveB)/3 < 127)
        {
            black = 0;
            white = 255;
        }
        for(int i = 0; i < width; i++)
        {
            for(int j = 0; j < height; j++)
            {
                Uint32 pixel = get_pixel(image_surface, i, j);
                Uint8 r, g, b;
                SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
                if ((r+g+b+aveR+aveG+aveB)/6 > 127)
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
    else
    {
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
                Uint32 pixel = get_pixel(image_surface, i, j);
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
    
}

int main(int argc, char **argv)
{
    SDL_Surface *image_surface;

    init_sdl();
    //test if arg error
    Binarisation(argc,argv);
    //ton code


    SDL_FreeSurface(image_surface);
    return 0;
}
