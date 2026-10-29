#include "skills/AttackHalfCircleSlash.h"
#include "world/World.h"
#include "layers/Layer.h"
#include "utils/Utils.h"
#include "entities/AABB.h"

void getRotatedHitBoxPoints(const AttackInfo &attackInfo, float elapsedTime, GEB::Vec2 hitBoxPoints[4])
{
    GEB::Vec2 originPoint1 = attackInfo.originPoint();

    const auto unitDirVec2 = GEB::Utils::transDirToUnitVec2(attackInfo.direction());

    auto startDirVec2 = unitDirVec2;
    startDirVec2.x = unitDirVec2.y;
    startDirVec2.y = -unitDirVec2.x;

    const auto hitBoxWidth = static_cast<float>(attackInfo.hitBoxWidth());
    const auto hitBoxHeight = static_cast<float>(attackInfo.hitBoxHeight());

    GEB::Vec2 originPoint2 = attackInfo.originPoint() + (startDirVec2 * hitBoxWidth);

    const auto perpVec2 = GEB::Math::normalize(GEB::Math::perpendicular(startDirVec2));

    // get hit box points
    hitBoxPoints[0] = originPoint1 + (perpVec2 * (hitBoxHeight / 2));
    hitBoxPoints[1] = originPoint1 - (perpVec2 * (hitBoxHeight / 2));
    hitBoxPoints[2] = originPoint2 + (perpVec2 * (hitBoxHeight / 2));
    hitBoxPoints[3] = originPoint2 - (perpVec2 * (hitBoxHeight / 2));

    // calcuate progress
    const float hitDuration = attackInfo.hitEndTime() - attackInfo.hitStartTime();
    float progress = 0;
    if (hitDuration > 0.0f)
    {
        progress = GEB::Math::min(1.0f, (elapsedTime - attackInfo.hitStartTime()) / hitDuration);
        progress = GEB::Math::max(0.0f, progress);
    }

    const float degree = 180.0f * progress;

    // apply rotation
    hitBoxPoints[0] = GEB::Utils::rotateClockwise(hitBoxPoints[0], attackInfo.originPoint(), degree);
    hitBoxPoints[1] = GEB::Utils::rotateClockwise(hitBoxPoints[1], attackInfo.originPoint(), degree);
    hitBoxPoints[2] = GEB::Utils::rotateClockwise(hitBoxPoints[2], attackInfo.originPoint(), degree);
    hitBoxPoints[3] = GEB::Utils::rotateClockwise(hitBoxPoints[3], attackInfo.originPoint(), degree);
}

AABB AttackHalfCircleSlash::getAttackContainer(World &world)
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

bool AttackHalfCircleSlash::tryHit(World &world, std::uintptr_t entityPtr)
{
    if (!Attack::tryHit(world, entityPtr))
        return false;

    // After [!Attack::tryHit], it means that the attack is active,
    // and hitTargets hasn't contained this id.
    const auto &attackInfo = this->attackInfo();

    if (elapsedTime_ < attackInfo.hitStartTime() || elapsedTime_ >= attackInfo.hitEndTime())
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

// todo: remove
void AttackHalfCircleSlash::render(World &world, Layer &layer)
{
    if (!isActive())
        return;

    const auto &info = attackInfo();
    if (elapsedTime_ < info.hitStartTime() || elapsedTime_ >= info.hitEndTime() ||
        info.hitBoxWidth() <= 0 || info.hitBoxHeight() <= 0)
        return;

    auto &canvas = world.canvas();
    const auto &viewport = world.gameInfo().viewport();
    const int visibleWidth = GEB::Math::min(viewport.width(), static_cast<int>(canvas.getWidth()));
    const int visibleHeight = GEB::Math::min(viewport.height(), static_cast<int>(canvas.getHeight()));

    GEB::Vec2 points[4];
    getRotatedHitBoxPoints(info, elapsedTime_, points);
    for (int i = 0; i < 4; ++i)
    {
        points[i].x -= viewport.x();
        points[i].y -= viewport.y();
    }

    const int edges[4][2] = {{0, 1}, {1, 3}, {3, 2}, {2, 0}};
    for (int i = 0; i < 4; ++i)
    {
        const auto &start = points[edges[i][0]];
        const auto &end = points[edges[i][1]];
        const float dx = end.x - start.x;
        const float dy = end.y - start.y;
        const int steps = GEB::Math::ceilToInt(
            GEB::Math::max(GEB::Math::abs(dx), GEB::Math::abs(dy)));
        if (steps <= 0)
            continue;

        for (int step = 0; step <= steps; ++step)
        {
            const float progress = static_cast<float>(step) / steps;
            const float x = start.x + dx * progress;
            const float y = start.y + dy * progress;
            if (x < 0.0f || y < 0.0f || x >= visibleWidth || y >= visibleHeight)
                continue;

            canvas.draw(static_cast<int>(x), static_cast<int>(y), 255, 220, 0);
        }
    }
}
