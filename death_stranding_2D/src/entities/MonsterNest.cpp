#include "entities/MonsterNest.h"

MonsterNest::MonsterNest(float x, float y, int width, int height)
    : ItemEntity(x, y, width, height)
{
    GEB::Pair<int, float> props[] = {
        {State::HEALTH, MONSTER_NEST_HP},
        {State::DEFENSE, MONSTER_NEST_DEFENSE},
    };
    initializeState(props, sizeof(props) / sizeof(props[0]));
    setColor(128, 80, 48);
}

void MonsterNest::update(World &world, Layer &layer)
{
    // The shared lifecycle stops skills and removes the nest after destruction.
    ItemEntity::coreUpdate(world, layer);
}
