#include "entities/Monster.h"
#include "world/World.h"
#include "layers/Layer.h"

// Beast
void Beast::move(World &world, Layer &layer)
{
    if (!awakened_)
        return;

    float dt = world.gameInfo().dt();
    float distance = dt * speed_;

    // todo: polish the movement logic for monsters, with alldirections.
    // for now, just move in single directions.
    handleMovement(world, layer);
}

void Beast::update(World &world, Layer &layer)
{
    checkDangerDistance(world, layer);
    move(world, layer);
}