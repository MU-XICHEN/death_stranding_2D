#include "Hero.h"
#include "World.h"
#include "GameMath.h"

void Hero::handleEvents(World &world)
{
    const auto &map = world.gameInfo().map();
    auto &canvas = world.canvas();

    if (canvas.keyPressed(VK_UP))
    {
        int resPos = y_ - STEP_STRIDE;
        y_ = GEB::Math::max(resPos, map.topBoundary());
    }
    if (canvas.keyPressed(VK_DOWN))
    {
        int resPos = y_ + STEP_STRIDE;
        y_ = GEB::Math::min(resPos, map.bottomBoundary());
    }
    if (canvas.keyPressed(VK_LEFT))
    {
        int resPos = x_ - STEP_STRIDE;
        x_ = GEB::Math::max(resPos, map.leftBoundary());
    }
    if (canvas.keyPressed(VK_RIGHT))
    {
        int resPos = x_ + STEP_STRIDE;
        x_ = GEB::Math::min(resPos, map.leftBoundary());
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

    int transX = x_ - viewport.x();
    int transY = y_ - viewport.y();

    for (int i = 0; i < width_; i++)
    {
        for (int j = 0; j < height_; j++)
        {
            world.canvas().draw(transX + i, transY + j, color_.r, color_.g, color_.b);
        }
    }
}
