#include "entities/AABB.h"
#include "entities/Entity.h"
#include "core/GameBase.h"

bool AABB::intersects(const Entity &aabb) const
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

    // Cover the whole movement path, from the old position to the new one.
    auto targetX = item->x() + offset.x;
    auto targetY = item->y() + offset.y;

    auto startX = std::min(item->x(), targetX);
    auto startY = std::min(item->y(), targetY);

    auto endX = std::max(item->x() + item->width(), targetX + item->width());
    auto endY = std::max(item->y() + item->height(), targetY + item->height());

    // Round up so the box does not miss part of the path.
    return AABB(startX, startY,
                GEB::Math::ceilToInt(endX - startX),
                GEB::Math::ceilToInt(endY - startY));
}

AABB AABB::createAABB(GEB::Vec2 ori,
                      GEB::Vec2 des)

{
    auto startX = std::min(ori.x, des.x);
    auto startY = std::min(ori.y, des.y);

    auto endX = std::max(ori.x, des.x);
    auto endY = std::max(ori.y, des.y);

    return AABB(startX, startY,
                GEB::Math::ceilToInt(endX - startX),
                GEB::Math::ceilToInt(endY - startY));
}

AABB AABB::createAABB(GEB::Vec2 center,
                      float radius)
{
    auto aabbX = center.x - radius;
    auto aabbY = center.y - radius;

    return AABB(
        aabbX,
        aabbY,
        static_cast<int>(2 * radius),
        static_cast<int>(2 * radius));
}

bool AABB::contains(const GEB::Vec2 &point) const
{
    return point.x >= x_ && point.x <= x_ + width_ &&
           point.y >= y_ && point.y <= y_ + height_;
}
