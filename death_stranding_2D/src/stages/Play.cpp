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
}

PlayStage::~PlayStage()
{
    if (characterList_.empty())
        return;

    auto *headPointer = characterList_.front();
    while (headPointer)
    {
        if (headPointer->data)
            delete headPointer->data;

        headPointer = headPointer->next;
    }
}

void PlayStage::pushEntityToCharacterList(Character *character)
{
    characterList_.push_back(character);
}

// updateEntities
void PlayStage::updateEntities(World &world, Layer &layer)
{
    if (characterList_.empty())
        return;

    auto *head = characterList_.front();
    while (head)
    {
        auto &character = head->data;
        character->update(world, layer);

        head = head->next;
    }

    Stage::updateEntities(world, layer);
}

// renderEntities
void PlayStage::renderEntities(World &world, Layer &layer)
{
    if (characterList_.empty())
        return;
    auto *head = characterList_.front();

    while (head)
    {
        auto &character = head->data;
        character->render(world, layer);

        head = head->next;
    }

    Stage::renderEntities(world, layer);
}