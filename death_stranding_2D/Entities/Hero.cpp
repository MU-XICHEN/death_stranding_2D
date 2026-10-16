#include "Hero.h"
#include "World.h"
#include "GameBase.h"
#include "GamesEngineeringBase.h"

void Hero::handleEvents(World &world)
{
    const auto &map = world.gameInfo().map();
    auto &canvas = world.canvas();

    if (canvas.keyPressed('W'))
    {
        int resPos = y_ - STEP_STRIDE;
        y_ = GEB::Math::max(resPos, map.topBoundary());
    }
    if (canvas.keyPressed('S'))
    {
        int resPos = y_ + STEP_STRIDE;
        y_ = GEB::Math::min(resPos, map.bottomBoundary() - height_);
    }
    if (canvas.keyPressed('A'))
    {
        int resPos = x_ - STEP_STRIDE;
        x_ = GEB::Math::max(resPos, map.leftBoundary());
    }
    if (canvas.keyPressed('D'))
    {
        int resPos = x_ + STEP_STRIDE;
        x_ = GEB::Math::min(resPos, map.rightBoundary() - width_);
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

    // [todo: remove]
    if (transX < 0 || transY < 0)
    {
        GEB::Debugger::showAlert();
    }

    for (int i = 0; i < width_; i++)
    {
        for (int j = 0; j < height_; j++)
        {
            int targetX = transX + i;
            int targetY = transY + j;
            world.canvas().draw(transX + i, transY + j, color_.r, color_.g, color_.b);
        }
    }
}
