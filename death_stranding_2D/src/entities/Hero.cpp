#include "entities/Hero.h"
#include "world/World.h"
#include "core/GameBase.h"
#include "GamesEngineeringBase.h"

void Hero::handleEvents(World &world)
{
    auto &canvas = world.canvas();

    const auto &map = world.gameInfo().map();

    float dt = world.gameInfo().dt();

    float distance = dt * speed_;

    // Handle movement
    if (canvas.keyPressed('W') && canvas.keyPressed('A'))
    {
        moveUpLeft(distance, map);
    }
    else if (canvas.keyPressed('W') && canvas.keyPressed('D'))
    {
        moveUpRight(distance, map);
    }
    else if (canvas.keyPressed('S') && canvas.keyPressed('A'))
    {
        moveDownLeft(distance, map);
    }
    else if (canvas.keyPressed('S') && canvas.keyPressed('D'))
    {
        moveDownRight(distance, map);
    }
    else if (canvas.keyPressed('W'))
    {
        moveUp(distance, map);
    }
    else if (canvas.keyPressed('S'))
    {
        moveDown(distance, map);
    }
    else if (canvas.keyPressed('A'))
    {
        moveLeft(distance, map);
    }
    else if (canvas.keyPressed('D'))
    {
        moveRight(distance, map);
    }
}

void Hero::update(World &world, Layer &layer)
{
    // handle keyboard events for hero
    handleEvents(world);

    HeroInfo heroInfo{
        .x = x_,
        .y = y_,
        .width = width_,
        .height = height_};

    world.gameInfo()
        .updateHeroInfo(heroInfo);
        
    world.gameInfo().updateViewport(heroInfo);
}
