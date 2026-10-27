#pragma once

#include "entities/Entity.h"
#include "structure/Array.h"

class Stage
{
public:
    Stage() = default;
    virtual ~Stage();
    void pushEntity(Entity *entity);
    virtual void handleEvents(World &world, Layer &layer);
    virtual void update(World &world, Layer &layer);
    virtual void render(World &world, Layer &layer);

    static bool isUpdating() { return stageUpdating_; }
    static void setUpdating(bool updating) { stageUpdating_ = updating; }

private:
    static inline bool stageUpdating_ = false;
    GEB::Structures::Array<Entity *> entityList_;
};
