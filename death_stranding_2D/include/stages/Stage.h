#pragma once

#include "entities/Entity.h"
#include "structure/Array.h"

class Stage
{
public:
    Stage() = default;
    virtual ~Stage();
    void pushEntity(Entity *entity);
    virtual void updateEntities(World &world, Layer &layer);
    virtual void renderEntities(World &world, Layer &layer);

private:
    GEB::Structures::Array<Entity *> entityList_;
};
