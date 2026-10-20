#include "entities/Hero.h"
#include "world/World.h"
#include "core/GameBase.h"
#include "GamesEngineeringBase.h"

void Hero::handleEvents(World &world)
{
    auto &canvas = world.canvas();

    const auto &map = world.gameInfo().map();

    float dt = world.gameInfo().dt();

    float distance = dt * PIXEL_PER_SEC;

    // Handle movement
    if (canvas.keyPressed('W') && canvas.keyPressed('A'))
    {
        moveUpLeft(distance, static_cast<float>(map.leftBoundary()), static_cast<float>(map.topBoundary()));
    }
    else if (canvas.keyPressed('W') && canvas.keyPressed('D'))
    {
        moveUpRight(distance, static_cast<float>(map.rightBoundary() - width_), static_cast<float>(map.topBoundary()));
    }
    else if (canvas.keyPressed('S') && canvas.keyPressed('A'))
    {
        moveDownLeft(distance, static_cast<float>(map.leftBoundary()), static_cast<float>(map.bottomBoundary() - height_));
    }
    else if (canvas.keyPressed('S') && canvas.keyPressed('D'))
    {
        moveDownRight(distance, static_cast<float>(map.rightBoundary() - width_), static_cast<float>(map.bottomBoundary() - height_));
    }
    else if (canvas.keyPressed('W'))
    {
        moveUp(distance, static_cast<float>(map.topBoundary()));
    }
    else if (canvas.keyPressed('S'))
    {
        moveDown(distance, static_cast<float>(map.bottomBoundary() - height_));
    }
    else if (canvas.keyPressed('A'))
    {
        moveLeft(distance, static_cast<float>(map.leftBoundary()));
    }
    else if (canvas.keyPressed('D'))
    {
        moveRight(distance, static_cast<float>(map.rightBoundary() - width_));
    }
}

void Hero::update(World &world, Layer &layer)
{
    // handle keyboard events for hero
    handleEvents(world);
    world.gameInfo().updateHero(*this);
}

void Hero::render(World &world, Layer &layer)
{
    const Environment &environment = world.gameInfo().environment();

    float transX = x_ - environment.x();
    float transY = y_ - environment.y();

    // [todo: remove]
    if (transX < 0 || transY < 0)
    {
        GEB::Debugger::showAlert();
    }

    for (int i = 0; i < width_; i++)
    {
        for (int j = 0; j < height_; j++)
        {
            float targetX = transX + i;
            float targetY = transY + j;
            world.canvas().draw(static_cast<int>(transX + i),
                                static_cast<int>(transY + j),
                                color_.r,
                                color_.g,
                                color_.b);
        }
    }
}
