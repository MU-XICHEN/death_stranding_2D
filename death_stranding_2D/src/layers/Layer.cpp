#include "layers/Layer.h"
#include "core/GameBase.h"

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