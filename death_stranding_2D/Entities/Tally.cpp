
#include "Tally.h"
#include "World.h"

void Tally::handleEvents(World &world)
{
}

void Tally::update(World &world)
{
}

void Tally::render(World &world)
{
    auto &canvas = world.canvas();

    int size = 50;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            canvas.draw(i, j, 0, 0, 255);
        }
    }
}