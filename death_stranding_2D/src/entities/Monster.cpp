#include "entities/Monster.h"
#include "entities/Hero.h"
#include "core/GameBase.h"
#include "world/World.h"
#include "world/Grid.h"
#include "stages/Play.h"
#include "layers/MainLayer.h"

template <GEB::Direction dir>
void calculateDotAndDir(GEB::Direction &bestDir, float &dot, float newDot)
{
    if (newDot > dot)
    {
        dot = newDot;
        bestDir = dir;
    }
}

bool Monster::canAttackTarget(const ItemEntity *target) const
{
    return dynamic_cast<const Hero *>(target) != nullptr;
}

GEB::Direction Monster::transDirVecToDiretion(const GEB::Vec2 &dirVec, GEB::Direction prevDirection)
{
    if (dirVec.x == 0 && dirVec.y == 0)
        return prevDirection;

    // Choose the closest of the eight facing directions.
    auto normalizedDirVec = GEB::Math::normalize(dirVec);

    float maxDot = 0;
    float tempDot = 0;

    GEB::Direction bestDirection = GEB::Direction::None;

    // left
    tempDot = GEB::Math::dotProduct(normalizedDirVec, GEB::Vec2{-1, 0});
    calculateDotAndDir<GEB::Direction::Left>(bestDirection, maxDot, tempDot);
    // right
    tempDot = GEB::Math::dotProduct(normalizedDirVec, GEB::Vec2{1, 0});
    calculateDotAndDir<GEB::Direction::Right>(bestDirection, maxDot, tempDot);
    // up
    tempDot = GEB::Math::dotProduct(normalizedDirVec, GEB::Vec2{0, -1});
    calculateDotAndDir<GEB::Direction::Up>(bestDirection, maxDot, tempDot);
    // down
    tempDot = GEB::Math::dotProduct(normalizedDirVec, GEB::Vec2{0, 1});
    calculateDotAndDir<GEB::Direction::Down>(bestDirection, maxDot, tempDot);
    // left-up
    tempDot = GEB::Math::dotProduct(normalizedDirVec, GEB::Math::normalize(GEB::Vec2{-1, -1}));
    calculateDotAndDir<GEB::Direction::UpLeft>(bestDirection, maxDot, tempDot);
    // left-down
    tempDot = GEB::Math::dotProduct(normalizedDirVec, GEB::Math::normalize(GEB::Vec2{-1, 1}));
    calculateDotAndDir<GEB::Direction::DownLeft>(bestDirection, maxDot, tempDot);
    // right-up
    tempDot = GEB::Math::dotProduct(normalizedDirVec, GEB::Math::normalize(GEB::Vec2{1, -1}));
    calculateDotAndDir<GEB::Direction::UpRight>(bestDirection, maxDot, tempDot);
    // right-down
    tempDot = GEB::Math::dotProduct(normalizedDirVec, GEB::Math::normalize(GEB::Vec2{1, 1}));
    calculateDotAndDir<GEB::Direction::DownRight>(bestDirection, maxDot, tempDot);

    // Default direction if no match
    return bestDirection == GEB::Direction::None ? prevDirection : bestDirection;
}

void Monster::checkDangerDistance(World &world, Layer &layer)
{
    if (awakened_)
        return;
    // Wake up when the hero enters the detection range.
    GEB::Vec2 heroCenter = world.gameInfo().heroInfo().center();
    float dx = GEB::Math::abs(heroCenter.x - center().x);
    float dy = GEB::Math::abs(heroCenter.y - center().y);

    if (dx <= state_.perceptionRange() && dy <= state_.perceptionRange())
    {
        awaken();
    }
}

void Monster::handleMovement(World &w, Layer &layer)
{
    if (!awakened_)
        return;

    const MapInfo &map = w.gameInfo().mapInfo();
    auto &canvas = w.canvas();
    float dt = w.gameInfo().dt();
    float step = dt * state_.speed();

    auto &mainLayer = dynamic_cast<MainLayer &>(layer);

    auto &playStage = dynamic_cast<PlayStage &>(mainLayer.currentStage());
    auto *gridSystem = playStage.gridSystem();
    auto *stageMap = playStage.map();
    if (!gridSystem || !stageMap)
        return;

    // Move toward the hero and collect nearby obstacles.
    auto oldCenter = center();
    auto delta = GEB::Math::normalize(Monster::toHero(center(), w.gameInfo().heroInfo().center())) * step;
    direction_ = transDirVecToDiretion(delta, direction_);

    GEB::Structures::Array<Entity *> candidateItemList;
    gridSystem->getCandidateListByItem(this, delta, candidateItemList);

    // Include map walls in the collision check.
    stageMap->addCollisionTileToCandiListByDisVec(this,
                                                  delta,
                                                  candidateItemList);

    moveWithCollision(delta, map, candidateItemList.head(), candidateItemList.size());

    gridSystem->updateEntityGridIfNeed(oldCenter, this);
    return;
}

void Monster::update(World &world, Layer &layer)
{
    auto &mainLayer = dynamic_cast<MainLayer &>(layer);
    const auto &playStage = dynamic_cast<const PlayStage &>(mainLayer.currentStage());
    if (!playStage.isHeroAlive())
        return;

    ItemEntity::coreUpdate(world, layer);
    if (!active())
        return;

    checkDangerDistance(world, layer);
}
