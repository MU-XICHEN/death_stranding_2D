#include "Hero.h"
#include "World.h"
#include "GameBase.h"
#include "GamesEngineeringBase.h"

void Hero::handleEvents(World &world)
{
    const auto &map = world.gameInfo().map();
    auto &canvas = world.canvas();

    float dt = world.timer().dt();

    float distance = dt * PIXEL_PER_SEC;

    // Handle movement
    if (canvas.keyPressed('W') && canvas.keyPressed('A'))
    {
        moveUp(distance, static_cast<float>(map.topBoundary()));
        moveLeft(distance, static_cast<float>(map.leftBoundary()));
    }
    else if (canvas.keyPressed('W') && canvas.keyPressed('D'))
    {
        moveUp(distance, static_cast<float>(map.topBoundary()));
        moveRight(distance, static_cast<float>(map.rightBoundary() - width_));
    }
    else if (canvas.keyPressed('S') && canvas.keyPressed('A'))
    {
        moveDown(distance, static_cast<float>(map.bottomBoundary() - height_));
        moveLeft(distance, static_cast<float>(map.leftBoundary()));
    }
    else if (canvas.keyPressed('S') && canvas.keyPressed('D'))
    {
        moveDown(distance, static_cast<float>(map.bottomBoundary() - height_));
        moveRight(distance, static_cast<float>(map.rightBoundary() - width_));
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

void Hero::update(World &world)
{
    handleEvents(world);
    world.gameInfo().updateHero(*this);
}

void Hero::render(World &world)
{
    const Viewport &viewport = world.gameInfo().viewport();

    float transX = x_ - viewport.x();
    float transY = y_ - viewport.y();

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
