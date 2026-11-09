#include "skills/AttackHalfCircleSlash.h"
#include "world/World.h"
#include "layers/Layer.h"
#include "utils/Utils.h"
#include "entities/AABB.h"
#include "layers/MainLayer.h"
#include "stages/Play.h"

void getRotatedHitBoxPoints(const AttackInfo &attackInfo,
                            float elapsedTime,
                            GEB::Vec2 points[4])
{
    GEB::Vec2 hitBoxPoints[4];
    GEB::Utils::getRectByPoint(attackInfo.originPoint(),
                               attackInfo.direction(),
                               attackInfo.hitBoxWidth(),
                               attackInfo.hitBoxHeight(),
                               hitBoxPoints);

    // Turn the hit time into progress for a half-circle swing.
    const float hitDuration = attackInfo.hitEndTime() - attackInfo.hitStartTime();
    float progress = 0;
    if (hitDuration > 0.0f)
    {
        progress = std::min(1.0f, (elapsedTime - attackInfo.hitStartTime()) / hitDuration);
        progress = std::max(0.0f, progress);
    }

    const float degree = 180.0f * progress;

    // apply rotation
    hitBoxPoints[0] = GEB::Utils::rotateClockwise(hitBoxPoints[0], attackInfo.originPoint(), degree);
    hitBoxPoints[1] = GEB::Utils::rotateClockwise(hitBoxPoints[1], attackInfo.originPoint(), degree);
    hitBoxPoints[2] = GEB::Utils::rotateClockwise(hitBoxPoints[2], attackInfo.originPoint(), degree);
    hitBoxPoints[3] = GEB::Utils::rotateClockwise(hitBoxPoints[3], attackInfo.originPoint(), degree);
    memset(points, 0, sizeof(GEB::Vec2) * 4);
    memcpy(points, hitBoxPoints, sizeof(GEB::Vec2) * 4);
}

AABB AttackHalfCircleSlash::getAttackContainer(World &world, Layer &layer) const
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

void AttackHalfCircleSlash::tryHit(World &world, Layer &layer)
{
    auto &mainLayer = dynamic_cast<MainLayer &>(layer);
    auto &playStage = dynamic_cast<PlayStage &>(mainLayer.currentStage());
    auto *gridSystem = playStage.gridSystem();
    if (!gridSystem)
        return;

    if (!preTryHit(world, layer))
        return;

    AABB attackContainer = getAttackContainer(world, layer);
    // Find nearby entities using a box around the swing.
    GEB::Structures::Array<Entity *> candidateItemList;
    gridSystem->getCandidateListByAABB(owner_, attackContainer, candidateItemList);

    if ((candidateItemList.size() == 0))
        return;

    // try hit
    for (int j = 0; j < candidateItemList.size(); ++j)
    {
        if (!candidateItemList[j])
            continue;

        auto *entity = dynamic_cast<ItemEntity *>(candidateItemList[j]);
        if (!entity)
            continue;

        std::uintptr_t entityPtr = reinterpret_cast<std::uintptr_t>(entity);
        if (hitTargets_.contains(entityPtr))
            continue;

        const auto &atkInfo = attackInfo();
        GEB::Vec2 points[4];
        getRotatedHitBoxPoints(atkInfo, elapsedTime_, points);

        // Check the rotated sword box against the target body.
        GEB::Vec2 rectA[4] = {points[0], points[1], points[2], points[3]};
        GEB::Vec2 rectB[4] = {
            GEB::Vec2{entity->x(), entity->y()},
            GEB::Vec2{entity->x(), entity->y()} + GEB::Vec2(static_cast<float>(entity->width()), 0),
            GEB::Vec2{entity->x(), entity->y()} + GEB::Vec2(0, static_cast<float>(entity->height())),
            GEB::Vec2{entity->x(), entity->y()} + GEB::Vec2(static_cast<float>(entity->width()), static_cast<float>(entity->height())),
        };

        GEB::Utils::adaptForSAT(rectA);
        GEB::Utils::adaptForSAT(rectB);

        if (GEB::Utils::SAT(rectA, rectB))
        {
            hitTargets_.insert(entityPtr);
            hitTarget(*entity, atkInfo);
        }
    }
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
    const int visibleWidth = std::min(viewport.width(), static_cast<int>(canvas.getWidth()));
    const int visibleHeight = std::min(viewport.height(), static_cast<int>(canvas.getHeight()));

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
            std::max(GEB::Math::abs(dx), GEB::Math::abs(dy)));
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
