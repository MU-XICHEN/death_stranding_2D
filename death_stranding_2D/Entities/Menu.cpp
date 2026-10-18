
#include "Menu.h"
#include "World.h"

void Menu::handleEvents(World &world)
{
}

void Menu::update(World &world, Layer &layer)
{
}

void Menu::render(World &world, Layer &layer)
{
    auto &canvas = world.canvas();
    for (int i = 0; i < 50; i++)
    {
        for (int j = 0; j < 50; j++)
        {
            canvas.draw(i, j, 0, 0, 255);
        }
    }
}