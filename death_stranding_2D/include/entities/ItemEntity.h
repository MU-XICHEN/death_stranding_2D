#pragma once

#include "entities/Entity.h"
#include "entities/State.h"

struct MapInfo;

class World;
class Layer;

// ItemEntity is used to anything which should consider collision.
class ItemEntity : public Entity
{
public:
    ItemEntity(float x, float y, int width, int height, float speed = PIXEL_PER_SEC)
        : Entity(x, y, width, height), speed_(speed) {}
    virtual ~ItemEntity() = default;

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

protected:
    GEB::Color color_{0, 0, 0};
    float speed_;
    State state_;
};
