#include "skills/HalfCircleSlash.h"
#include "world/World.h"
#include "utils/Utils.h"
#include "entities/AABB.h"

void getRotatedHitBoxPoints(const AttackInfo &attackInfo, float elapsedTime, GEB::Vec2 hitBoxPoints[4])
{
    auto degree = attackInfo.angleSpeed * elapsedTime;

    const auto unitDirVec2 = GEB::Utils::transDirToUnitVec2(attackInfo.direction);

    const auto hitBoxWidth = attackInfo.hitBoxWidth;
    const auto hitBoxHeight = attackInfo.hitBoxHeight;

    GEB::Vec2 originPoint1 = attackInfo.originPoint;
    GEB::Vec2 originPoint2 = attackInfo.originPoint + (unitDirVec2 * hitBoxHeight);

    const auto perpVec2 = GEB::Math::normalize(GEB::Math::perpendicular(unitDirVec2));

    // get hit box points
    hitBoxPoints[0] = originPoint1 + (perpVec2 * (hitBoxWidth / 2));
    hitBoxPoints[1] = originPoint1 - (perpVec2 * (hitBoxWidth / 2));
    hitBoxPoints[2] = originPoint2 + (perpVec2 * (hitBoxWidth / 2));
    hitBoxPoints[3] = originPoint2 - (perpVec2 * (hitBoxWidth / 2));

    // apply rotation
    hitBoxPoints[0] = GEB::Utils::rotateClockwise(hitBoxPoints[0], attackInfo.originPoint, degree);
    hitBoxPoints[1] = GEB::Utils::rotateClockwise(hitBoxPoints[1], attackInfo.originPoint, degree);
    hitBoxPoints[2] = GEB::Utils::rotateClockwise(hitBoxPoints[2], attackInfo.originPoint, degree);
    hitBoxPoints[3] = GEB::Utils::rotateClockwise(hitBoxPoints[3], attackInfo.originPoint, degree);
}

AABB HalfCircleSlash::getAttackContainer(World &world)
{
    auto &attackInfo = this->attackInfo();

    GEB::Vec2 hitBoxPoints[4];
    getRotatedHitBoxPoints(attackInfo, elapsedTime_, hitBoxPoints);

    float minX = hitBoxPoints[0].x;
    float maxX = hitBoxPoints[0].x;
    float minY = hitBoxPoints[0].y;
    float maxY = hitBoxPoints[0].y;

    for (int i = 1; i < 4; ++i)
    {
        if (hitBoxPoints[i].x < minX)
            minX = hitBoxPoints[i].x;
        if (hitBoxPoints[i].x > maxX)
            maxX = hitBoxPoints[i].x;
        if (hitBoxPoints[i].y < minY)
            minY = hitBoxPoints[i].y;
        if (hitBoxPoints[i].y > maxY)
            maxY = hitBoxPoints[i].y;
    }

    return AABB(minX, minY, static_cast<int>(std::ceil(maxX - minX)), static_cast<int>(std::ceil(maxY - minY)));
}

bool HalfCircleSlash::tryHit(World &world, const ItemEntity &item, std::uintptr_t entityPtr)
{
    if (!Attack::tryHit(world, item, entityPtr))
        return false;

    // After [!Attack::tryHit], it means that the attack is active,
    // and hitTargets hasn't contained this id.
    const auto &attackInfo = this->attackInfo();

    // todo: In the future, maybe support the remote attach here.
    if (elapsedTime_ >= attackInfo.duration)
    {
        end();
        return false;
    }

    if (elapsedTime_ < attackInfo.hitStartTime || elapsedTime_ >= attackInfo.hitEndTime)
        return false;

    GEB::Vec2 points[4];
    getRotatedHitBoxPoints(attackInfo, elapsedTime_, points);

    // SAT
    const Entity *entity = reinterpret_cast<const Entity *>(entityPtr);
    GEB::Vec2 rectA[4] = {points[0], points[1], points[2], points[3]};
    GEB::Vec2 rectB[4] = {
        GEB::Vec2{entity->x(), entity->y()},
        GEB::Vec2{entity->x(), entity->y()} + GEB::Vec2(static_cast<float>(entity->width()), 0),
        GEB::Vec2{entity->x(), entity->y()} + GEB::Vec2(0, static_cast<float>(entity->height())),
        GEB::Vec2{entity->x(), entity->y()} + GEB::Vec2(static_cast<float>(entity->width()), static_cast<float>(entity->height())),
    };

    if (GEB::Utils::SAT(rectA, rectB))
    {
        hitTargets_.insert(entityPtr);
        return true;
    }

    return false;
}

void HalfCircleSlash::render(World &world)
{
}