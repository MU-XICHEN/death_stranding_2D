#pragma once

#include "stages/Stage.h"
#include "entities/Hero.h"
#include "entities/Viewport.h"

class PlayStage : public Stage
{
public:
    PlayStage();
    virtual ~PlayStage() override;
    void pushEntityToCharacterList(Character *character);
    void updateEntities(World &world, Layer &layer) override;
    void renderEntities(World &world, Layer &layer) override;

private:
    GEB::Structures::DoublyLinkedList<Character *> characterList_;
};
