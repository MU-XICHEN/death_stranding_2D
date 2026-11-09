#include "entities/Potion.h"
#include "entities/Hero.h"
#include "entities/AABB.h"
#include "renderer/ImageRenderer.h"

Potion::Potion(Hero &hero, float x, float y, int width, int height)
    : ItemEntity(x, y, width, height), hero_(hero)
{
}

void Potion::update(World &world, Layer &layer)
{
    if (!active() || !hero_.active() || hero_.state().health() <= 0)
        return;

    // Use the potion once when the hero touches it.
    if (AABB(x_, y_, width_, height_).intersects(hero_))
    {
        hero_.onUsePotion(this);
        setActive(false);
    }
}

void Potion::render(World &world, Layer &layer)
{
    if (!active())
        return;

    const auto &viewport = world.gameInfo().viewport();
    drawImage(world, AssetId::PotionSprite,
              static_cast<int>(x_ - viewport.x()), static_cast<int>(y_ - viewport.y()));
}
