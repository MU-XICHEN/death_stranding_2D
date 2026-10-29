#include "entities/AABB.h"
#include "entities/Entity.h"
#include "core/GameBase.h"

bool AABB::intersects(const AABB &aabb) const
{
    return !(x_ + width_ < aabb.x() || x_ > aabb.x() + aabb.width() ||
             y_ + height_ < aabb.y() || y_ > aabb.y() + aabb.height());
}

AABB AABB::createAABB(
    const Entity *item,
    GEB::Vec2 offset)
{
    if (!item)
        return AABB(0, 0, 0, 0);

    auto targetX = item->x() + offset.x;
    auto targetY = item->y() + offset.y;

    auto startX = GEB::Math::min(item->x(), targetX);
    auto startY = GEB::Math::min(item->y(), targetY);

    auto endX = GEB::Math::max(item->x() + item->width(), targetX + item->width());
    auto endY = GEB::Math::max(item->y() + item->height(), targetY + item->height());

    // [bugfix]: use ceilToInt to ensure the AABB fully covers the movement path.
    return AABB(startX, startY,
                GEB::Math::ceilToInt(endX - startX),
                GEB::Math::ceilToInt(endY - startY));
}

AABB AABB::createAABB(GEB::Vec2 ori,
                      GEB::Vec2 des)

{
    auto startX = GEB::Math::min(ori.x, des.x);
    auto startY = GEB::Math::min(ori.y, des.y);

    auto endX = GEB::Math::max(ori.x, des.x);
    auto endY = GEB::Math::max(ori.y, des.y);

    return AABB(startX, startY,
                GEB::Math::ceilToInt(endX - startX),
                GEB::Math::ceilToInt(endY - startY));
}
