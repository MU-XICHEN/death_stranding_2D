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

void Stage::handleEvents(World &world, Layer &layer)
{
    const auto &canvas = world.canvas();
    if (Stage::enterState.justPressed(canvas.keyPressed(VK_RETURN)))
    {
        auto mainLayer = dynamic_cast<MainLayer *>(&layer);
        if (mainLayer)
        {
            mainLayer->goToNextStageInNextFrame();
        }
    }
}

// Iterate over the list in insertion order, which means the entity added first is updated first.
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

// Iterate over the list in reverse order, which means the entity pushed last is rendered first.
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