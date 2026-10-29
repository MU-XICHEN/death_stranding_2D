#pragma once

#include "skills/Attack.h"

class World;
class Layer;

// Implementation of the half-circle slash attack
// Rotate a hitbox in a half-circle pattern, do the attack detection each frame.
// Current issues: miss detection if FPS is low.
class AttackHalfCircleSlash : public Attack
{
public:
    AttackHalfCircleSlash(int key, float duration)
        : Attack(key, duration) {}
    virtual ~AttackHalfCircleSlash() = default;

    virtual AABB getAttackContainer(World &world) override;
    virtual bool tryHit(World &world, std::uintptr_t entityPtr) override;
    virtual void render(World &world, Layer &layer) override;
};
