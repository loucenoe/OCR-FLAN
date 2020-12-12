#ifndef TOOLS_H_
#define TOOLS_H_

#include <stdlib.h>
#include <SDL.h>

int isColored(Uint32 pixel, SDL_Surface* image, int r1 , int g1, int b1);
int toBin(SDL_Surface* image);
void Draw_HorizontalLine(SDL_Surface* img , int y0 , int y1 , int x, int R , int G , int B);
void Draw_VerticalLine(SDL_Surface* img , int x0 , int x1 , int y, int R , int G , int B);

#endif