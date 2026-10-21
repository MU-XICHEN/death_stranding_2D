#include "stages/Play.h"
#include "entities/Hero.h"
#include "entities/Environment.h"
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

    // environment initialization
    Stage::pushEntity(new Environment(
        0,
        0,
        ENVIRONMENT_WIDTH,
        ENVIRONMENT_HEIGHT));
}

PlayStage::~PlayStage()
{
    if (characterList_.empty())
        return;

    auto *headPointer = characterList_.front();
    while (headPointer)
    {
        // delete list nodes
        auto *temp = headPointer->next;

        headPointer->next = nullptr;
        headPointer->prev = nullptr;

        if (headPointer->data)
            delete headPointer->data;

        delete headPointer;

        headPointer = temp;
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
    auto *tail = characterList_.back();

    while (tail)
    {
        auto &character = tail->data;
        character->render(world, layer);

        tail = tail->prev;
    }

    Stage::renderEntities(world, layer);
}