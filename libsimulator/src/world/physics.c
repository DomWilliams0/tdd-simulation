#include "world/internal/physics.h"
#include "world/internal/world.h"
#include "world/world.h"

#include "util/bool.h"

static BOOL ode_init;

void create_physics_world(struct world *world)
{
    if (!ode_init)
    {
	ode_init = TRUE;
	dInitODE();
    }

    dWorldID w = dWorldCreate();
    dWorldSetGravity(w, 0, 0, 0);
    dWorldSetQuickStepNumIterations(w, 5);

    dVector3 centre = { 0, 0, 0, 0 };
    dVector3 extents = { 5, 5, 5, 0 };
    dSpaceID s = dQuadTreeSpaceCreate(0, centre, extents, 4);

    world->phys_id = w;
    world->collision_space = s;
    world->contacts = dJointGroupCreate(0);
}

void destroy_physics_world(struct world *world)
{
    if (world->phys_id == NULL)
	return;

    dWorldDestroy(world->phys_id);
    dSpaceDestroy(world->collision_space);
    dJointGroupDestroy(world->contacts);
}


void near_callback(void *data, dGeomID o1, dGeomID o2)
{
    dBodyID b1 = dGeomGetBody(o1);
    dBodyID b2 = dGeomGetBody(o2);
    if (b1 && b2 && dAreConnected(b1, b2))
	return;


    struct world *w = (struct world *)data;

    // TODO this absolutely needs tweaking, the current values are meaningless
    dContact contact;
    contact.surface.mode = dContactBounce;
    contact.surface.bounce = 10;
    contact.surface.bounce_vel = 10;
    contact.surface.mu = 0;

    if (dCollide(o1, o2, 1, &contact.geom, sizeof(dContactGeom)))
    {
	dJointID c = dJointCreateContact(w->phys_id, w->contacts, &contact);
	dJointAttach(c, b1, b2);
    }
}