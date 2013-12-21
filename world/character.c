#include "character.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "mine.h"

#define M_PI 3.14159265358979323846

void character_init(character_t* c, universe_t* u)
{
	c->o.t = O_CHARACTER;
	c->o.x = 0;
	c->o.y = 0;
	c->o.w = 24;
	c->o.h = 32;
	c->go_x = 0;
	c->go_y = 0;
	c->go_o = NULL;

	c->materials = calloc(sizeof(float), u->n_materials);
	if (c->materials == NULL)
	{
		fprintf(stderr, "Memory allocation error\n");
		exit(1);
	}
}

void character_deinit(character_t* c)
{
	free(c->materials);
}

void character_doRound(character_t* c, float duration)
{
	float dx;
	float dy;
	if (c->go_o != NULL)
	{
		dx = c->go_o->x;
		dy = c->go_o->y;
	}
	else
	{
		dx = c->go_x;
		dy = c->go_y;
	}
	dx -= c->o.x;
	dy -= c->o.y;
	float remDistance = sqrt(dx*dx + dy*dy);
	if (remDistance == 0)
	{
		if (c->go_o == NULL)
			return;

		if (c->go_o->t != O_MINE)
			return;

		mine_t* m = (mine_t*) c->go_o;
		int mat_id = m->t->material_id;
		c->materials[mat_id] += 1 * duration;
		printf("I now have %f of '%s'\n", c->materials[mat_id], m->t->material->name);
		return;
	}

	float dir;
	if (dx > 0)
	{
		dir = atan(dy / dx);
		if (dy < 0)
			dir += 2 * M_PI;
	}
	else if (dx < 0)
	{
		dir = atan(dy / dx) + M_PI;
	}
	else // dx == 0
	{
		dir = M_PI / 2;
		if (dy < 0)
			dir += M_PI;
	}

	float distance = 100 * duration;
	if (distance > remDistance)
		distance = remDistance;

	c->o.x += distance * cos(dir);
	c->o.y += distance * sin(dir);
}
