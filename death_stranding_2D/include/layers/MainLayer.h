#pragma once

#include "layers/Layer.h"
#include "stages/Stage.h"
#include "stages/Intro.h"
#include "stages/Menu.h"
#include "stages/Tally.h"
#include "stages/Play.h"

using StageCreator = Stage *(*)();

// Only use in this file.
static const StageCreator stageCreator[] = {
    []() -> Stage *
    { return new IntroStage(); },
    []() -> Stage *
    { return new MenuStage(); },
    []() -> Stage *
    { return new PlayStage(); },
    []() -> Stage *
    { return new TallyStage(); }};

static const int stageCreatorSize = sizeof(stageCreator) / sizeof(stageCreator[0]);

class MainLayer : public Layer
{
public:
    MainLayer();

    ~MainLayer();

    Stage &currentStage() const { return *currentStage_; }

    bool nextFrameFlag() { return nextFrameFlag_; }
    void setNextFrameFlag() { nextFrameFlag_ = true; }

    void checkGoToNextStage();

    virtual void update(World &world) override;
    virtual void render(World &world) override;

private:
    const StageCreator *stageCreator_ = nullptr;
    unsigned int size_ = 0;
    unsigned int currentIndex_ = 0;
    Stage *currentStage_ = nullptr;

    bool nextFrameFlag_ = false;
};
