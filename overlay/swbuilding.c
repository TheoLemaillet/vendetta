#include "swbuilding.h"

#include <stdlib.h>
#include <math.h>

#define PANEL_N_COLS 3

void swbuilding_draw(game_t* g)
{
	building_t* b = g->player->inBuilding;
	if (b == NULL)
		return;

	sfText* text = NULL;
	if (text == NULL)
	{
		sfColor color = {255, 255, 255, 255};

		text = sfText_create();
		sfText_setFont         (text, g->g->font);
		sfText_setCharacterSize(text, 18);
		sfText_setColor        (text, color);
	}

	sfVector2f pos = {PANEL_N_COLS * 28 + 10, 10};

	sfText_setPosition(text, pos);
	sfText_setUnicodeString(text, (sfUint32*) b->t->name);
	sfRenderWindow_drawText(g->g->render, text, NULL);

	kindOf_building_t* t = b->t;

	pos.y += 20;
	if (t->make_res.n != 0)
	{
		component_t* c = &t->make_res.c[0];
		if (c->is_item)
			exit(1);

		const wchar_t* action = t->make_req.n == 0 ? L"Harvest" : L"Transform to";
		const wchar_t* name   = g->u->materials[c->id].name;
		wchar_t buffer[1024];
		swprintf(buffer, 1024, L"%ls %ls", action, name);

		sfText_setPosition(text, pos);
		sfText_setUnicodeString(text, (sfUint32*) buffer);
		sfRenderWindow_drawText(g->g->render, text, NULL);
	}

	pos.y += 20;
	for (int i = 0; i < t->item_n; i++)
	{
		pos.y += 20;

		components_t* l = &t->item_res[i];
		if (l->n == 0)
			continue;

		component_t* c = &l->c[0];
		if (!c->is_item)
			exit(1);

		wchar_t* name = g->u->items[c->id].name;
		wchar_t buffer[1024];
		if (i == b->item_current)
			swprintf(buffer, 1024, L"%ls (%i%%)", name, (int) floor(100*b->item_progress));
		else
			swprintf(buffer, 1024, L"%ls", name);

		sfText_setPosition(text, pos);
		sfText_setUnicodeString(text, (sfUint32*) buffer);
		sfRenderWindow_drawText(g->g->render, text, NULL);
	}

//	sfText_destroy(text); // TODO
}
