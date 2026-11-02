#include "skills/AttackCollision.h"
#include "skills/AttackInfo.h"
#include "layers/MainLayer.h"

void AttackCollision::tryHit(World &world, Layer &layer)
{

    if (!preTryHit(world, layer))
        return;

    const auto &atkInfo = attackInfo();

    const auto point = atkInfo.originPoint();
    int width = static_cast<int>(atkInfo.hitBoxWidth());
    int height = static_cast<int>(atkInfo.hitBoxHeight());

    AABB aabb{
        point.x,
        point.y,
        width,
        height};

    auto &mainLayer = dynamic_cast<MainLayer &>(layer);
    auto &playStage = dynamic_cast<PlayStage &>(mainLayer.currentStage());
    auto &gridSystem = playStage.gridSystem();
    GEB::Structures::Array<Entity *> candidateItemList;
    gridSystem.getCandidateListByAABB(owner_, aabb, candidateItemList);

    
}