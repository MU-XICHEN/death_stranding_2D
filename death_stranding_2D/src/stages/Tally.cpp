#include "stages/Tally.h"
#include "world/World.h"
#include "layers/Layer.h"
#include "layers/MainLayer.h"
#include <string>

void TallyStage::update(World &world, Layer &layer)
{
    if (Stage::enterState.justPressed(world.canvas().keyPressed(VK_RETURN)))
        dynamic_cast<MainLayer &>(layer).goToNextStageInNextFrame();
}

void TallyStage::render(World &world, Layer &layer)
{
    auto &canvas = world.canvas();
    const auto &result = world.gameInfo().runResult();
    // Choose the background for a win or a loss.
    const auto *background = world.assetManager().getImage(
        result.victory ? AssetId::TallyVictory : AssetId::TallyDefeat);
    if (background && background->channels == 3 &&
        background->width == canvas.getWidth() && background->height == canvas.getHeight())
    {
        memcpy(canvas.getBackBuffer(), background->data, background->width * background->height * 3);
    }

    // Draw the title and the line below it.
    auto &text = world.textRenderer();
    const GEB::Color ink{57, 39, 25};
    int left = static_cast<int>(canvas.getWidth()) - 344;
    text.drawString(world, result.victory ? "VICTORY" : "GAME OVER", left, 310,
                    GEB::TextSizeType::TEXT_SIZE_LARGE, ink);
    for (int x = left; x < left + 264; ++x)
        canvas.draw(x, 364, 112, 90, 65);

    // Show the hero, kills, and final score.
    const char *heroName = result.heroType == HeroType::Archer ? "Archer" : "Swordsman";
    text.drawString(world, "HERO", left, 390, GEB::TextSizeType::TEXT_SIZE_SMALL, ink);
    text.drawString(world, heroName, left, 416, GEB::TextSizeType::TEXT_SIZE_MEDIUM, ink);
    text.drawString(world, "MONSTERS DEFEATED", left, 466, GEB::TextSizeType::TEXT_SIZE_SMALL, ink);
    text.drawString(world, std::to_string(result.kills).c_str(), left, 492,
                    GEB::TextSizeType::TEXT_SIZE_MEDIUM, ink);
    text.drawString(world, "FINAL SCORE", left, 542, GEB::TextSizeType::TEXT_SIZE_SMALL, ink);
    text.drawString(world, std::to_string(result.score).c_str(), left, 568,
                    GEB::TextSizeType::TEXT_SIZE_LARGE, ink);
    // Show how to return to the menu.
    text.drawString(world, "Press Enter to return to menu", left, 660,
                    GEB::TextSizeType::TEXT_SIZE_SMALL, ink);
}
