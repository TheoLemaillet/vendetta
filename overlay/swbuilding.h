#ifndef O_SWBUILDING_H
#define O_SWBUILDING_H

typedef struct swbuilding swbuilding_t;

#include "subwindow.h"

struct swbuilding
{
	subwindow_t w;
};

#include "../game.h"

void swbuilding_init(swbuilding_t* w, graphics_t* g);
void swbuilding_exit(swbuilding_t* w);

char swbuilding_cursor(swbuilding_t* w, game_t* g, int x, int y);
void swbuilding_draw  (swbuilding_t* w, game_t* g);
char swbuilding_catch (swbuilding_t* w, game_t* g, int x, int y, int t);

#endif
