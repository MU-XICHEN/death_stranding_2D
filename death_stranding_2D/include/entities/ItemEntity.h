#pragma once

#include "entities/Entity.h"
#include "entities/State.h"
#include "structure/Array.h"
#include "utils/Utils.h"

struct MapInfo;

class World;
class Layer;
class Attack;
class AttackInfo;

// ItemEntity is used to anything which should consider collision.
class ItemEntity : public Entity
{
public:
    ItemEntity(float x, float y, int width, int height, float speed = PIXEL_PER_SEC)
        : Entity(x, y, width, height), speed_(speed) {}
    virtual ~ItemEntity();

    // update method needs to be implemented by derived classes
    virtual void update(World &world, Layer &layer);

    // render mothod is implemented here and can be used by derived classes
    virtual void render(World &world, Layer &layer) override;

    void setColor(unsigned int r, unsigned int g, unsigned int b)
    {
        color_.r = r;
        color_.g = g;
        color_.b = b;
    }

    void updateState(const State &state) { state_ = state; }

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

    GEB::Vec2 getOriginPoint() const
    {
        auto unitDirVec2 = GEB::Utils::transDirToUnitVec2(direction_);
        auto centerX = center().x;
        auto centerY = center().y;
        return GEB::Vec2{centerX + (unitDirVec2.x * width_ / 2),
                         centerY + (unitDirVec2.y * height_ / 2)};
    }

    virtual void onAttack(const AttackInfo &attackInfo);
    inline void addAttack(Attack *attack) { attacks_.pushBack(attack); }
    GEB::Structures::Array<Attack *> &attacks() { return attacks_; }
    const GEB::Structures::Array<Attack *> &attacks() const { return attacks_; }

protected:
    GEB::Color color_{0, 0, 0};
    float speed_;
    State state_;
    GEB::Direction direction_ = GEB::Direction::None;

    GEB::Structures::Array<Attack *> attacks_;
};
