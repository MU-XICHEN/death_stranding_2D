#pragma once

#include "entities/Entity.h"

class AABB : public Entity
{
public:
    using Entity::Entity;
    AABB(float x, float y, int width, int height)
        : Entity(x, y, width, height) {}

    static AABB createAABB(
        const Entity *item,
        GEB::Vec2 distanceVec2);

    static AABB createAABB(
        GEB::Vec2 ori,
        GEB::Vec2 des);

    static AABB createAABB(
        GEB::Vec2 center,
        float radius);

    void update(World &world, Layer &layer) override {}
    void render(World &world, Layer &layer) override {}

    bool intersects(const Entity &aabb) const;
    bool contains(const GEB::Vec2 &point) const;
};