#include "entities/Monster.h"
#include "world/World.h"
#include "layers/Layer.h"

// Ghost
void Ghost::move(World &world, Layer &layer)
{
    // Todo: ghosts have their own movement manner.
    if (!awakened_)
        return;

    float dt = world.gameInfo().dt();
    float distance = dt * speed_;

    auto dirVec = calculateDirVecToHero(center(), world.gameInfo().heroInfo().center());

    // todo: handle move
}

void Ghost::checkDangerDistance(World &world, Layer &layer)
{
    if (awakened_)
        return;

    GEB::Position heroCenter = world.gameInfo().heroInfo().center();
    float dx = GEB::Math::abs(heroCenter.x - center().x);
    float dy = GEB::Math::abs(heroCenter.y - center().y);

    if (dx <= dangerDistance_ && dy <= dangerDistance_)
    {
        appear();
        awaken();
    }
}

void Ghost::update(World &world, Layer &layer)
{
    checkDangerDistance(world, layer);
    move(world, layer);
}

void Ghost::render(World &world, Layer &layer)
{
    if (!visible_)
        return;

    Monster::render(world, layer);
}