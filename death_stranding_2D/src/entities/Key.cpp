#include "entities/Key.h"
#include "entities/Hero.h"
#include "entities/AABB.h"
#include "map/Map.h"
#include "renderer/ImageRenderer.h"

Key::Key(Hero &hero, const Map &map)
    : Entity(map.center().x - 32.0f, map.center().y - 32.0f, 64, 64), hero_(hero)
{
}

void Key::update(World &world, Layer &layer)
{
    if (collected_ || !hero_.active() || hero_.state().health() <= 0)
        return;

    if (AABB(x_, y_, width_, height_).intersects(hero_))
        onCollected();
}

void Key::onCollected()
{
    // Hide the key and let the hero remember the pickup.
    collected_ = true;
    hero_.collectKey();
}

void Key::render(World &world, Layer &layer)
{
    if (collected_)
        return;

    const auto &viewport = world.gameInfo().viewport();
    drawImage(world, AssetId::KeySprite,
              static_cast<int>(x_ - viewport.x()), static_cast<int>(y_ - viewport.y()));
}
