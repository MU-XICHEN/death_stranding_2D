#include "entities/Monster.h"
#include "core/GameBase.h"
#include "world/World.h"
#include "world/Grid.h"
#include "stages/Play.h"

template <GEB::Direction dir>
void calculateDotAndDir(GEB::Direction &bestDir, float &dot, float newDot)
{
    if (newDot > dot)
    {
        dot = newDot;
        bestDir = dir;
    }
}

GEB::Direction Monster::transDirVecToDiretion(const GEB::Vec2 &dirVec)
{
    if (dirVec.x == 0 && dirVec.y == 0)
        return GEB::Direction::None;

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
    return bestDirection;
}

void Monster::checkDangerDistance(World &world, Layer &layer)
{

    if (awakened_)
        return;
    GEB::Position heroCenter = world.gameInfo().heroInfo().center();
    float dx = GEB::Math::abs(heroCenter.x - center().x);
    float dy = GEB::Math::abs(heroCenter.y - center().y);

    if (dx <= dangerDistance_ && dy <= dangerDistance_)
    {
        awaken();
    }
}

void Monster::handleMovement(World &world, Layer &layer)
{
    const Map &map = world.gameInfo().map();
    auto &canvas = world.canvas();
    float dt = world.gameInfo().dt();
    float distance = dt * speed_;

    auto &playStage = dynamic_cast<PlayStage &>(layer.currentStage());

    // get candidate items for collision detection
    GEB::Structures::Array<ItemEntity *> candidateItemList;
    GridSystem::getCandidateItemList(playStage.gridSystem(), this, candidateItemList);

    // get old position
    auto oldCenter = center();

    auto dirVec = Monster::calculateDirVecToHero(center(), world.gameInfo().heroInfo().center());
    auto direction = Monster::transDirVecToDiretion(dirVec);

    moveWithCollision(distance, direction, map, candidateItemList.head(), candidateItemList.size());

    // calculate new position
    auto newCenter = center();
    playStage.gridSystem().updateEntityGridIfNeed(oldCenter, newCenter, this);
    return;
}