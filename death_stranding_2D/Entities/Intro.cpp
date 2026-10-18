
#include "Intro.h"
#include "World.h"

void Intro::handleEvents(World &world)
{
}

void Intro::update(World &world, Layer &layer)
{
}

void Intro::render(World &world, Layer &layer)
{
    auto &canvas = world.canvas();
    for (int i = 0; i < 50; i++)
    {
        for (int j = 0; j < 50; j++)
        {
            canvas.draw(i, j, 0, 255, 0);
        }
    }
}