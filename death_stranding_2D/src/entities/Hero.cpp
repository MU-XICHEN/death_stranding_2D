#include "entities/Hero.h"
#include "world/World.h"
#include "core/GameBase.h"
#include "GamesEngineeringBase.h"
#include "core/Types.h"

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

void Hero::handleEvents(World &world, Layer &layer)
{
    auto &canvas = world.canvas();

    float dt = world.gameInfo().dt();

    float distance = dt * speed_;

    GEB::Direction direction = translateEventsToDirection(canvas);

    switch (direction)
    {
    case GEB::Direction::Up:
    case GEB::Direction::UpLeft:
    case GEB::Direction::UpRight:
        moveUp(distance, calculateBoundary(world, layer, GEB::Direction::Up).top);
        break;
    case GEB::Direction::Down:
    case GEB::Direction::DownLeft:
    case GEB::Direction::DownRight:
        moveDown(distance, calculateBoundary(world, layer, GEB::Direction::Down).bottom);
        break;
    default:
        break;
    }

    switch (direction)
    {
    case GEB::Direction::Left:
    case GEB::Direction::UpLeft:
    case GEB::Direction::DownLeft:
        moveLeft(distance, calculateBoundary(world, layer, GEB::Direction::Left).left);
        break;
    case GEB::Direction::Right:
    case GEB::Direction::UpRight:
    case GEB::Direction::DownRight:
        moveRight(distance, calculateBoundary(world, layer, GEB::Direction::Right).right);
        break;
    default:
        break;
    }
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

    world.gameInfo().updateViewport(heroInfo);
}
