#pragma once

#include "GamesEngineeringBase.h"
#include "core/GameBase.h"
#include "layers/Layer.h"
#include "utils/DoublyLinkedList.h"
#include "world/Map.h"
#include "world/GameInfo.h"

class World
{
public:
    World();
    virtual ~World();

    GEB::Window &canvas() { return canvas_; }
    GameInfo &gameInfo() { return gameInfo_; }

private:
    void gameLoop();
    void refreshTimer();
    void initLayers();

    bool running_ = true;
    GEB::Window canvas_;

    GEB::Timer timer_;
    GameInfo gameInfo_;
    GEB::Structures::DoublyLinkedList<Layer *> layerList_;

    // Life Cycle
    void frameStart();
    void frameDraw();
    bool frameFinished();
};
