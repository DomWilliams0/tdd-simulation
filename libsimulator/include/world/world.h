#ifndef WORLD_H
#define WORLD_H

#include <stddef.h>
#include <stdbool.h>

#include "world/world_forward.h"
#include "util/util.h"

#define HUMAN_MASS (10.0)

struct world_parameters
{
	coord width;
	coord height;
};

MOD_FWD_DECLARE(world)

void world_step(struct world *w);

// entities
world_body world_create_entity(struct world *w);
void world_get_position(world_body body, double pos[2]);
void world_set_position(world_body body, double pos[2]);

#endif
