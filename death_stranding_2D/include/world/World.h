#pragma once

#include "GamesEngineeringBase.h"
#include "core/GameBase.h"
#include "layers/Layer.h"
#include "world/GameInfo.h"
#include "renderer/TextRenderer.h"

class World
{
public:
    World();
    virtual ~World();

    GEB::Window &canvas() { return canvas_; }
    const TextRenderer &textRenderer() { return textRenderer_; }
    GameInfo &gameInfo() { return gameInfo_; }

private:
    void gameLoop();
    void refreshTimer();
    void initLayers();

    bool running_ = true;
    GEB::Window canvas_;

    GEB::Timer timer_;
    GameInfo gameInfo_;
    TextRenderer textRenderer_;
    GEB::Structures::Array<Layer *> layerList_;

    // Life Cycle
    void frameStart();
    void frameDraw();
    bool frameFinished();
};
