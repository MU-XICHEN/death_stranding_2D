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

    const HeroInfo &heroInfo() const { return heroInfo_; }
    const Map &map() const { return map_; }
    const Viewport &viewport() const { return viewport_; }

    void updateDt(float dt) { dt_ = dt; }
    float dt() const { return dt_; }

    void updateHeroInfo(const HeroInfo &heroInfo) { heroInfo_ = heroInfo; }
    void updateMap(const Map &map) { map_ = map; }
    void updateViewport(const HeroInfo &heroInfo);

    void loadNewMap(const Map &map) { map_ = map; }

    void initialize();

private:
    float dt_;

    Viewport viewport_;
    HeroInfo heroInfo_;
    Map map_;
};