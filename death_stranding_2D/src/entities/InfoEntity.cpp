#include <algorithm>
#include "entities/InfoEntity.h"
#include "entities/Hero.h"
#include "world/World.h"

InfoEntity::InfoEntity(const Hero &hero)
    : hero_(&hero)
{
    refreshText();
}

void InfoEntity::refreshText()
{
    if (!hero_)
        return;

    const auto &state = hero_->state();
    healthText_ = "Health: " + std::to_string(static_cast<int>(std::max(0.0f, state.health())));
    manaText_ = "Mana: " + std::to_string(static_cast<int>(std::max(0.0f, state.mana())));
    scoreText_ = "Score: " + std::to_string(static_cast<int>(state.score()));

    if (!hero_->active())
        hero_ = nullptr;
}

void InfoEntity::update(World &world, Layer &layer)
{
    refreshText();
}

void InfoEntity::render(World &world, Layer &layer)
{
    auto &canvas = world.canvas();
    constexpr int margin = 20;
    constexpr int lineGap = 8;
    constexpr int fontWidth = GEB::TextSizeW::TEXT_SIZE_W_MEDIUM;
    constexpr int fontHeight = GEB::TextSizeH::TEXT_SIZE_H_MEDIUM;

    const int textLength = static_cast<int>(std::max({healthText_.size(), manaText_.size(), scoreText_.size()}));
    width_ = textLength * fontWidth;
    height_ = fontHeight * 3 + lineGap * 2;
    x_ = static_cast<float>(std::max(0, static_cast<int>(canvas.getWidth()) - margin - width_));
    y_ = static_cast<float>(margin);

    const int left = static_cast<int>(x_);
    const int top = static_cast<int>(y_);

    // render information text
    auto &textRenderer = world.textRenderer();
    textRenderer.drawString(world, healthText_.c_str(), left, top,
                            GEB::TextSizeType::TEXT_SIZE_MEDIUM);
    textRenderer.drawString(world, manaText_.c_str(), left, top + fontHeight + lineGap,
                            GEB::TextSizeType::TEXT_SIZE_MEDIUM);
    textRenderer.drawString(world, scoreText_.c_str(), left, top + (fontHeight + lineGap) * 2,
                            GEB::TextSizeType::TEXT_SIZE_MEDIUM);
}
