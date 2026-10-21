#include "stages/Play.h"
#include "entities/Hero.h"
#include "world/Viewport.h"
#include "entities/Monster.h"

PlayStage::PlayStage()
{
    // character initialization
    auto hero = new Hero(
        START_POSITION_X,
        START_POSITION_Y,
        ROLE_SIZE,
        ROLE_SIZE,
        PIXEL_PER_SEC);
    hero->setColor(255, 0, 0);
    pushEntityToCharacterList(hero);

    auto beast1 = new Beast(
        500,
        500,
        ROLE_SIZE,
        ROLE_SIZE);
    beast1->setColor(0, 0, 255);
    pushEntityToCharacterList(beast1);

    auto ghost1 = new Ghost(
        300,
        300,
        ROLE_SIZE,
        ROLE_SIZE);
    ghost1->setColor(0, 255, 0);
    pushEntityToCharacterList(ghost1);

    // add map
    auto map = new Map(
        0,
        0,
        MAP_WIDTH,
        MAP_HEIGHT,
        MAP_BORDER_SIZE,
        GEB::Color(255, 255, 255));
    pushEntity(map);
}

PlayStage::~PlayStage()
{
    if (characterList_.size() == 0)
        return;

    for (int i = 0; i < characterList_.size(); ++i)
    {
        if (characterList_[i] == nullptr)
            continue;
        delete characterList_[i];
        characterList_[i] = nullptr;
    }
}

void PlayStage::pushEntityToCharacterList(Character *character)
{
    characterList_.push_back(character);
}

// updateEntities
void PlayStage::updateEntities(World &world, Layer &layer)
{
    if (characterList_.size() == 0)
        return;

    for (int i = 0; i < characterList_.size(); ++i)
    {
        if (characterList_[i])
            characterList_[i]->update(world, layer);
    }

    Stage::updateEntities(world, layer);
}

// renderEntities
void PlayStage::renderEntities(World &world, Layer &layer)
{
    if (characterList_.size() == 0)
        return;

    for (int i = 0; i < characterList_.size(); ++i)
    {
        if (characterList_[i])
            characterList_[i]->render(world, layer);
    }

    Stage::renderEntities(world, layer);
}