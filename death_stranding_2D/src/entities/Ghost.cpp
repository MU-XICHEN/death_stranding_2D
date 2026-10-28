#include "entities/Monster.h"
#include "world/World.h"
#include "layers/Layer.h"

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

void Ghost::render(World &world, Layer &layer)
{
    if (!visible_)
        return;

    Monster::render(world, layer);
}