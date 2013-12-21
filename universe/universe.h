#ifndef U_UNIVERSE_H
#define U_UNIVERSE_H

#include "material.h"
#include "mine.h"

typedef struct universe universe_t;

struct universe
{
	kindOf_material_t materials[1];
	kindOf_mine_t     mines[1];
};

void universe_init(universe_t* u);

#endif
