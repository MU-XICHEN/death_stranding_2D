#pragma once

#include "entities/Entity.h"
#include "core/GameBase.h"
#include "structure/Array.h"

class World;

class Layer
{
public:
    Layer() = default;
    virtual ~Layer()
    {
        for (int i = 0; i < itemEntities_.size(); ++i)
        {
            if (itemEntities_[i] == nullptr)
                continue;
            delete itemEntities_[i];
            itemEntities_[i] = nullptr;
        }
    };

    virtual void update(World &world)
    {
        for (int i = 0; i < itemEntities_.size(); ++i)
        {
            itemEntities_[i]->update(world, *this);
        }
    }
    virtual void render(World &world)
    {
        for (int i = 0; i < itemEntities_.size(); ++i)
        {
            itemEntities_[i]->render(world, *this);
        }
    };
    virtual void renderFinished(World &world) {}

    inline void pushEntity(Entity *entity)
    {
        itemEntities_.pushBack(entity);
    }

private:
    GEB::Structures::Array<Entity *> itemEntities_;
};