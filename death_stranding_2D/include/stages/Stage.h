#pragma once

#include "entities/Entity.h"
#include "utils/DoublyLinkedList.h"

class Stage
{
public:
    Stage() = default;
    virtual ~Stage();
    void pushEntity(Entity *entity);
    void updateEntities(World &world, Layer &layer);
    void renderEntities(World &world, Layer &layer);

private:
    // List includes various types of Entity, so pass <Entity *>
    GEB::Structures::DoublylinkedList<Entity *> entityList_;
};
