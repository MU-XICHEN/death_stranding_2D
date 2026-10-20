#include "stages/Play.h"
#include "entities/Hero.h"
#include "entities/Viewport.h"

PlayStage::PlayStage()
{
    auto hero = new Hero(
        START_POSITION_X,
        START_POSITION_Y,
        ROLE_SIZE,
        ROLE_SIZE,
        PIXEL_PER_SEC);
    hero->setColor(255, 0, 0);
    pushEntityToCharacterList(hero);

    Stage::pushEntity(new Viewport(
        0,
        0,
        VIEWPORT_WIDTH,
        VIEWPORT_HEIGHT));
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