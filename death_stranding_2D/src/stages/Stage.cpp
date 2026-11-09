#include "stages/Stage.h"
#include "world/World.h"
#include "layers/Layer.h"
#include "layers/MainLayer.h"

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

// Update entities in the order they were added.
void Stage::update(World &world, Layer &layer)
{
    if (entityList_.size() == 0)
        return;

    for (int i = 0; i < entityList_.size(); ++i)
    {
        if (entityList_[i])
            entityList_[i]->update(world, layer);
    }
}

// Draw in reverse order, so earlier entities appear on top.
void Stage::render(World &world, Layer &layer)
{
    if (entityList_.size() == 0)
        return;

    for (int i = entityList_.size() - 1; i >= 0; --i)
    {
        if (entityList_[i])
            entityList_[i]->render(world, layer);
    }
}

void Stage::renderFinished(World &world, Layer &layer) {}
