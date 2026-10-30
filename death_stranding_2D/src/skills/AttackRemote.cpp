#include "skills/AttackRemote.h"
#include "world/World.h"
#include "utils/Utils.h"
#include "stages/Play.h"
#include "layers/MainLayer.h"

void AttackRemote::tryHit(World &world, Layer &layer)
{
    if (!preTryHit(world, layer))
        return;

    // out of viewport
    auto viewport = world.gameInfo().viewport();
    if (lastPos_.x < viewport.x() || lastPos_.x > viewport.x() + viewport.width() ||
        lastPos_.y < viewport.y() || lastPos_.y > viewport.y() + viewport.height())
    {
        end();
        return;
    }

    auto &mainLayer = dynamic_cast<MainLayer &>(layer);
    auto &playStage = dynamic_cast<PlayStage &>(mainLayer.currentStage());
    auto &gridSystem = playStage.gridSystem();

    AABB attackContainer = getAttackContainer(world, layer);
    // pointers in list do not need to be owned, just references to existing entities, like 'this'.
    GEB::Structures::Array<Entity *> candidateItemList;
    gridSystem.getCollisionCandidateListByAABB(owner_, attackContainer, candidateItemList);

    const auto &atkInfo = attackInfo();
    float speed = atkInfo.rangedAttackSpeed();
    float dt = world.gameInfo().dt();
    float distance = speed * dt;
    GEB::Vec2 startPos = lastPos_;
    GEB::Direction startDir = startDirection_;
    GEB::Vec2 desPos = GEB::Utils::getDestination(startPos, startDir, distance);
    lastPos_ = desPos;

    if ((candidateItemList.size() == 0))
        return;

    // try hit
    GEB::Structures::Array<ItemEntity *> hitEntities;
    for (int j = 0; j < candidateItemList.size(); ++j)
    {
        auto *entity = dynamic_cast<ItemEntity *>(candidateItemList[j]);
        if (!entity)
            continue;

        GEB::Vec2 entityHitBox[4];
        GEB::Utils::getRectByEntity(entity, entityHitBox);

        if (GEB::Utils::SATLineBox(startPos, desPos, entityHitBox))
        {
            hitEntities.pushBack(entity);
        }
    }

    if (hitEntities.size() == 0)
        return;

    int targetIndex = -1;
    float squaredDist = std::numeric_limits<float>::max();
    for (int i = 0; i < hitEntities.size(); ++i)
    {
        const auto *entity = hitEntities[i];
        AABB aabb{
            entity->x(),
            entity->y(),
            entity->width(),
            entity->height()};

        GEB::Vec2 ips[2];
        int count = GEB::Utils::getIntersectionPoints(startPos, desPos - startPos, aabb, ips);
        if (count > 0)
        {
            /**
             * [bugfix]: consider two points
             *  It could fail if startPos was already inside the AABB, while only checking one point.
             *  So, select the nearest intersection point to the start position.
             */
            auto vector1 = (ips[0] - startPos);
            float newSquaredDist = vector1.x * vector1.x + vector1.y * vector1.y;

            if (count == 2)
            {
                auto vector2 = (ips[1] - startPos);
                float newSquaredDist2 = vector2.x * vector2.x + vector2.y * vector2.y;
                newSquaredDist = std::min(newSquaredDist, newSquaredDist2);
            }

            if (newSquaredDist < squaredDist)
            {
                targetIndex = i;
                squaredDist = newSquaredDist;
            }
        }
    }
    if (targetIndex != -1)
    {
        hitEntities[targetIndex]->OnAttacked(atkInfo);
        // remote attack ends after hitting the first target
        end();
    }
}

AABB AttackRemote::getAttackContainer(World &world, Layer &layer) const
{
    const auto &attackInfo = this->attackInfo();
    float speed = attackInfo.rangedAttackSpeed();
    float dt = world.gameInfo().dt();
    float distance = speed * dt;
    GEB::Vec2 desPos = GEB::Utils::getDestination(lastPos_, startDirection_, distance);
    return AABB::createAABB(lastPos_, desPos);
}

void AttackRemote::render(World &world, Layer &layer)
{
    // Implement any cleanup or state reset needed when the attack ends
}
