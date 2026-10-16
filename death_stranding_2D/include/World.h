#pragma once

#include "GamesEngineeringBase.h"
#include "GameBase.h"
#include "Layer.h"
#include "Hero.h"
#include "Viewport.h"
#include "DoublyLinkedList.h"
#include "Map.h"

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
    virtual ~World()
    {
        if (layerList_.empty())
            return;

        auto *headPointer = layerList_.front();
        while (headPointer)
        {
            // delete list nodes
            auto *temp = headPointer->next;

            headPointer->next = nullptr;
            headPointer->prev = nullptr;

            if (headPointer->data)
                delete headPointer->data;

            delete headPointer;

            headPointer = temp;
        }
    };

    GEB::Window &canvas() { return canvas_; }
    GEB::Timer &timer() { return timer_; }
    
    GameInfo &gameInfo() { return gameInfo_; }
    void setRunning(bool val) { running_ = val; }

private:
    void gameLoop();
    void pushMainLayer();

    bool running_ = true;
    GEB::Window canvas_;
    GEB::Timer timer_;

    GameInfo gameInfo_;
    GEB::Structures::DoublylinkedList<Layer *> layerList_;

    // renderer
    void renderLayers();
};
