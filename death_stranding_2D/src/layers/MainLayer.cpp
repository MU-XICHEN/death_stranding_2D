#include "layers/MainLayer.h"
#include "core/GameBase.h"

// ------------------------------------------ Layer ------------------------------------------

const int initialIndex = 2;

MainLayer::MainLayer()
    : stageCreator_(stageCreator),
      size_(stageCreatorSize),
      currentIndex_(initialIndex),
      currentStage_(stageCreator[initialIndex]())
{
}

MainLayer::~MainLayer()
{
    if (currentStage_)
    {
        delete currentStage_;
        currentStage_ = nullptr;
    }
}

void MainLayer::goToNextStageInNextFrame()
{
    nextStageFlag_ = true;
}

void MainLayer::checkgoToNextStage()
{
    if (!nextStageFlag_)
        return;

    // delete current stage
    if (currentStage_)
    {
        delete currentStage_;
        currentStage_ = nullptr;
    }

    // create next stage
    if (++currentIndex_ < size_)
    {
        currentStage_ = stageCreator_[currentIndex_]();
    }
    else
    {
        currentIndex_ = 0;
        currentStage_ = stageCreator_[currentIndex_]();
    }

    nextStageFlag_ = false;
}

void MainLayer::update(World &world)
{
    checkgoToNextStage();

    auto &stage = currentStage();
    // update states of nodes in current Layer
    stage.update(world, *this);
};

void MainLayer::render(World &world)
{
    auto &stage = currentStage();
    stage.render(world, *this);
};

void MainLayer::renderFinished(World &world)
{
    auto &stage = currentStage();
    stage.renderFinished(world, *this);
}