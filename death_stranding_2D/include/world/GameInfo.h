#pragma once

#include "entities/Hero.h"
#include "world/Map.h"
#include "world/Viewport.h"
#include "GamesEngineeringBase.h"

class World;

class GameInfo
{
public:
    GameInfo() : dt_(0),
                 viewport_(0, 0, VIEWPORT_WIDTH, VIEWPORT_HEIGHT) {}

    const Hero &hero() const { return hero_; }
    const Map &map() const { return map_; }
    const Viewport &viewport() const { return viewport_; }

    void updateDt(float dt) { dt_ = dt; }
    float dt() const { return dt_; }

    void updateHero(const Hero &hero) { hero_ = hero; }
    void updateViewport(const Hero &hero);

    void loadNewMap(const Map &map) { map_ = map; }

    void initialize();

private:
    float dt_;

    Viewport viewport_;

    Map map_;
    Hero hero_;
};