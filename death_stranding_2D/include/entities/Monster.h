#pragma once

#include "core/GameBase.h"
#include "entities/ItemEntity.h"

class Monster : public ItemEntity
{
public:
    Monster(float x, float y, int width, int height)
        : ItemEntity(x, y, width, height) {}

    virtual ~Monster() = default;

    virtual void update(World &world, Layer &layer) override;
    virtual void handleMovement(World &world, Layer &layer) override;

protected:
    static GEB::Direction transDirVecToDiretion(const GEB::Vec2 &dirVec, GEB::Direction prevDirection);
    static inline GEB::Vec2 calculateDirVecToHero(GEB::Position monsterCenter, GEB::Position heroCenter)
    {
        float dx = heroCenter.x - monsterCenter.x;
        float dy = heroCenter.y - monsterCenter.y;
        return GEB::Vec2{dx, dy};
    }

    void awaken() { awakened_ = true; }
    virtual void checkDangerDistance(World &world, Layer &layer);

    bool awakened_ = false;
};

class Beast : public Monster
{
public:
    Beast(float x, float y, int width, int height)
        : Monster(x, y, width, height)
    {
        // initialize states
        GEB::Pair<int, float> props[] = {
            {State::HEALTH, M_BEAST_HP},
            {State::SPEED, M_BEAST_PIXEL_PER_SEC},
            {State::PERCEPTION_RANGE, M_BEAST_DANGER_DISTANCE},
            {State::DEFENSE, M_BEAST_DEFENSE},
            {State::DAMAGE, M_BEAST_DAMAGE}};
        initializeState(props, sizeof(props) / sizeof(props[0]));
    }
};

class Ghost : public Monster
{
public:
    Ghost(float x, float y, int width, int height)
        : Monster(x, y, width, height)
    {
        // initialize states
        GEB::Pair<int, float> props[] = {
            {State::HEALTH, M_GHOST_HP},
            {State::SPEED, M_GHOST_PIXEL_PER_SEC},
            {State::PERCEPTION_RANGE, M_GHOST_DANGER_DISTANCE},
            {State::DEFENSE, M_GHOST_DEFENSE},
            {State::DAMAGE, M_GHOST_DAMAGE}};
        initializeState(props, sizeof(props) / sizeof(props[0]));
    }

    void render(World &world, Layer &layer) override;

protected:
    virtual void checkDangerDistance(World &world, Layer &layer) override;

private:
    void appear() { visible_ = true; }

    bool visible_ = false;
};
