#pragma once

#include "stages/Stage.h"
#include "entities/Character.h"

class PlayStage : public Stage
{
public:
    PlayStage();
    virtual ~PlayStage() override;
    void pushEntityToCharacterList(Character *character);
    void updateEntities(World &world, Layer &layer) override;
    void renderEntities(World &world, Layer &layer) override;

private:
    GEB::Structures::Array<Character *> characterList_;
};
