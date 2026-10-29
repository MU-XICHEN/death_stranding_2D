#pragma once

#include "entities/Entity.h"
#include "structure/Array.h"
#include "utils/KeyState.h"

class Stage
{
public:
    Stage() = default;
    virtual ~Stage();
    void pushEntity(Entity *entity);
    virtual void handleEvents(World &world, Layer &layer);
    virtual void update(World &world, Layer &layer);
    virtual void render(World &world, Layer &layer);
    virtual void renderFinished(World &world, Layer &layer);

    static inline KeyState enterState{};

private:
    GEB::Structures::Array<Entity *> entityList_;
};
