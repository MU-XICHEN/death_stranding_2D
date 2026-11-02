
#include "stages/Menu.h"
#include "world/World.h"
#include "layers/Layer.h"
#include "layers/MainLayer.h"
#include "world/AssetManager.h"

void MenuStage::moveSelection(int direction)
{
    int newIdx = selectedIdx_ + direction;
    if (newIdx < 0)
        newIdx = 0;
    if (newIdx > maxIdx_)
        newIdx = maxIdx_;
    selectedIdx_ = newIdx;
}

void MenuStage::update(World &world, Layer &layer)
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

    // handle current events
    if (wKeyState_.justPressed(canvas.keyPressed('W')) || wKeyState_.justPressed(canvas.keyPressed(VK_UP)))
        moveSelection(-1);
    else if (sKeyState_.justPressed(canvas.keyPressed('S')) || sKeyState_.justPressed(canvas.keyPressed(VK_DOWN)))
        moveSelection(1);
}

void MenuStage::renderSelection(World &world, Layer &layer)
{
}

void MenuStage::render(World &world, Layer &layer)
{
    // render the background cover
    auto &canvas = world.canvas();

    auto *image = world.assetManager().getImage(AssetId::Cover);
    if (!image || image->channels != 3)
        return;

    auto *buffer = canvas.getBackBuffer();
    if (!buffer)
        return;

    auto imageWidth = image->width;
    auto imageHeight = image->height;

    auto windowWidth = canvas.getWidth();
    auto windowHeight = canvas.getHeight();

    if (imageWidth != windowWidth || imageHeight != windowHeight)
        return;

    int bytes = imageWidth * imageHeight * 3;
    memcpy(buffer, image->data, bytes);

    // render the selection
    renderSelection(world, layer);
}