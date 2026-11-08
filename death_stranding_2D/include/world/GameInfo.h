#pragma once

#include "entities/Hero.h"
#include "map/Map.h"
#include "world/Viewport.h"
#include "GamesEngineeringBase.h"

class World;

struct RunResult
{
    bool victory = false;
    HeroType heroType = HeroType::Swordsman;
    int kills = 0;
    int score = 0;
};

class GameInfo
{
public:
    GameInfo() : dt_(0),
                 viewport_(0, 0, VIEWPORT_WIDTH, VIEWPORT_HEIGHT) {}

    const HeroInfo &heroInfo() const { return heroInfo_; }
    const MapInfo &mapInfo() const { return mapInfo_; }
    const Viewport &viewport() const { return viewport_; }

    void updateDt(float dt) { dt_ = dt; }
    float dt() const { return dt_; }

    void updateHeroInfo(const HeroInfo &heroInfo) { heroInfo_ = heroInfo; }
    void updateViewport(const HeroInfo &heroInfo, const MapInfo &map);
    void updateMapInfo(const Map &map) { mapInfo_.setMap(&map); }
    void setRunResult(const RunResult &result) { runResult_ = result; }
    const RunResult &runResult() const { return runResult_; }
    bool victory() const { return runResult_.victory; }

private:
    float dt_;
    RunResult runResult_;

    Viewport viewport_;
    HeroInfo heroInfo_;
    MapInfo mapInfo_;
};
