#ifndef TOOLS_H_
#define TOOLS_H_

#include <stdlib.h>
#include <SDL.h>

int isColored(Uint32 pixel, SDL_Surface* image, int r1 , int g1, int b1);
int toBin(SDL_Surface* image);

#endif