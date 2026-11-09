#include "layers/MainLayer.h"
#include "core/GameBase.h"

// ------------------------------------------ Layer ------------------------------------------

const int initialIndex = 0;

MainLayer::MainLayer()
    : stageCreator_(stageCreator),
      size_(stageCreatorSize),
      currentIndex_(initialIndex),
      currentStage_(stageCreator[initialIndex](selectedHeroType_))
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
    // Wait for the next frame before deleting the current stage.
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

    // Open the next stage, or return to the menu after the results.
    if (++currentIndex_ < size_)
    {
        currentStage_ = stageCreator_[currentIndex_](selectedHeroType_);
    }
    else
    {
        currentIndex_ = 0;
        currentStage_ = stageCreator_[currentIndex_](selectedHeroType_);
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
