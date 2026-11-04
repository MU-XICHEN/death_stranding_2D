#include "entities/MonsterNest.h"
#include "entities/Hero.h"
#include "entities/Monster.h"
#include "skills/SkillCreateMonster.h"

MonsterNest::MonsterNest(float x, float y, int width, int height)
    : ItemEntity(x, y, width, height)
{
    GEB::Pair<int, float> props[] = {
        {State::HEALTH, MONSTER_NEST_HP},
        {State::DEFENSE, MONSTER_NEST_DEFENSE},
    };
    initializeState(props, sizeof(props) / sizeof(props[0]));
    setColor(128, 80, 48);

    addSkill(new SkillCreateMonster<Beast>(3.0f, 10));
}

bool MonsterNest::canAttackTarget(const ItemEntity *target) const
{
    return dynamic_cast<const Hero *>(target) != nullptr;
}

void MonsterNest::update(World &world, Layer &layer)
{
    // The shared lifecycle stops skills and removes the nest after destruction.
    ItemEntity::coreUpdate(world, layer);
}
