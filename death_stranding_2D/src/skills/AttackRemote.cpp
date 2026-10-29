#include "skills/AttackRemote.h"
#include "world/World.h"
#include "utils/Utils.h"
#include "stages/Play.h"
#include "layers/MainLayer.h"

void AttackRemote::tryHit(World &world, Layer &layer)
{
    if (!preTryHit(world, layer))
        return;

    auto &mainLayer = dynamic_cast<MainLayer &>(layer);
    auto &playStage = dynamic_cast<PlayStage &>(mainLayer.currentStage());
    auto &gridSystem = playStage.gridSystem();

    AABB attackContainer = getAttackContainer(world, layer);
    // pointers in list do not need to be owned, just references to existing entities, like 'this'.
    GEB::Structures::Array<Entity *> candidateItemList;
    gridSystem.getCollisionCandidateListByAABB(owner_, attackContainer, candidateItemList);

    if ((candidateItemList.size() == 0))
        return;

    const auto &atkInfo = attackInfo();
    float speed = atkInfo.rangedAttackSpeed();
    float distance = speed * elapsedTime_;
    GEB::Vec2 unitDirVec2 = GEB::Utils::transDirToUnitVec2(startDirection_);
    GEB::Vec2 startPos = lastPos_;
    GEB::Vec2 desPos = startPos + (unitDirVec2 * distance);
    lastPos_ = desPos;

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

    int targetIndex = 0;
    float minProjection = std::numeric_limits<float>::max();
    for (int i = 0; i < hitEntities.size(); ++i)
    {
        auto *entity = hitEntities[i];
        GEB::Vec2 entityHitBox[4];
        GEB::Utils::getRectByEntity(entity, entityHitBox);

        const auto projectionPair = GEB::Utils::projectRect(entityHitBox, unitDirVec2);
        float projection = projectionPair.first;
        if (projection < minProjection)
        {
            minProjection = projection;
            targetIndex = i;
        }
    }
    hitEntities[targetIndex]->OnAttacked(atkInfo);

    // remote attack ends after hitting the first target
    end();
}

AABB AttackRemote::getAttackContainer(World &world, Layer &layer) const
{
    const auto &attackInfo = this->attackInfo();
    float speed = attackInfo.rangedAttackSpeed();
    float distance = speed * elapsedTime_;
    GEB::Vec2 unitDirVec2 = GEB::Utils::transDirToUnitVec2(startDirection_);
    GEB::Vec2 desPos = lastPos_ + (unitDirVec2 * distance);
    return AABB::createAABB(lastPos_, desPos);
}

void AttackRemote::render(World &world, Layer &layer)
{
    // Implement any cleanup or state reset needed when the attack ends
}