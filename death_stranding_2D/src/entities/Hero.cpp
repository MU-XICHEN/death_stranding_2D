#include "entities/Hero.h"
#include "world/World.h"
#include "core/GameBase.h"
#include "GamesEngineeringBase.h"
#include "core/Types.h"
#include "layers/MainLayer.h"
#include "stages/Play.h"

template <typename T>
using GEBArray = GEB::Structures::Array<T>;

GEB::Vec2 translateEventsToDirection(const GamesEngineeringBase::Window &canvas)
{
    if (canvas.keyPressed('W') && canvas.keyPressed('A'))
        return GEB::Vec2{-1, -1};
    if (canvas.keyPressed('W') && canvas.keyPressed('D'))
        return GEB::Vec2{1, -1};
    if (canvas.keyPressed('S') && canvas.keyPressed('A'))
        return GEB::Vec2{-1, 1};
    if (canvas.keyPressed('S') && canvas.keyPressed('D'))
        return GEB::Vec2{1, 1};
    if (canvas.keyPressed('W'))
        return GEB::Vec2{0, -1};
    if (canvas.keyPressed('S'))
        return GEB::Vec2{0, 1};
    if (canvas.keyPressed('A'))
        return GEB::Vec2{-1, 0};
    if (canvas.keyPressed('D'))
        return GEB::Vec2{1, 0};
    return GEB::Vec2{0, 0};
}

bool checkIsSingleDirection(GEB::Direction direction)
{
    switch (direction)
    {
    case GEB::Direction::Up:
    case GEB::Direction::Down:
    case GEB::Direction::Left:
    case GEB::Direction::Right:
        return true;
    default:
        return false;
    }
}

void Hero::handleEvents(World &world, Layer &layer)
{
    const MapInfo &map = world.gameInfo().mapInfo();
    auto &canvas = world.canvas();
    float dt = world.gameInfo().dt();
    float distance = dt * speed_;

    auto &mainLayer = dynamic_cast<MainLayer &>(layer);

    auto &playStage = dynamic_cast<PlayStage &>(mainLayer.currentStage());

    auto distanceVec = GEB::Math::normalize(translateEventsToDirection(canvas)) * distance;

    // get candidate items for collision detection
    GEBArray<Entity *> candidateItemList;
    GEB::Boundary mapBoundary{
        .top = 0,
        .bottom = static_cast<float>(map.height()),
        .left = 0,
        .right = static_cast<float>(map.width()),
    };
    GridSystem::getCandidateItemList(playStage.gridSystem(), this, distanceVec, mapBoundary, candidateItemList);

    // add map tiles which may collide with the hero to the candidate list
    playStage.map().addCollisionTileToCandiList(this,
                                                distanceVec,
                                                candidateItemList);

    // get old position
    auto oldCenter = center();

    // move
    moveWithCollision(distanceVec, map, candidateItemList.head(), candidateItemList.size());

    // calculate new position
    auto newCenter = center();

    // update grid system
    playStage.gridSystem().updateEntityGridIfNeed(oldCenter, newCenter, this);
}

void Hero::update(World &world, Layer &layer)
{
    // handle keyboard events for hero
    handleEvents(world, layer);

    HeroInfo heroInfo{
        .x = x_,
        .y = y_,
        .width = width_,
        .height = height_};

    world.gameInfo()
        .updateHeroInfo(heroInfo);

    world.gameInfo().updateViewport(heroInfo, world.gameInfo().mapInfo());
}
