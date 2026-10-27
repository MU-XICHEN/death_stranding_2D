
#include "stages/Menu.h"
#include "world/World.h"
#include "layers/Layer.h"
#include "layers/MainLayer.h"

void MenuStage::update(World &world, Layer &layer)
{
    handleEvents(world, layer);
}

void MenuStage::render(World &world, Layer &layer)
{
    auto &canvas = world.canvas();
    for (int i = 0; i < 50; i++)
    {
        for (int j = 0; j < 50; j++)
        {
            canvas.draw(i, j, 255, 0, 0);
        }
    }
}