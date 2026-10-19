#include "layers/Layer.h"
#include "core/GameBase.h"

Stage::~Stage()
{
    if (entityList_.empty())
        return;

    auto *headPointer = entityList_.front();
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

void Stage::pushEntity(Entity *entity)
{
    entityList_.push_back(entity);
}

// Iterate over the list in insertion order, which means the entity added first is updated first.
void Stage::updateEntities(World &world, Layer &layer)
{
    if (entityList_.empty())
        return;

    auto *head = entityList_.front();
    while (head)
    {
        auto &entity = head->data;
        entity->update(world, layer);

        head = head->next;
    }
}

// Iterate over the list in reverse order, which means the entity pushed last is rendered first.
void Stage::renderEntities(World &world, Layer &layer)
{
    if (entityList_.empty())
        return;
    auto *tail = entityList_.back();

    while (tail)
    {
        auto &entity = tail->data;
        entity->render(world, layer);

        tail = tail->prev;
    }
}

// ------------------------------------------ Layer ------------------------------------------

Layer::Layer(const StageCreator *stageCreator, unsigned int size, unsigned int initialIndex) : stageCreator_(stageCreator), size_(size), currentIndex_(initialIndex)
{
    size_ = size_;
    currentIndex_ = initialIndex;

    currentStage_ = stageCreator[initialIndex]();

    if (!currentStage_)
    {
        GamesEngineeringBase::Debugger::showAlert();
    }
}

Layer::~Layer()
{
    if (currentStage_)
    {
        delete currentStage_;
        currentStage_ = nullptr;
    }
}

void Layer::checkGoToNextStage()
{
    if (!nextFrameFlag_)
        return;

    // delete current stage
    if (currentStage_)
    {
        delete currentStage_;
        currentStage_ = nullptr;
    }

    // create next stage
    unsigned int index = (++currentIndex_) % size_;
    currentStage_ = stageCreator_[index]();
    nextFrameFlag_ = false;

    if (!currentStage_)
    {
        GamesEngineeringBase::Debugger::showAlert();
        return;
    }
}