
#include "stages/Intro.h"
#include "world/World.h"
#include "layers/Layer.h"
#include "layers/MainLayer.h"

void IntroStage::update(World &world, Layer &layer)
{
    const auto &canvas = world.canvas();
    if (Stage::enterState.justPressed(canvas.keyPressed(VK_RETURN)))
    {
        auto mainLayer = dynamic_cast<MainLayer *>(&layer);
        if (mainLayer)
        {
            mainLayer->goToNextStageInNextFrame();
        }
    }
}

void IntroStage::render(World &world, Layer &layer)
{
    auto &canvas = world.canvas();
    for (int y = 0; y < 50; y++)
    {
        for (int x = 0; x < 50; x++)
        {
            canvas.draw(x, y, 255, 0, 0);
        }
    }
}
