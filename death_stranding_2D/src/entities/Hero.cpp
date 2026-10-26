#include "entities/Hero.h"
#include "world/World.h"
#include "core/GameBase.h"
#include "GamesEngineeringBase.h"
#include "core/Types.h"
#include "stages/Play.h"

template <typename T>
using GEBArray = GEB::Structures::Array<T>;

GEB::Direction translateEventsToDirection(const GamesEngineeringBase::Window &canvas)
{
    if (canvas.keyPressed('W') && canvas.keyPressed('A'))
        return GEB::Direction::UpLeft;
    if (canvas.keyPressed('W') && canvas.keyPressed('D'))
        return GEB::Direction::UpRight;
    if (canvas.keyPressed('S') && canvas.keyPressed('A'))
        return GEB::Direction::DownLeft;
    if (canvas.keyPressed('S') && canvas.keyPressed('D'))
        return GEB::Direction::DownRight;
    if (canvas.keyPressed('W'))
        return GEB::Direction::Up;
    if (canvas.keyPressed('S'))
        return GEB::Direction::Down;
    if (canvas.keyPressed('A'))
        return GEB::Direction::Left;
    if (canvas.keyPressed('D'))
        return GEB::Direction::Right;
    return GEB::Direction::None;
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

    auto &playStage = dynamic_cast<PlayStage &>(layer.currentStage());

    // get candidate items for collision detection
    GEBArray<Entity *> candidateItemList;
    GridSystem::getCandidateItemList(playStage.gridSystem(), this, distance, candidateItemList);

    // get old position
    auto oldCenter = center();
    GEB::Direction direction = translateEventsToDirection(canvas);

    // add map tiles which may collide with the hero to the candidate list
    playStage.map().addCollisionTileToCandiList(this,
                                                direction,
                                                distance,
                                                candidateItemList);

    // move
    moveWithCollision(distance, direction, map, candidateItemList.head(), candidateItemList.size());

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
