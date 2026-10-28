#include "stages/Tally.h"
#include "world/World.h"
#include "layers/Layer.h"
#include "layers/MainLayer.h"

void TallyStage::update(World &world, Layer &layer)
{
    handleEvents(world, layer);
}

void TallyStage::render(World &world, Layer &layer)
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