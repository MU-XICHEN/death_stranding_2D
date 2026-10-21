#pragma once

#include "entities/Entity.h"
#include "core/GameBase.h"
#include "GamesEngineeringBase.h"
#include "stages/Stage.h"

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