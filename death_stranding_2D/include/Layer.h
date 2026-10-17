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
    virtual void pushEntity(Entity *entity);
    virtual void updateEntities(World &world);
    virtual void renderEntities(World &world);

private:
    // List includes various types of Entity, so pass <Entity *>
    GEB::Structures::DoublylinkedList<Entity *> entityList_;
};

using StageCreator = Stage *(*)();

class Layer
{
public:
    explicit Layer(const StageCreator *stageCreator);
    ~Layer();

    Stage *currentStage() { return currentStage_; }

    void nextStage();

protected:
    Stage *currentStage_ = nullptr;
};