#include "stages/Stage.h"

Stage::~Stage()
{
    if (entityList_.size() == 0)
        return;

    for (int i = 0; i < entityList_.size(); ++i)
    {
        if (entityList_[i] == nullptr)
            continue;
        delete entityList_[i];
        entityList_[i] = nullptr;
    }
}

void Stage::pushEntity(Entity *entity)
{
    entityList_.pushBack(entity);
}

// Iterate over the list in insertion order, which means the entity added first is updated first.
void Stage::updateEntities(World &world, Layer &layer)
{
    if (entityList_.size() == 0)
        return;

    for (int i = 0; i < entityList_.size(); ++i)
    {
        if (entityList_[i])
            entityList_[i]->update(world, layer);
    }
}

// Iterate over the list in reverse order, which means the entity pushed last is rendered first.
void Stage::renderEntities(World &world, Layer &layer)
{
    if (entityList_.size() == 0)
        return;

    for (int i = entityList_.size() - 1; i >= 0; --i)
    {
        if (entityList_[i])
            entityList_[i]->render(world, layer);
    }
}