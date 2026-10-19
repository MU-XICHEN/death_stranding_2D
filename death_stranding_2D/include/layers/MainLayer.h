#pragma once

#include "layers/Layer.h"
#include "stages/Intro.h"
#include "stages/Menu.h"
#include "stages/Tally.h"
#include "stages/Play.h"

// Only use in this file.
static const StageCreator mainLayerStageCreator[] = {
    []() -> Stage *
    { return new IntroStage(); },
    []() -> Stage *
    { return new MenuStage(); },
    []() -> Stage *
    { return new PlayStage(); },
    []() -> Stage *
    { return new TallyStage(); }};

static const int stageCreatorSize = sizeof(mainLayerStageCreator) / sizeof(mainLayerStageCreator[0]);

class MainLayer : public Layer
{
public:
    MainLayer() : Layer(mainLayerStageCreator, stageCreatorSize, 2) {};
};
