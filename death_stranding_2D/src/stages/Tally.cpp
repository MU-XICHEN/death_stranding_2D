#include "stages/Tally.h"
#include "world/World.h"
#include "layers/Layer.h"
#include "layers/MainLayer.h"

void TallyStage::update(World &world, Layer &layer)
{
}

void TallyStage::render(World &world, Layer &layer)
{
    auto &canvas = world.canvas();

    int size = 50;
    for (int y = 0; y < size; y++)
    {
        for (int x = 0; x < size; x++)
        {
            canvas.draw(x, y, 0, 0, 255);
        }
    }
}
