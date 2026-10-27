#pragma once

#include "GamesEngineeringBase.h"
#include "core/GameBase.h"
#include "layers/Layer.h"
#include "world/GameInfo.h"
#include "renderer/TextRenderer.h"
#include "renderer/SliceRenderer.h"
#include "world/AssetManager.h"

class World
{
public:
    World();
    virtual ~World();

    GEB::Window &canvas() { return canvas_; }
    AssetManager &assetManager() { return assetManager_; }
    SliceRenderer &sliceRenderer() { return sliceRenderer_; }
    TextRenderer &textRenderer() { return textRenderer_; }
    GameInfo &gameInfo() { return gameInfo_; }

private:
    void gameLoop();
    void refreshTimer();
    void initLayers();

    // Life Cycle
    void frameStart();
    void frameDraw();
    bool frameFinished();

    bool running_ = true;
    GEB::Window canvas_;

    GEB::Timer timer_;
    GameInfo gameInfo_;
    AssetManager assetManager_;
    TextRenderer textRenderer_;
    SliceRenderer sliceRenderer_;
    GEB::Structures::Array<Layer *> layerList_;
};
