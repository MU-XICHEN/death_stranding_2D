
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
    // Handle menu logic:
    // enter to continue
    // move selection with up/down keys
    const auto &canvas = world.canvas();
    const bool upPressed = wKeyState_.justPressed(canvas.keyPressed('W') || canvas.keyPressed(VK_UP));
    const bool downPressed = sKeyState_.justPressed(canvas.keyPressed('S') || canvas.keyPressed(VK_DOWN));
    if (Stage::enterState.justPressed(canvas.keyPressed(VK_RETURN)))
    {
        if (!selectingHero_)
        {
            selectingHero_ = true;
            return;
        }

        auto mainLayer = dynamic_cast<MainLayer *>(&layer);
        if (mainLayer)
        {
            mainLayer->setSelectedHero(selectedIdx_ == 0 ? HeroType::Swordsman : HeroType::Archer);
            mainLayer->goToNextStageInNextFrame();
        }
        return;
    }

    if (!selectingHero_)
        return;
    if (upPressed)
        moveSelection(-1);
    else if (downPressed)
        moveSelection(1);
}

void MenuStage::renderSelection(World &world, Layer &layer)
{
    auto &canvas = world.canvas();
    auto &text = world.textRenderer();
    const GEB::Color ink{57, 39, 25};
    const int left = static_cast<int>(canvas.getWidth()) - 232;
    const int top = 350;

    if (!selectingHero_)
    {
        text.drawString(world, "Press Enter to continue", left + 12, top + 22,
                        GEB::TextSizeType::TEXT_SIZE_SMALL, ink);
        return;
    }

    text.drawString(world, "Choose your hero", left + 12, top + 16,
                    GEB::TextSizeType::TEXT_SIZE_SMALL, ink);
    text.drawString(world, selectedIdx_ == 0 ? "> Swordsman" : "  Swordsman", left + 16, top + 54,
                    GEB::TextSizeType::TEXT_SIZE_MEDIUM, ink);
    text.drawString(world, selectedIdx_ == 1 ? "> Archer" : "  Archer", left + 16, top + 88,
                    GEB::TextSizeType::TEXT_SIZE_MEDIUM, ink);
    text.drawString(world, "Up/Down to select", left + 12, top + 130,
                    GEB::TextSizeType::TEXT_SIZE_SMALL, ink);
    text.drawString(world, "Enter to start", left + 12, top + 152,
                    GEB::TextSizeType::TEXT_SIZE_SMALL, ink);
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
