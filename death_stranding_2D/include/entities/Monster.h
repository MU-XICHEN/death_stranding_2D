#pragma once

#include "core/GameBase.h"
#include "entities/ItemEntity.h"

class Monster : public ItemEntity
{
public:
    Monster(float x, float y, int width, int height,
            float speed,
            float distance)
        : ItemEntity(x, y, width, height, speed), dangerDistance_(distance) {}

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
    float dangerDistance_;
};

class Beast : public Monster
{
public:
    Beast(float x, float y, int width, int height,
          float speed = M_Beast_PIXEL_PER_SEC,
          float distance = M_Beast_DANGER_DISTANCE)
        : Monster(x, y, width, height, speed, distance) {}
};

class Ghost : public Monster
{
public:
    Ghost(float x, float y, int width, int height,
          float speed = M_Ghost_PIXEL_PER_SEC,
          float distance = M_Ghost_DANGER_DISTANCE)
        : Monster(x, y, width, height, speed, distance) {}

    void render(World &world, Layer &layer) override;

protected:
    virtual void checkDangerDistance(World &world, Layer &layer) override;

private:
    void appear() { visible_ = true; }

    bool visible_ = false;
};
