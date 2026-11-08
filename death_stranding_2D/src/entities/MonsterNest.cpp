#include "entities/MonsterNest.h"
#include "entities/Hero.h"
#include "entities/Monster.h"
#include "skills/SkillCreateMonster.h"
#include "renderer/ImageRenderer.h"
#include <string>

MonsterNest::MonsterNest(float x, float y, int width, int height)
    : ItemEntity(x, y, width, height)
{
    GEB::Pair<int, float> props[] = {
        {State::HEALTH, MONSTER_NEST_HP},
        {State::DEFENSE, MONSTER_NEST_DEFENSE},
    };
    initializeState(props, sizeof(props) / sizeof(props[0]));

    addSkill(new SkillCreateMonster<Ghost>(6.0f, 10));
}

bool MonsterNest::canGenerateMonsters() const
{
    const auto *generator = static_cast<const SkillCreateMonster<Ghost> *>(skills_[0]);
    return active() && state_.health() > 0.0f && !generator->finishedGenerating();
}

void MonsterNest::render(World &world, Layer &layer)
{
    if (!active())
        return;
    const auto &viewport = world.gameInfo().viewport();
    // Align the larger gate image with the nest's ground footprint.
    int left = static_cast<int>(x_ - viewport.x()) - width_ / 2;
    int top = static_cast<int>(y_ - viewport.y()) - height_;
    drawImage(world, AssetId::MonsterNestSprite, left, top);

    // Render the health text.
    const std::string health = "Nest HP: " + std::to_string(static_cast<int>(state_.health()));
    world.textRenderer().drawString(world, health.c_str(), left + 24, top,
                                    GEB::TextSizeType::TEXT_SIZE_SMALL);
}

bool MonsterNest::canAttackTarget(const ItemEntity *target) const
{
    return dynamic_cast<const Hero *>(target) != nullptr;
}

void MonsterNest::update(World &world, Layer &layer)
{
    ItemEntity::coreUpdate(world, layer);
}
