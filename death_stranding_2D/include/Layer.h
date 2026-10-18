#pragma once

#include "Entity.h"
#include "DoublyLinkedList.h"
#include "GameBase.h"
#include "GamesEngineeringBase.h"

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

using StageCreator = Stage *(*)();

class Layer
{
public:
    Layer(const StageCreator *stageCreator, unsigned int size, unsigned int initialIndex = 0);
    virtual ~Layer();

    Stage &currentStage() const { return *currentStage_; }

    bool nextFrameFlag() { return nextFrameFlag_; }
    void setNextFrameFlag() { nextFrameFlag_ = true; }

    void checkGoToNextStage();

private:
    // members variables
    Stage *currentStage_ = nullptr;
    unsigned int currentIndex_ = 0;
    unsigned int size_ = 0;
    bool nextFrameFlag_ = false;

    const StageCreator *stageCreator_ = nullptr;
};