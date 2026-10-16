#pragma once

#include "GamesEngineeringBase.h"
#include "types.h"
#include "Layer.h"
#include "Hero.h"
#include "Viewport.h"
#include "DoublyLinkedList.h"
#include "Map.h"

namespace GEB = GamesEngineeringBase;

class GameInfo
{

public:
    GameInfo(Map map, Hero hero, Viewport viewport);
    Hero &hero() { return hero_; }
    Map &map() { return map_; }
    Viewport &viewport() { return viewport_; }

    void updateHero(const Hero &hero) { hero_ = hero; }
    void updateViewport(const Viewport &viewport) { viewport_ = viewport; }

private:
    Map map_;
    Hero hero_;
    Viewport viewport_;
};

class World
{
public:
    World();
    ~World() = default;

    GEB::Window &canvas() { return canvas_; }
    GameInfo &gameInfo() { return gameInfo_; }

private:
    void gameLoop();
    void pushMainLayer();

    GEB::Structures::DoublylinkedList<Layer> layerList_;

    bool running_ = true;
    GEB::Window canvas_;

    GameInfo gameInfo_;

    // renderer
    void render();
    void renderLayers();
};
