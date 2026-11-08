#pragma once

#include "entities/Entity.h"
#include "skills/State.h"
#include "structure/Array.h"
#include "utils/Utils.h"
#include "skills/Attack.h"
#include "skills/Skill.h"
#include "world/AssetManager.h"

struct MapInfo;

class World;
class Layer;
class Attack;
class AttackInfo;
class Potion;

class ItemEntity : public Entity
{
public:
    ItemEntity(float x, float y, int width, int height)
        : Entity(x, y, width, height) {}
    virtual ~ItemEntity();

    GEB::Vec2 getOriginPoint() const
    {
        auto unitDirVec2 = GEB::Utils::transDirToUnitVec2(direction_);
        auto centerX = center().x;
        auto centerY = center().y;
        return GEB::Vec2{centerX + (unitDirVec2.x * width_ / 2),
                         centerY + (unitDirVec2.y * height_ / 2)};
    }

    GEB::Direction direction() const { return direction_; }

    void setColor(unsigned int r, unsigned int g, unsigned int b)
    {
        color_.r = r;
        color_.g = g;
        color_.b = b;
    }

    void initializeState(GEB::Pair<int, float> *newProp, int size);
    void updateState(const State &state) { state_ = state; }
    State &state() { return state_; }
    const State &state() const { return state_; }

    void calculateBoundaryTop(GEB::Boundary &boundary, const Entity *barrierItem);
    void calculateBoundaryBottom(GEB::Boundary &boundary, const Entity *barrierItem);
    void calculateBoundaryLeft(GEB::Boundary &boundary, const Entity *barrierItem);
    void calculateBoundaryRight(GEB::Boundary &boundary, const Entity *barrierItem);
    void calculateBoundaryImpl(GEB::Boundary &boundary, const Entity *barrierItem, GEB::Direction direction);
    GEB::Boundary calculateBoundary(const MapInfo &map, GEB::Direction direction, Entity **candidateItemHead, int size);

    void moveUp(float distance, float topBoundary);
    void moveDown(float distance, float bottomBoundary);
    void moveLeft(float distance, float leftBoundary);
    void moveRight(float distance, float rightBoundary);

    virtual void moveWithCollision(
        GEB::Vec2 distanceVec,
        const MapInfo &map,
        Entity **candidateItemHead,
        int size);

    // relevant to skills
    void addSkill(Skill *skill);
    GEB::Structures::Array<Skill *> &skills() { return skills_; }
    const GEB::Structures::Array<Skill *> &skills() const { return skills_; }

    // relevant to attacks
    void addAttack(Attack *attack);
    GEB::Structures::Array<Attack *> &attacks() { return attacks_; }
    const GEB::Structures::Array<Attack *> &attacks() const { return attacks_; }
    virtual void onAttacked(const Attack *, const AttackInfo &attackInfo);
    virtual bool canAttackTarget(const ItemEntity *target) const { return true; }
    virtual void onTargetDefeated(const ItemEntity &target) {}

    virtual void processSkill(World &world, Layer &layer);
    virtual void handleMovement(World &world, Layer &layer);
    virtual void processAttack(World &world, Layer &layer);
    virtual void tryTriggerSkills(World &world, Layer &layer);
    virtual void tryTriggerAttacks(World &world, Layer &layer);
    virtual AttackInfo createAttackInfo(Attack *attack) { return AttackInfo{}; };

    void coreUpdate(World &world, Layer &layer);
    // ❗Note: make sure that derived classed call coreUpdate first in their update method
    virtual void update(World &world, Layer &layer) = 0;
    virtual void render(World &world, Layer &layer) override;

    virtual bool onUsePotion(Potion *portion);

protected:
    virtual AssetId spriteAsset() const { return AssetId::None; }
    void renderSprite(World &world, AssetId asset);

    GEB::Color color_{0, 0, 0};
    State state_;
    GEB::Direction direction_ = GEB::Direction::Down;

    GEB::Structures::Array<Attack *> attacks_;
    GEB::Structures::Array<Skill *> skills_;

private:
    void renderStateInfo(World &world);
    bool deactivateIfNeeded(Layer &layer);
};
