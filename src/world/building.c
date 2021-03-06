/*\
 *  Role playing, management and strategy game
 *  Copyright (C) 2013-2014 Quentin SANTOS
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
\*/

#include "building.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../mem.h"

void building_init(building_t* b, world_t* w, kindOf_building_t* t, uuid_t owner, float x, float y)
{
	b->o.t = O_BUILDING;
	b->o.x = x;
	b->o.y = y;
	b->o.w = t->width;
	b->o.h = t->height;

	b->w = w;
	b->t = t;

	b->owner = owner;

	b->build_progress = 0;
	b->life = 0;

	b->work_n = 0;
	b->work_list = 0;
	b->work_progress = 0;

	universe_t* u = w->universe;
	inventory_init(&b->inventory, u->n_materials, u->n_items);
	b->open = 0;
}

void building_exit(building_t* b)
{
	if (b == NULL)
		return;

	inventory_exit(&b->inventory);
	free(b->work_list);
}

float building_build(building_t* b, float work)
{
	work = fmin(work, 1 - b->build_progress);
	b->build_progress += work;
	b->life = fmin(b->life + 20*work, 20);
	return work;
}

float building_attacked(building_t* b, float work)
{
	work = fmin(work, b->life);
	b->life -= work;
	if (b->life <= 0)
	{
		universe_t* u = b->w->universe;

		kindOf_event_t* e = &u->events[u->event_destroyBuilding];
		evtList_push(&b->w->events, e, b->o.x, b->o.y - b->o.h/2);
		world_delBuilding(b->w, b);
	}
	return work;
}

void building_work_enqueue(building_t* b, int c)
{
	int i = b->work_n++;
	b->work_list = CREALLOC(b->work_list, int, b->work_n);
	b->work_list[i] = c;
}

void building_work_dequeue(building_t* b, size_t n)
{
	if (n < b->work_n)
	{
		memmove(b->work_list + n, b->work_list + n + 1, sizeof(int) * (b->work_n - n -1));
		b->work_n--;
	}
}

void building_update(building_t* b)
{
	universe_t* u = b->w->universe;
	char open = 0;
	for (size_t i = 0; !open && i < u->n_materials; i++)
	{
		if (b->inventory.materials[i] >= 1)
			open = 1;
	}
	for (size_t i = 0; !open && i < u->n_items; i++)
	{
		if (b->inventory.items[i] >= 1)
			open = 1;
	}
	b->open = open;
}

void building_take(building_t* b, char is_item, int id, float amount, inventory_t* inv, char isOwner)
{
	universe_t* u = b->w->universe;

	amount = fmin(amount, inventory_get(&b->inventory, is_item, id));

	float price = 0;
	if (!isOwner)
	{
		price = is_item ? u->items[id].price : u->materials[id].price;
		price *= amount;
	}
	if (price > inv->money)
		return;

	inventory_pay(&b->inventory, price,                inv);
	inventory_mov(&b->inventory, is_item, id, -amount, inv);
	building_update(b);
}

void building_put(building_t* b, char is_item, int id, float amount, inventory_t* inv, char isOwner)
{
	if (!isOwner)
		return;

	inventory_mov(&b->inventory, is_item, id, amount, inv);
	building_update(b);
}

void building_withdraw(building_t* b, inventory_t* inv)
{
	inventory_pay(inv, b->inventory.money, &b->inventory);
}

float building_onSale(building_t* b, char is_item, int id)
{
	if (kindOf_building_canMake(b->t, is_item, id) == NULL)
		return 0;

	return inventory_get(&b->inventory, is_item, id);
}
