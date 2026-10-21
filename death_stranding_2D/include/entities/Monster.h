#pragma once

#include "core/GameBase.h"
#include "entities/Character.h"

class Monster : public Character
{
public:
    Monster(float x, float y, int width, int height,
            float speed = M_PIXEL_PER_SEC,
            float distance = M_DANGER_DISTANCE)
        : Character(x, y, width, height, speed), dangerDistance_(distance) {}

    virtual ~Monster() = default;

    virtual void move(World &world, Layer &layer) = 0;

    virtual void checkDangerDistance(World &world, Layer &layer);

protected:
    static GEB::DirectionVec calculateDirVecToHero(GEB::Position monsterCenter, GEB::Position heroCenter, float moveDistance);

    void awaken() { awakened_ = true; }

    bool awakened_ = false;
    float dangerDistance_;
};

class Beast : public Monster
{
public:
    using Monster::Monster;

    void move(World &world, Layer &layer) override;
    void update(World &world, Layer &layer) override;
};

class Ghost : public Monster
{
public:
    using Monster::Monster;

    void move(World &world, Layer &layer) override;
    void update(World &world, Layer &layer) override;

private:
    bool visible_ = false;
};