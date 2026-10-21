#pragma once

#include "core/GameBase.h"
#include "entities/Character.h"

class Monster : public Character
{
public:
    Monster(float x, float y, int width, int height,
            float speed,
            float distance)
        : Character(x, y, width, height, speed), dangerDistance_(distance) {}

    virtual ~Monster() = default;

protected:
    static GEB::DirectionVec calculateDirVecToHero(GEB::Position monsterCenter, GEB::Position heroCenter, float moveDistance);
    virtual void move(World &world, Layer &layer) = 0;

    virtual void checkDangerDistance(World &world, Layer &layer);

    void awaken() { awakened_ = true; }

    bool awakened_ = false;
    float dangerDistance_;
};

class Beast : public Monster
{
public:
    Beast(float x, float y, int width, int height,
          float speed = M_Beast_PIXEL_PER_SEC,
          float distance = M_Beast_DANGER_DISTANCE)
        : Monster(x, y, width, height, speed, distance) {}

    void move(World &world, Layer &layer) override;
    void update(World &world, Layer &layer) override;
};

class Ghost : public Monster
{
public:
    Ghost(float x, float y, int width, int height,
          float speed = M_Ghost_PIXEL_PER_SEC,
          float distance = M_Ghost_DANGER_DISTANCE)
        : Monster(x, y, width, height, speed, distance) {}

    void move(World &world, Layer &layer) override;
    void update(World &world, Layer &layer) override;
    void render(World &world, Layer &layer) override;

protected:
    virtual void checkDangerDistance(World &world, Layer &layer) override;

private:
    void appear() { visible_ = true; }

    bool visible_ = false;
};
