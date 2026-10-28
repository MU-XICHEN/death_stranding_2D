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
    GEB::Vec2 offset,
    const GEB::Boundary &boundary)
{
    if (!item)
        return AABB(0, 0, 0, 0);

    auto targetX = item->x() + offset.x;
    auto targetY = item->y() + offset.y;

    auto startX = GEB::Math::max(boundary.left, GEB::Math::min(item->x(), targetX));
    auto startY = GEB::Math::max(boundary.top, GEB::Math::min(item->y(), targetY));

    auto endX = GEB::Math::min(boundary.right, GEB::Math::max(item->x() + item->width(), targetX + item->width()));
    auto endY = GEB::Math::min(boundary.bottom, GEB::Math::max(item->y() + item->height(), targetY + item->height()));

    // [bugfix]: use ceilToInt to ensure the AABB fully covers the movement path.
    return AABB(startX, startY,
                GEB::Math::ceilToInt(endX - startX),
                GEB::Math::ceilToInt(endY - startY));
}
