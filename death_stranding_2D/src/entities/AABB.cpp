#include "entities/AABB.h"
#include "entities/Entity.h"
#include "core/GameBase.h"

GEB::Vec2 getOffset(GEB::Direction dir, float distance)
{
    switch (dir)
    {
    case GEB::Direction::Up:
        return GEB::Vec2{0, -distance};
    case GEB::Direction::Down:
        return GEB::Vec2{0, distance};
    case GEB::Direction::Left:
        return GEB::Vec2{-distance, 0};
    case GEB::Direction::Right:
        return GEB::Vec2{distance, 0};
    case GEB::Direction::UpLeft:
        return GEB::Vec2{-distance, -distance};
    case GEB::Direction::UpRight:
        return GEB::Vec2{distance, -distance};
    case GEB::Direction::DownLeft:
        return GEB::Vec2{-distance, distance};
    case GEB::Direction::DownRight:
        return GEB::Vec2{distance, distance};
    default:
        return GEB::Vec2{0, 0};
    }
}

AABB AABB::createAABB(
    const Entity &item,
    GEB::Direction dir,
    float distance,
    const GEB::Boundary &boundary)
{
    auto offset = getOffset(dir, distance);
    auto targetX = item.x() + offset.x;
    auto targetY = item.y() + offset.y;

    auto startX = GEB::Math::min(item.x(), targetX);
    auto startY = GEB::Math::min(item.y(), targetY);
    auto endX = GEB::Math::max(item.x() + item.width(), targetX + item.width());
    auto endY = GEB::Math::max(item.y() + item.height(), targetY + item.height());

    // [bugfix]: use ceilToInt to ensure the AABB fully covers the movement path.
    return AABB(startX, startY,
                GEB::Math::ceilToInt(endX - startX),
                GEB::Math::ceilToInt(endY - startY));
}
