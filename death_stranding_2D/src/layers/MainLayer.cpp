#include "layers/MainLayer.h"
#include "core/GameBase.h"

// ------------------------------------------ Layer ------------------------------------------

MainLayer::MainLayer()
    : stageCreator_(stageCreator),
      size_(stageCreatorSize),
      currentIndex_(2),
      currentStage_(stageCreator[currentIndex_]()) {}

MainLayer::~MainLayer()
{
    if (currentStage_)
    {
        delete currentStage_;
        currentStage_ = nullptr;
    }
}

void MainLayer::checkGoToNextStage()
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
}

void MainLayer::update(World &world)
{
    auto &stage = currentStage();
    // update states of nodes in current Layer
    stage.updateEntities(world, *this);
};

void MainLayer::render(World &world)
{
    auto &stage = currentStage();
    stage.renderEntities(world, *this);
};