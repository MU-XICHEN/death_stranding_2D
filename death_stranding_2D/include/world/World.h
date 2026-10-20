#pragma once

#include "GamesEngineeringBase.h"
#include "core/GameBase.h"
#include "layers/Layer.h"
#include "entities/Hero.h"
#include "entities/Environment.h"
#include "utils/DoublyLinkedList.h"
#include "world/Map.h"

class GameInfo
{

public:
    GameInfo(Map map, Hero hero, Environment environment);
    const Hero &hero() const { return hero_; }
    const Map &map() const { return map_; }
    const Environment &environment() const { return environment_; }
    float dt() { return dt_; }

    void updateDt(float dt) { dt_ = dt; };
    void updateHero(const Hero &hero) { hero_ = hero; }
    void updateEnvironment(const Environment &environment) { environment_ = environment; }

private:
    float dt_ = 0;
    Map map_;
    Hero hero_;
    Environment environment_;
};

class World
{
public:
    World();
    virtual ~World();

    GEB::Window &canvas() { return canvas_; }
    GEB::Timer &timer() { return timer_; }

    GameInfo &gameInfo() { return gameInfo_; }
    void setRunning(bool val) { running_ = val; }

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
