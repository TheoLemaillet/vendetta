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

#ifndef U_AI_H
#define U_AI_H

typedef struct ai      ai_t;
typedef struct ai_data ai_data_t;

#include "universe/transform.h"

struct ai
{
	char*       name;
	transform_t inventory;
	int         building;
};

struct ai_data
{
	int step;
	int collect; // need materials for item
	int sell;    // 0: ignore, other put item (id+1) for sell
};

#include "world/character.h"

void ai_init(ai_t* ai);
void ai_exit(ai_t* ai);

void ai_push_item(ai_t* ai, int id);

void ai_load(ai_t* ai, const char* filename);

char ai_get   (character_t* c, char is_item, int id, float amount, char keep);
char ai_getreq(character_t* c, transform_t* tr,      float amount, char keep);
char ai_build (character_t* c, int id);
char ai_do    (ai_t* ai, character_t* c);

#endif
