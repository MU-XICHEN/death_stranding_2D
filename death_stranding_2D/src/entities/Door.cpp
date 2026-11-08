#include "entities/Door.h"
#include "entities/Hero.h"
#include "entities/AABB.h"
#include "renderer/ImageRenderer.h"

Door::Door(Hero &hero, float x, float y)
    : Entity(x, y, 128, 64), hero_(hero)
{
}

void Door::update(World &world, Layer &layer)
{
    if (!active() || !hero_.active() || hero_.state().health() <= 0)
        return;

    if (hero_.hasKey() && AABB(x_, y_, width_, height_).intersects(hero_))
        unlocked_ = true;
}

void Door::render(World &world, Layer &layer)
{
    if (!active())
        return;

    const auto &viewport = world.gameInfo().viewport();
    // Only the bottom of the door is used for contact.
    drawImage(world, AssetId::DoorSprite,
              static_cast<int>(x_ - viewport.x()), static_cast<int>(y_ + height_ - 192 - viewport.y()));
}
